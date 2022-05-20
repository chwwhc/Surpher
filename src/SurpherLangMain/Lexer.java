package SurpherLangMain;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Stack;
import java.util.function.Supplier;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.Predicate;

import SurpherLangMain.Token.TokenType;

/**
 * Lexer for the Surpher language
 */
class Lexer {
    private String aSource;
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

    private Consumer<TokenType> addToken = pType -> addToken(pType, null);
    private Supplier<Character> anyChar = () -> aSource.charAt(aCurrent++);
    private Supplier<Boolean> isAtEnd = () -> aCurrent >= aSource.length();
    private Predicate<Character> isDigit = pChar -> pChar >= '0' && pChar <= '9';
    private Predicate<Character> isAlpha = pChar -> (pChar >= 'a' && pChar <= 'z') || (pChar >= 'A' && pChar <= 'Z')
            || pChar == '_';
    private Predicate<Character> isAlphaNumeric = isAlpha.or(isDigit);
    private Function<Integer, Character> lookAHead = pOffset -> {
        if (isAtEnd.get() || aCurrent + pOffset >= aSource.length()) {
            return '\0';
        } else {
            return aSource.charAt(aCurrent + pOffset);
        }
    };
    private Function<Character, Boolean> matchNext = pExpectedChar -> {
        if (isAtEnd.get() || aSource.charAt(aCurrent) != pExpectedChar) {
            return false;
        }
        aCurrent++;
        return true;
    };

    /**
     * Constructor for the Surpher lexer
     * 
     * @param pSource source code
     */
    Lexer(String pSource) {
        aSource = pSource;
    }

    /**
     * Attempts to match a Number type of Surpher
     */
    private void matchNumber() {

        // check if the first char is a digit
        while (isDigit.test(lookAHead.apply(0))) {
            anyChar.get();
        }

        if (lookAHead.apply(0) == '.' && isDigit.test(lookAHead.apply(1))) {
            // case 1: FLOAT
            anyChar.get();
            while (isDigit.test(lookAHead.apply(0))) {
                anyChar.get();
            }
            addToken(TokenType.FLOAT, Double.parseDouble(aSource.substring(aStart, aCurrent)));
            return;
        } else {
            // case 2: INTEGER
            while (isDigit.test(lookAHead.apply(0))) {
                anyChar.get();
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
        while (isAlphaNumeric.test(lookAHead.apply(0))) {
            anyChar.get();
        }
        String word = aSource.substring(aStart, aCurrent);
        TokenType type = aKeywords.get(word);
        if (type == null) {
            // case 1: identifier
            type = TokenType.IDENTIFIER;
        }
        // case 2: keyword
        addToken.accept(type);
    }

    /**
     * Attempts to match a String type of Surpher
     */
    private void matchString() {
        // skip over the string body
        while (!isAtEnd.get() && lookAHead.apply(0) != '"') {
            if (lookAHead.apply(0) == '\n') {
                aLine++;
            }
            anyChar.get();
        }

        // error: unterminated string
        if (isAtEnd.get()) {
            JSurpher.error(aLine, "unterminated string");
            return;
        }

        // skip over the right quotation
        anyChar.get();

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

        while (!commentStack.empty() && !isAtEnd.get()) {
            // error: unterminated comment
            if (isAtEnd.get()) {
                JSurpher.error(aLine, "unterminated comment");
                return;
            }

            // update the stack accordingly
            if (matchNext.apply('(')) {
                if (matchNext.apply('*')) {
                    commentStack.push(0);
                }
            } else if (matchNext.apply('*')) {
                if (matchNext.apply(')')) {
                    commentStack.pop();
                }
            }
            if (!isAtEnd.get())
                anyChar.get();
        }
    }

    /**
     * Reads and analyzes the next lexical token in the source code
     */
    private void scanToken() {
        char nextChar = anyChar.get();
        switch (nextChar) {
            case '(' -> {
                if (matchNext.apply('*')) {
                    skipComment();
                } else {
                    addToken.accept(TokenType.LEFT_PAREN);
                }
            }
            case ')' -> addToken.accept(TokenType.RIGHT_PAREN);
            case '{' -> addToken.accept(TokenType.LEFT_BRACE);
            case '}' -> addToken.accept(TokenType.RIGHT_BRACE);
            case '[' -> addToken.accept(TokenType.LEFT_BRACKET);
            case ']' -> addToken.accept(TokenType.RIGHT_BRACKET);
            case ',' -> addToken.accept(TokenType.COMMA);
            case '.' -> addToken.accept(TokenType.DOT);
            case ';' -> addToken.accept(matchNext.apply(';') ? TokenType.DOUBLE_SEMICOLON : TokenType.SINGLE_SEMICOLON);
            case '+' -> addToken.accept(TokenType.PLUS);
            case '-' -> addToken.accept(TokenType.MINUS);
            case '%' -> addToken.accept(TokenType.PERCENT);
            case '*' -> addToken.accept(TokenType.STAR);
            case '/' -> addToken.accept(TokenType.SLASH);
            case '|' -> addToken.accept(matchNext.apply('|') ? TokenType.DOUBLE_BAR : TokenType.SINGLE_BAR);
            case '^' -> addToken.accept(TokenType.CARET);
            case '&' -> addToken.accept(matchNext.apply('&') ? TokenType.DOUBLE_AMPERSAND : TokenType.SINGLE_AMPERSAND);
            case '!' -> addToken.accept(matchNext.apply('=') ? TokenType.BANG_EQUAL : TokenType.BANG);
            case '=' -> addToken.accept(matchNext.apply('=') ? TokenType.DOUBLE_EQUAL : TokenType.SINGLE_EQUAL);
            case '>' -> addToken.accept(matchNext.apply('=') ? TokenType.GREATER_EQUAL : TokenType.GREATER);
            case '<' -> addToken.accept(matchNext.apply('=') ? TokenType.LESS_EQUAL : TokenType.LESS);
            case ' ', '\r', '\t' -> {
            }
            case '"' -> matchString();
            case '\n' -> aLine++;
            default -> {
                if (isDigit.test(nextChar)) {
                    matchNumber();
                } else if (isAlpha.test(nextChar)) {
                    matchIdentifierAndReserved();
                } else {
                    JSurpher.error(aCurrent, "invalid character");
                }

            }
        }
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
     * Reads throught the source code and generates a list of lexical tokens
     * 
     * @return a list of lexical tokens of Surpher
     */
    List<Token> scanTokens() {
        while (!isAtEnd.get()) {
            aStart = aCurrent;
            scanToken();
        }

        aTokenList.add(new Token(TokenType.EOF, "", null, aLine));
        return aTokenList;
    }
}
