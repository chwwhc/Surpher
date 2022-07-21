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

static Interpreter interpreter;

enum mode{
    INTERPRET,
    PRINT_AST
};

void printAST(const std::list<std::shared_ptr<Stmt>>& statements){
    ASTPrinter printer;
    std::cout << "[";
    for(const auto& s: statements) std::cout << std::any_cast<std::string>(s->accept(printer)) << ",\n";
    std::cout << "]\n";
}

void run(const std::string &source, const mode& mode) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::list<std::shared_ptr<Stmt>> statements = parser.parse();
    interpreter.appendScriptBack(statements);

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
                std::ifstream input_file(e.script);
                if (input_file.fail()) {
                    std::cerr << "Failed to open file " << e.script << ": " << std::endl;
                    std::exit(74);
                }

                std::stringstream source_code;
                source_code << input_file.rdbuf();

                Lexer _lexer(source_code.str());
                std::vector<Token> _tokens = _lexer.scanTokens();
                Parser _parser(_tokens);
                std::list<std::shared_ptr<Stmt>> _statements = _parser.parse();

                interpreter.appendScriptFront(_statements);
            }

            break;
        default:
            interpreter.interpret();
    }
}

void runScript(const std::string &path, const mode& mode) {

    std::ifstream input_file(path);
    if (input_file.fail()) {
        std::cerr << "Failed to open file " << path << ": " << std::endl;
        std::exit(74);
    }

    std::stringstream source_code;
    source_code << input_file.rdbuf();

    run(source_code.str(), mode);
    if (had_error) {
        std::exit(65);
    } else if (had_runtime_error) {
        std::exit(70);
    }
}

void runRepl() {
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
            runScript(file_path, INTERPRET);
            continue;
        } else if(cmd.substr(0, 9) == "!printAST"){
            uint32_t curr = 9;
            while (curr < cmd.size() && cmd[curr] == ' ') {
                curr++;
            }
            std::string file_path = cmd.substr(curr, cmd.size() - curr);
            runScript(file_path, PRINT_AST);
            continue;
        }
        run(cmd, INTERPRET);
        cmd.clear();
        had_error = false;
    }
}

int main(int argc, char *argv[]) {
    /*
    if (argc > 2) {
        std::cout << "Usage: Surpher [script]" << std::endl;
        std::exit(64);
    } else if (argc == 2) {
        runScript(argv[0]);
    } else {
     */

    runRepl();
    /*
}
     */
}
