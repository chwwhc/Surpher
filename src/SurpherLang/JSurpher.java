package SurpherLang;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import java.util.Scanner;

public final class JSurpher {
    private static boolean aHadError = false;


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
     * @param pPath path of the script
     * @throws IOException
     */
    private static void runScript(String pPath) throws IOException {
        byte[] script = Files.readAllBytes(Paths.get(pPath));
        run(new String(script, Charset.defaultCharset()));

        // an error occurred during the execution
        if (aHadError) {
            System.exit(65);
        }
    }

    /**
     * Enters the interactive mode
     * @throws IOException
     */
    private static void runPrompt() throws IOException {
        InputStreamReader userInput = new InputStreamReader(System.in);
        BufferedReader buffer = new BufferedReader(userInput);

        // REPL
        while (true) {
            System.out.print("Surpher> ");
            String line = buffer.readLine();
            if (line == null) {
                break;
            }
            run(line);

            // an error occurred during the execution
            if (aHadError) {
                System.exit(65);
            }
        }
    }

    /**
     * Interprets and executes the source code
     * @param pScript the source code 
     */
    private static void run(String pScript) {
        Lexer lexer = new Lexer(pScript);
        List<Token> tokens = lexer.scanTokens();

        for(Token t: tokens){
            System.out.println(t);
        }
    }

    /**
     * Generates an error
     * @param pLine location of the error
     * @param pMessage explanation of the error
     */
    static void error(int pLine, String pMessage) {
        report(pLine, "", pMessage);
    }

    private static void report(int pLine, String pLocation, String pMessage) {
        System.err.println("<line " + pLine + " > error" + pLocation + ": " + pMessage);
        aHadError = true;
    }
}
