package SurpherLangTool;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.function.BiConsumer;

public class ASTGenerator {
        private static <T, U> void zip(Collection<T> pCollectionA, Collection<U> pCollectionB,
                        BiConsumer<T, U> pConsumerFun) {
                Iterator<T> iterA = pCollectionA.iterator();
                Iterator<U> iterB = pCollectionB.iterator();
                while (iterA.hasNext() && iterB.hasNext()) {
                        pConsumerFun.accept(iterA.next(), iterB.next());
                }
        }

        public static void main(String[] args) throws IOException {
                /*
                 * if (args.length != 1) {
                 * System.err.println("Usage: generate_ast <output directory>");
                 * System.exit(64);
                 * }
                 */
                // String outputDir = args[0];
                System.out.println();
                System.out.println(System.getProperty("user.dir"));
                System.out.println();
                BufferedReader buffer = new BufferedReader(new InputStreamReader(System.in));
                String outputDir = buffer.readLine();
                defineAST(outputDir, "Expr", Arrays.asList(
                                "Binary   : Expr left, Token operator, Expr right",
                                "Grouping : Expr expression",
                                "Literal  : Object value",
                                "Unary    : Token operator, Expr right",
                                "Variable : Token name",
                                "Assign : Token name, Expr value"));
                defineAST(outputDir, "Stmt", Arrays.asList("Expression : Expr expression",
                                "Var : Token name, Expr initializer",
                                "Print : Expr expression"));
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
                writer.println("  abstract <R> R accept(Visitor<R> pVisitor);");
                writer.println();

                defineVisitor(writer, pBaseName, pTypes);

                // the AST classes
                pTypes.stream().map(t -> new String[] { t.split(":")[0].trim(), t.split(":")[1].trim() })
                                .forEach(x -> defineType(writer, pBaseName, x[0], x[1]));

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
                                .map((fieldName -> "a" + (fieldName.split(" ")[1]).substring(0, 1).toUpperCase()
                                                + (fieldName.split(" ")[1]).substring(1)))
                                .toList();
                List<String> paramNames = fields.stream()
                                .map((field -> "p" + (field.split(" ")[1]).substring(0, 1).toUpperCase()
                                                + (field.split(" ")[1]).substring(1)))
                                .toList();
                List<String> typeNames = fields.stream().map((typeName -> typeName.split(" ")[0])).toList();

                // class signature
                pWriter.println("   static class " + pClassName + " extends " + pBaseName + " {");

                // fields
                zip(typeNames, fieldNames,
                                ((typeName, fieldName) -> pWriter
                                                .println("      private final " + typeName + " " + fieldName + ";")));
                pWriter.println();

                // constructor
                pWriter.print("     " + pClassName + "(");
                zip(typeNames.subList(0, typeNames.size() - 1), paramNames.subList(0, paramNames.size() - 1),
                                ((typeName, paramName) -> pWriter.print(typeName + " " + paramName + ", ")));
                pWriter.println(typeNames.get(typeNames.size() - 1) + " " + paramNames.get(paramNames.size() - 1)
                                + ") {");

                // store parameters in fields
                zip(fieldNames, paramNames,
                                ((fieldName, paramName) -> pWriter
                                                .println("       " + fieldName + " = " + paramName + ";")));

                pWriter.println("       }");

                // visitor implementation
                pWriter.println();
                pWriter.println("    @Override");
                pWriter.println("    <R> R accept(Visitor<R> pVisitor) {");
                pWriter.println("      return pVisitor.visit" +
                                pClassName + pBaseName + "(this);");
                pWriter.println("   }");
                pWriter.println();

                // getters
                zip(typeNames, fieldNames, (typeName, fieldName) -> {
                        pWriter.print("    " + typeName);
                        pWriter.println(" get" + fieldName.substring(1) + "() {");
                        pWriter.println("      return " + fieldName + ";");
                        pWriter.println("   }");
                        pWriter.println();
                });

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
                pWriter.println("   interface Visitor<R> {");

                pTypes.stream().map(type -> type.split(":")[0].trim()).forEach(type -> pWriter.println(
                                "       R visit" + type + pBaseName + "(" + type + " p"
                                                + pBaseName.substring(0, 1).toUpperCase()
                                                + pBaseName.substring(1).toLowerCase() + ");"));

                pWriter.println("       }");
                pWriter.println();
        }
}
