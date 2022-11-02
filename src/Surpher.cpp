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

Interpreter interpreter;
void run(const std::string &source);

void runScript(const std::string &path) {
    std::ifstream input_file(path);
    if (input_file.fail()) {
        std::cerr << "Failed to open file " << path << ": " << std::endl;
        return;
    }

    std::stringstream source_code;
    source_code << input_file.rdbuf();

    run(source_code.str());
    if (had_error) {
        return;
    } else if (had_runtime_error) {
        return;
    }
}

void run(const std::string &source) {
    Lexer lexer(source);
    std::vector<Token> tokens{lexer.scanTokens()};
    Parser parser{tokens};
    std::list<std::shared_ptr<Stmt>> script {parser.parse()};

    interpreter.appendScriptFront(script);

    if (had_error) return;

    Resolver resolver(interpreter);
    resolver.resolve(script);

    if (had_error) return;

    try {
        interpreter.interpret();
    } catch (ImportError &e) {
        runScript(e.script);
        interpreter.interpret();
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
            runScript(file_path);
            continue;
        }
        run(cmd);
        cmd.clear();
        had_error = false;
    }
}

int main(int argc, char *argv[]) {
    if(argc == 1){
        runRepl();
    }else if(argc == 2){
        runScript(argv[1]);
        runRepl();
    }else{
        std::cerr << "Usage: Surpher [path to script]*\n";
    }
}
