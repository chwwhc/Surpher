package SurpherLangMain;

/**
 * Represents a lexical token of Surpher
 */
class Token {
    /**
     * A collection of valid token types
     */
    static enum TokenType {
        // symbol
        LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,
        COMMA, DOT, SINGLE_SEMICOLON, DOUBLE_SEMICOLON, PLUS, MINUS, PERCENT, STAR, SLASH,
        SINGLE_BAR, DOUBLE_BAR, CARET, SINGLE_AMPERSAND, DOUBLE_AMPERSAND,
        BANG, BANG_EQUAL, SINGLE_EQUAL, DOUBLE_EQUAL, GREATER, GREATER_EQUAL,
        LESS, LESS_EQUAL,

        // literal
        IDENTIFIER, STRING, INTEGER, FLOAT,

        // reserved
        CLASS, ELSE, FALSE, FUN, FOR, IF, NONE, RETURN, SUPER, SELF,
        TRUE, VAR, WHILE, DO, MATCH, CASE, BREAK, CONTINUE, NEWTYPE,

        EOF
    }

    private final TokenType aType;
    private final String aLexeme;
    private final Object aLiteral;
    private final int aLine;

    /**
     * Constructor for Surpher lexical tokens
     * 
     * @param pType    token type
     * @param pLexeme  string represented lexeme
     * @param pLiteral original literal
     * @param pLine    location info
     */
    Token(TokenType pType, String pLexeme, Object pLiteral, int pLine) {
        aType = pType;
        aLexeme = pLexeme;
        aLiteral = pLiteral;
        aLine = pLine;
    }

    /**
     * Getter method to retrieve the lexeme
     * 
     * @return
     */
    String getLexeme() {
        return aLexeme;
    }

    @Override
    public String toString() {
        return "lexical type: " + aType + " , lexeme: " + aLexeme + " , literal: " + aLiteral;
    }

}
