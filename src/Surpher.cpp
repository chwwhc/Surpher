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

void run(const std::string &source, Interpreter &interpreter);

void runScript(const std::string &path, Interpreter &interpreter) {
    std::ifstream input_file(path);
    if (input_file.fail()) {
        std::cerr << "Failed to open file " << path << ": " << std::endl;
        std::exit(74);
    }

    std::stringstream source_code;
    source_code << input_file.rdbuf();

    run(source_code.str(), interpreter);
    if (had_error) {
        std::exit(65);
    } else if (had_runtime_error) {
        std::exit(70);
    }
}

void run(const std::string &source, Interpreter &interpreter) {
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
        runScript(e.script, interpreter);
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
            runScript(file_path, interpreter);
            continue;
        }
        run(cmd, interpreter);
        cmd.clear();
        had_error = false;
    }
}

int main(int argc, char *argv[]) {
    runRepl();
}
