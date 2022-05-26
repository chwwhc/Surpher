package SurpherLangMain;

import SurpherLangMain.Token.TokenType;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.Predicate;
import java.util.function.Supplier;

/**
 * Lexer for the Surpher language
 */
class Lexer {
    private static final Map<String, TokenType> aKeywords = new HashMap<>();

    /*
      Automatically adds the reserved keywords
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
        aKeywords.put("print", TokenType.PRINT);
    }

    private final List<Token> aTokenList = new LinkedList<>();
    private final Predicate<Character> isDigit = pChar -> pChar >= '0' && pChar <= '9';
    private final Predicate<Character> isAlpha = pChar -> (pChar >= 'a' && pChar <= 'z') || (pChar >= 'A' && pChar <= 'Z')
            || pChar == '_';
    private final Predicate<Character> isAlphaNumeric = isAlpha.or(isDigit);
    private String aSource;
    private int aStart = 0;
    private int aCurrent = 0;
    private final Supplier<Character> anyChar = () -> aSource.charAt(aCurrent++);
    private final Supplier<Boolean> isAtEnd = () -> aCurrent >= aSource.length();
    private final Function<Integer, Character> lookAHead = pOffset -> {
        if (isAtEnd.get() || aCurrent + pOffset >= aSource.length()) {
            return '\0';
        } else {
            return aSource.charAt(aCurrent + pOffset);
        }
    };
    private final Function<Character, Boolean> matchNext = pExpectedChar -> {
        if (isAtEnd.get() || aSource.charAt(aCurrent) != pExpectedChar) {
            return false;
        }
        aCurrent++;
        return true;
    };
    private int aLine = 1;
    private final Consumer<TokenType> addType = pType -> addToken(pType, null);

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
            // case 1: float number
            anyChar.get();
        }  // case 2: int number

        while (isDigit.test(lookAHead.apply(0))) {
            anyChar.get();
        }
        addToken(TokenType.NUMBER, Double.parseDouble(aSource.substring(aStart, aCurrent)));

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
        addType.accept(type);
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
        // keep track of the balance
        int flag = 1;

        while (flag != 0 && !isAtEnd.get()) {
            // error: unterminated comment
            if (isAtEnd.get()) {
                JSurpher.error(aLine, "unterminated comment");
                return;
            }

            // update the stack accordingly
            if (matchNext.apply('(') && matchNext.apply('*')) {
                flag++;
            } else if (matchNext.apply('*') && matchNext.apply(')')) {
                flag--;
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
                    addType.accept(TokenType.LEFT_PAREN);
                }
            }
            case ')' -> addType.accept(TokenType.RIGHT_PAREN);
            case '{' -> addType.accept(TokenType.LEFT_BRACE);
            case '}' -> addType.accept(TokenType.RIGHT_BRACE);
            case '[' -> addType.accept(TokenType.LEFT_BRACKET);
            case ']' -> addType.accept(TokenType.RIGHT_BRACKET);
            case ',' -> addType.accept(TokenType.COMMA);
            case '.' -> addType.accept(TokenType.DOT);
            case ';' -> addType.accept(matchNext.apply(';') ? TokenType.DOUBLE_SEMICOLON : TokenType.SINGLE_SEMICOLON);
            case '+' -> addType.accept(matchNext.apply('+') ? TokenType.DOUBLE_PLUS : TokenType.SINGLE_PLUS);
            case '-' -> addType.accept(TokenType.MINUS);
            case '%' -> addType.accept(TokenType.PERCENT);
            case '*' -> addType.accept(TokenType.STAR);
            case '/' -> addType.accept(TokenType.SLASH);
            case '|' -> addType.accept(matchNext.apply('|') ? TokenType.DOUBLE_BAR : TokenType.SINGLE_BAR);
            case '^' -> addType.accept(TokenType.CARET);
            case '&' -> addType.accept(matchNext.apply('&') ? TokenType.DOUBLE_AMPERSAND : TokenType.SINGLE_AMPERSAND);
            case '!' -> addType.accept(matchNext.apply('=') ? TokenType.BANG_EQUAL : TokenType.BANG);
            case '=' -> addType.accept(matchNext.apply('=') ? TokenType.DOUBLE_EQUAL : TokenType.SINGLE_EQUAL);
            case '>' -> addType.accept(matchNext.apply('=') ? TokenType.GREATER_EQUAL : TokenType.GREATER);
            case '<' -> addType.accept(matchNext.apply('=') ? TokenType.LESS_EQUAL : TokenType.LESS);
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
        aTokenList.add(new Token(pType, aSource.substring(aStart, aCurrent), pLiteral, aLine));
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
