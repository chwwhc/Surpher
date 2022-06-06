#include "Parser.hpp"

template<typename... T>
bool Parser::match(T... types) {
    if((... || check(types))){
    any_token();
    return true;
}
return false;
}

template<typename... T>
std::shared_ptr<Expr> Parser::parseBinary(const std::function<std::shared_ptr<Expr>()>& operand, T... types){
    std::shared_ptr<Expr> lower_expr = operand();

    while(match(types...)){
        Token op = previous();
        std::shared_ptr<Expr> right = operand();
        lower_expr = std::shared_ptr<Expr>(new Binary(lower_expr, op, right));
    }

    return lower_expr;
}

std::shared_ptr<Expr> Parser::primary() {
    if(match(FALSE)){
        return std::shared_ptr<Expr>(new Literal(false));
    }else if(match(TRUE)){
        return std::shared_ptr<Expr>(new Literal(true));
    }else if(match(NONE)){
        return std::shared_ptr<Expr>(new Literal(nullptr));
    }else if(match(NUMBER, STRING)){
        return std::shared_ptr<Expr>(new Literal(previous().literal));
    }else if(match(LEFT_PAREN)){
        std::shared_ptr<Expr> expr_in = expression();
        consume(RIGHT_PAREN, "LOREM IPSUM");
        return std::shared_ptr<Expr>(new Group(expr_in));
    }
    throw error(peek(), "Expected expression.");
}

std::shared_ptr<Expr> Parser::expression() {
    return logical_or();
}

Token Parser::consume(TokenType type, std::string_view message) {
    if(check(type)){
        return any_token();
    }
   throw error(peek(), message);
}

Parser::Parser(std::vector<Token> &tokens) : tokens(tokens){

}

Parser::ParseError Parser::error(const Token &token, std::string_view message) {
    ::error(token, message);
    return ParseError{""};
}

std::shared_ptr<Expr> Parser::parse() {
    try{
        return expression();
    }catch (ParseError &e){
        return nullptr;
    }
}






