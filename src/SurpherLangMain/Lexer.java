package SurpherLangMain;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Stack;

import SurpherLangMain.Token.TokenType;

/**
 * Lexer for the Surpher language
 */
class Lexer {
    private final String aSource;
    private final List<Token> aTokenList = new LinkedList<>();
    private static final Map<String, TokenType> aKeywords = new HashMap<>();
    private int aStart = 0;
    private int aCurrent = 0;
    private int aLine = 1;

    /**
     * Automatically adds the reserved keywords
     */
    static {
        aKeywords.put("class", TokenType.CLASS);
        aKeywords.put("else", TokenType.ELSE);
        aKeywords.put("false", TokenType.FALSE);
        aKeywords.put("fun", TokenType.FUN);
        aKeywords.put("for", TokenType.FOR);
        aKeywords.put("if", TokenType.IF);
        aKeywords.put("none", TokenType.NONE);
        aKeywords.put("return", TokenType.RETURN);
        aKeywords.put("super", TokenType.SUPER);
        aKeywords.put("self", TokenType.SELF);
        aKeywords.put("true", TokenType.TRUE);
        aKeywords.put("var", TokenType.VAR);
        aKeywords.put("while", TokenType.WHILE);
        aKeywords.put("do", TokenType.DO);
        aKeywords.put("match", TokenType.MATCH);
        aKeywords.put("case", TokenType.CASE);
        aKeywords.put("break", TokenType.BREAK);
        aKeywords.put("continue", TokenType.CONTINUE);
        aKeywords.put("newtype", TokenType.NEWTYPE);
    }

    /**
     * Constructor for the Surpher lexer
     * 
     * @param pSource source code
     */
    Lexer(String pSource) {
        aSource = pSource;
    }

    /**
     * Determines if the given char is a valid identifier
     * element([a-zA-Z_][0-9a-zA-Z_]*)
     * 
     * @param pChar
     * @return
     */
    private boolean isAlpha(char pChar) {
        return (pChar >= 'a' && pChar <= 'z') || (pChar >= 'A' && pChar <= 'Z') || pChar == '_';
    }

    /**
     * Determines if the given char is a valid identifier
     * element([a-zA-Z_][0-9a-zA-Z_]*) or a valid digit
     * 
     * @param pChar
     * @return
     */
    private boolean isAlphaNumeric(char pChar) {
        return isAlpha(pChar) || isDigit(pChar);
    }

    /**
     * Determines if the given char is a valid digit
     * 
     * @param pChar
     * @return
     */
    private boolean isDigit(char pChar) {
        return pChar >= '0' && pChar <= '9';
    }

    /**
     * Attempts to match the given char; increments the cursor if it succeeds
     * 
     * @param pExpected given char to be matched
     * @return
     */
    private boolean matchNext(char pExpected) {
        if (isAtEnd() || aSource.charAt(aCurrent) != pExpected) {
            return false;
        }
        aCurrent++;
        return true;
    }

    /**
     * Attempts to match a Number type of Surpher
     */
    private void matchNumber() {
        // check if the first char is a digit
        while (isDigit(lookAHead(0))) {
            anyChar();
        }

        if (lookAHead(0) == '.' && isDigit(lookAHead(1))) {
            // case 1: FLOAT
            anyChar();
            while (isDigit(lookAHead(0))) {
                anyChar();
            }
            addToken(TokenType.FLOAT, Double.parseDouble(aSource.substring(aStart, aCurrent)));
            return;
        } else {
            // case 2: INTEGER
            while (isDigit(lookAHead(0))) {
                anyChar();
            }
            addToken(TokenType.INTEGER, Integer.parseInt(aSource.substring(aStart, aCurrent)));
            return;
        }

    }

    /**
     * Attempts to match a Identifier type or a reserved keyword of Surpher
     */
    private void matchIdentifierAndReserved() {
        // check if the leading char is a valid identifier element
        while (isAlphaNumeric(lookAHead(0))) {
            anyChar();
        }
        String word = aSource.substring(aStart, aCurrent);
        TokenType type = aKeywords.get(word);
        if (type == null) {
            // case 1: identifier
            type = TokenType.IDENTIFIER;
        }
        // case 2: keyword
        addToken(type);
    }

    /**
     * Attempts to match a String type of Surpher
     */
    private void matchString() {
        // skip over the string body
        while (!isAtEnd() && lookAHead(0) != '"') {
            if (lookAHead(0) == '\n') {
                aLine++;
            }
            anyChar();
        }

        // error: unterminated string
        if (isAtEnd()) {
            JSurpher.error(aLine, "unterminated string");
            return;
        }

        // skip over the right quotation
        anyChar();

        String value = aSource.substring(aStart + 1, aCurrent - 1);
        addToken(TokenType.STRING, value);
    }

    /**
     * Skips over a comment section of Surpher
     */
    private void skipComment() {
        // use a stack to identify a comment section
        Stack<Integer> commentStack = new Stack<>();
        commentStack.push(0);

        while (!commentStack.empty() && !isAtEnd()) {
            // error: unterminated comment
            if (isAtEnd()) {
                JSurpher.error(aLine, "unterminated comment");
                return;
            }

            // update the stack accordingly
            if (matchNext('(')) {
                if (matchNext('*')) {
                    commentStack.push(0);
                }
            } else if (matchNext('*')) {
                if (matchNext(')')) {
                    commentStack.pop();
                }
            }
            anyChar();
        }
    }

    /**
     * Reads and analyzes the next lexical token in the source code
     */
    private void scanToken() {
        char nextChar = anyChar();
        switch (nextChar) {
            case '(' -> {
                if (matchNext('*')) {
                    skipComment();
                } else {
                    addToken(TokenType.LEFT_PAREN);
                }
            }
            case ')' -> addToken(TokenType.RIGHT_PAREN);
            case '{' -> addToken(TokenType.LEFT_BRACE);
            case '}' -> addToken(TokenType.RIGHT_BRACE);
            case '[' -> addToken(TokenType.LEFT_BRACKET);
            case ']' -> addToken(TokenType.RIGHT_BRACKET);
            case ',' -> addToken(TokenType.COMMA);
            case '.' -> addToken(TokenType.DOT);
            case ';' -> addToken(matchNext(';') ? TokenType.DOUBLE_SEMICOLON : TokenType.SINGLE_SEMICOLON);
            case '+' -> addToken(TokenType.PLUS);
            case '-' -> addToken(TokenType.MINUS);
            case '%' -> addToken(TokenType.PERCENT);
            case '*' -> addToken(TokenType.STAR);
            case '/' -> addToken(TokenType.SLASH);
            case '|' -> addToken(matchNext('|') ? TokenType.DOUBLE_BAR : TokenType.SINGLE_BAR);
            case '^' -> addToken(TokenType.CARET);
            case '&' -> addToken(matchNext('&') ? TokenType.DOUBLE_AMPERSAND : TokenType.SINGLE_AMPERSAND);
            case '!' -> addToken(matchNext('=') ? TokenType.BANG_EQUAL : TokenType.BANG);
            case '=' -> addToken(matchNext('=') ? TokenType.DOUBLE_EQUAL : TokenType.SINGLE_EQUAL);
            case '>' -> addToken(matchNext('=') ? TokenType.GREATER_EQUAL : TokenType.GREATER);
            case '<' -> addToken(matchNext('=') ? TokenType.LESS_EQUAL : TokenType.LESS);
            case ' ', '\r', '\t' -> {
            }
            case '"' -> matchString();
            case '\n' -> aLine++;
            default -> {
                if (isDigit(nextChar)) {
                    matchNumber();
                } else if (isAlpha(nextChar)) {
                    matchIdentifierAndReserved();
                } else {
                    JSurpher.error(aCurrent, "invalid character");
                }

            }
        }
    }

    /**
     * Consumes a char in the source code; increments the cursor by 1
     * 
     * @return the next char in the source code
     */
    private char anyChar() {
        return aSource.charAt(aCurrent++);
    }

    /**
     * Appends a lexical token with a literal to the token list
     * 
     * @param pType    the type of the lexical token
     * @param pLiteral the literal of the lexical token
     */
    private void addToken(TokenType pType, Object pLiteral) {
        String text = aSource.substring(aStart, aCurrent);
        aTokenList.add(new Token(pType, text, pLiteral, aLine));
    }

    /**
     * Appends a lexical token without a literal to the token list
     * 
     * @param pType the type of the lexical token
     */
    private void addToken(TokenType pType) {
        addToken(pType, null);
    }

    /**
     * Peeks the source code without updating the cursor
     * 
     * @param pOffset location of the desired char
     * @return the char at the desired location
     */
    private char lookAHead(int pOffset) {
        if (isAtEnd() || aCurrent + pOffset >= aSource.length()) {
            return '\0';
        } else {
            return aSource.charAt(aCurrent + pOffset);
        }
    }

    /**
     * Checks if the cursor reaches the end of the source code
     * 
     * @return
     */
    private boolean isAtEnd() {
        return aCurrent >= aSource.length();
    }

    /**
     * Reads throught the source code and generates a list of lexical tokens
     * 
     * @return a list of lexical tokens of Surpher
     */
    List<Token> scanTokens() {
        while (!isAtEnd()) {
            aStart = aCurrent;
            scanToken();
        }

        aTokenList.add(new Token(TokenType.EOF, "", null, aLine));
        return aTokenList;
    }

}
