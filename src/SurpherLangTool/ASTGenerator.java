package SurpherLangTool;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.List;

public class ASTGenerator {
    public static void main(String[] args) throws IOException {
        if (args.length != 1) {
            System.err.println("Usage: generate_ast <output directory>");
            System.exit(64);
        }
        String outputDir = args[0];
        defineAST(outputDir, "Expr", Arrays.asList(
                "Binary   : Expr left, Token operator, Expr right",
                "Grouping : Expr expression",
                "Literal  : Object value",
                "Unary    : Token operator, Expr right"));
    }

    /**
     * Auto-generates an AST
     * 
     * @param pOutputDir target path
     * @param pBaseName  base name of the expression
     * @param pTypes     types in the productions
     * @throws IOException
     */
    private static void defineAST(String pOutputDir, String pBaseName, List<String> pTypes) throws IOException {
        String path = pOutputDir + "/" + pBaseName + ".java";
        PrintWriter writer = new PrintWriter(path, "UTF-8");

        writer.println("package SurpherLangMain;");
        writer.println();
        writer.println("import java.util.List;");
        writer.println();
        writer.println("/**");
        writer.println(" * Auto-generated AST file");
        writer.println(" */");
        writer.println("abstract class " + pBaseName + " {");

        // abstract accept method
        writer.println("  abstract <R> R accept(ExprVisitor<R> pVisitor);");
        writer.println();

        defineVisitor(writer, pBaseName, pTypes);

        // the AST classes
        for (String t : pTypes) {
            String className = t.split(":")[0].trim();
            String fields = t.split(":")[1].trim();
            defineType(writer, pBaseName, className, fields);
        }

        writer.println("}");
        writer.close();
    }

    /**
     * Auto-generates a type
     * 
     * @param pWriter
     * @param pBaseName  base name of the expression
     * @param pClassName
     * @param pFieldList list of fields
     */
    private static void defineType(PrintWriter pWriter, String pBaseName, String pClassName, String pFieldList) {
        // formats the field names and parameter names
        List<String> fields = Arrays.asList(pFieldList.split(", "));
        List<String> fieldNames = fields.stream()
                .map((x -> "a" + (x.split(" ")[1]).substring(0, 1).toUpperCase() + (x.split(" ")[1]).substring(1)))
                .toList();
        List<String> paramNames = fields.stream()
                .map((x -> "p" + (x.split(" ")[1]).substring(0, 1).toUpperCase() + (x.split(" ")[1]).substring(1)))
                .toList();
        List<String> typeNames = fields.stream().map((x -> x.split(" ")[0])).toList();

        // class signature
        pWriter.println("   static class " + pClassName + " extends " + pBaseName + " {");

        // fields
        for (int i = 0; i < fieldNames.size(); i++) {
            pWriter.println("       final " + typeNames.get(i) + " " + fieldNames.get(i) + ";");
        }
        pWriter.println();

        // constructor
        pWriter.print("     " + pClassName + "(");
        for (int i = 0; i < fieldNames.size() - 1; i++) {
            pWriter.print(typeNames.get(i) + " " + paramNames.get(i) + ", ");
        }
        pWriter.println(typeNames.get(typeNames.size() - 1) + " " + paramNames.get(paramNames.size() - 1) + ") {");

        // store parameters in fields
        for (int i = 0; i < fieldNames.size(); i++) {
            pWriter.println("       " + fieldNames.get(i) + " = " + paramNames.get(i) + ";");
        }

        pWriter.println("       }");

        // visitor implementation
        pWriter.println();
        pWriter.println("    @Override");
        pWriter.println("    <R> R accept(ExprVisitor<R> pVisitor) {");
        pWriter.println("      return pVisitor.visit" +
                pClassName + pBaseName + "(this);");
        pWriter.println("   }");

        pWriter.println("   }");
        pWriter.println();
    }

    /**
     * Auto-generates a visitor method
     * 
     * @param pWriter
     * @param pBaseName base name of the expression
     * @param pTypes
     */
    private static void defineVisitor(PrintWriter pWriter, String pBaseName, List<String> pTypes) {
        pWriter.println("   interface ExprVisitor<R> {");

        for (String t : pTypes) {
            String typeName = t.split(":")[0].trim();
            pWriter.println(
                    "       R visit" + typeName + pBaseName + "(" + typeName + " p"
                            + pBaseName.substring(0, 1).toUpperCase() + pBaseName.substring(1).toLowerCase() + ");");
        }

        pWriter.println("       }");
        pWriter.println();
    }
}
