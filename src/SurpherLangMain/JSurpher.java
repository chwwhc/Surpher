package SurpherLangMain;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import SurpherLangMain.Stmt.Expression;
import SurpherLangMain.Token.TokenType;

public final class JSurpher {
    private static boolean aHadError = false;
    private static boolean aHadRuntimeError = false;
    private static final Interpreter aInterpreter = new Interpreter();

    public static void main(String[] args) throws IOException {
        if (args.length > 1) {
            // invalid usage
            System.out.println("Usage: jsurpher [script]");
            System.exit(64);
        } else if (args.length == 1) {
            // batch mode
            runScript(args[0]);
        } else {
            // interactive mode
            runPrompt();
        }
    }

    /**
     * Executes a script in batch mode
     * 
     * @param pPath path of the script
     * @throws IOException
     */
    private static void runScript(String pPath) throws IOException {
        byte[] script = Files.readAllBytes(Paths.get(pPath));
        run(new String(script, Charset.defaultCharset()));

        // an error occurred during the execution
        if (aHadError)
            System.exit(65);
        if (aHadRuntimeError)
            System.exit(70);

    }

    /**
     * Enters the interactive mode
     * 
     * @throws IOException
     */
    private static void runPrompt() throws IOException {
        BufferedReader buffer = new BufferedReader(new InputStreamReader(System.in));

        // REPL
        while (true) {
            System.out.print("Surpher> ");
            String line = buffer.readLine();
            if (line.equals("quit!!!")) {
                System.out.println("closing the prompt...");
                System.exit(0);
            }
            run(line);

            if (aHadError)
                System.exit(65);
        }
    }

    /**
     * Interprets and executes the source code
     * 
     * @param pScript the source code
     */
    private static void run(String pScript) {
        Lexer lexer = new Lexer(pScript);
        List<Token> tokens = lexer.scanTokens();
        Parser parser = new Parser(tokens);

        if (aHadError)
            return;
        aInterpreter.interpret(parser.parse());
    }

    /**
     * Generates an error
     * 
     * @param pLine    location of the error
     * @param pMessage explanation of the error
     */
    static void error(int pLine, String pMessage) {
        report(pLine, "", pMessage);
    }

    static void error(Token pToken, String pMessage) {
        if (pToken.getType() == TokenType.EOF) {
            report(pToken.getLine(), " at end", pMessage);
        } else {
            report(pToken.getLine(), " at '" + pToken.getLexeme() + "'", pMessage);
        }
    }

    static void runtimeError(RuntimeError pError) {
        System.err.println(pError.getMessage() + "\n[line " + pError.getToken().getLine() + "]");
        aHadRuntimeError = true;
    }

    private static void report(int pLine, String pLocation, String pMessage) {
        System.err.println("<line " + pLine + " > error" + pLocation + ": " + pMessage);
        aHadError = true;
    }
}
