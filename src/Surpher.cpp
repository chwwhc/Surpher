#include <iostream>
#include <istream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "AstPrinter.hpp"
#include "Error.hpp"
#include "Interpreter.hpp"
#include "Resolver.hpp"

static Interpreter interpreter{};

void run(const std::string &source) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    if (had_error) {
        return;
    }

    Resolver resolver(interpreter);
    resolver.resolve(statements);

    if (had_error) {
        return;
    }

    interpreter.interpret(statements);
}

void runScript(const std::string &path) {

    std::ifstream input_file(path);
    if (input_file.fail()) {
        std::cerr << "Failed to open file " << path << ": " << std::endl;
        std::exit(74);
    }

    std::stringstream source_code;
    source_code << input_file.rdbuf();


    run(source_code.str());
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
            runScript(file_path);
            continue;
        }
        run(cmd);
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
