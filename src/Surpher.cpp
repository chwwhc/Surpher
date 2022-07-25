#include <iostream>
#include <istream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Error.hpp"
#include "Interpreter.hpp"
#include "Resolver.hpp"
#include "./tool/ASTPrinter.hpp"

enum mode{
    INTERPRET,
    PRINT_AST
};

void run(const std::string &source, const mode& mode, Interpreter& interpreter);

void printAST(const std::list<std::shared_ptr<Stmt>>& statements){
    ASTPrinter printer;
    std::cout << "[";
    for(const auto& s: statements) std::cout << std::any_cast<std::string>(s->accept(printer)) << ",\n";
    std::cout << "]\n";
}

void runScript(const std::string &path, const mode& mode, Interpreter& interpreter) {
    std::ifstream input_file(path);
    if (input_file.fail()) {
        std::cerr << "Failed to open file " << path << ": " << std::endl;
        std::exit(74);
    }

    std::stringstream source_code;
    source_code << input_file.rdbuf();

    run(source_code.str(), mode, interpreter);
    if (had_error) {
        std::exit(65);
    } else if (had_runtime_error) {
        std::exit(70);
    }
}

void run(const std::string &source, const mode& mode, Interpreter& interpreter) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    Parser parser(tokens);
    std::list<std::shared_ptr<Stmt>> statements = parser.parse();

    interpreter.appendScriptFront(statements);

    if (had_error) {
        return;
    }

    Resolver resolver(interpreter);
    resolver.resolve(statements);

    if (had_error) {
        return;
    }

    switch (mode) {
        case PRINT_AST:
            printAST(statements);
            break;
        case INTERPRET:
            try{
                interpreter.interpret();
            } catch (ImportError& e) {
                if(!e.module_name.empty()){
                    interpreter.setPrefix(e.module_name + ".");
                }else{
                    interpreter.setPrefix(e.module_name);
                }
                runScript(e.script, INTERPRET, interpreter);
                interpreter.interpret();
                interpreter.setPrefix("");
            }
            break;
        default:
            interpreter.interpret();
    }
}

void runRepl() {
    Interpreter interpreter;
    std::string cmd;
    while (true) {
        std::cout << "Surpher> ";
        std::getline(std::cin, cmd);
        if (cmd == "!quit") {
            std::cout << std::endl;
            std::cout << "Bye!!!" << std::endl;
            std::exit(0);
        } else if (cmd.substr(0, 4) == "!run") {
            uint32_t curr = 4;
            while (curr < cmd.size() && cmd[curr] == ' ') {
                curr++;
            }
            std::string file_path = cmd.substr(curr, cmd.size() - curr);
            runScript(file_path, INTERPRET, interpreter);
            continue;
        } else if(cmd.substr(0, 9) == "!printAST"){
            uint32_t curr = 9;
            while (curr < cmd.size() && cmd[curr] == ' ') {
                curr++;
            }
            std::string file_path = cmd.substr(curr, cmd.size() - curr);
            runScript(file_path, PRINT_AST, interpreter);
            continue;
        }
        run(cmd, INTERPRET, interpreter);
        cmd.clear();
        had_error = false;
    }
}

int main(int argc, char *argv[]) {
    runRepl();
}
