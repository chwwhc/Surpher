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

void run(const std::string &source) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    AstPrinter ast_printer{};
    std::shared_ptr<Expr> parser_result = parser.parse();
    Interpreter interpreter;

    if (had_error) {
        return;
    }
    interpreter.interpret(parser_result);

   // std::cout << ast_printer.printAst(parser_result) << std::endl;
}

void runScript(const std::string &path) {
    std::ifstream input_file(path);
    std::ostringstream source_code;
    source_code << input_file.rdbuf();

    run(source_code.str());
    if(had_error){
        exit(65);
    }else if(had_runtime_error){
        exit(70);
    }
}

void runRepl() {
    std::string cmd;
    while (true) {
        std::cout << "Surpher > ";
        std::getline(std::cin, cmd);
        if (cmd == "quit!!!") {
            std::cout << std::endl;
            std::cout << "Bye!!!" << std::endl;
            exit(0);
        }
        run(cmd);
        cmd.clear();
        had_error = false;
    }
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        std::cout << "Usage: Surpher [script]" << std::endl;
        exit(64);
    } else if (argc == 2) {
        runScript(argv[0]);
    } else {
        runRepl();
    }
}
