package SurpherLangMain;

import java.util.HashMap;
import java.util.Map;
import java.util.function.BiConsumer;

public class Environment {
    private final Map<String, Object> aValuesTable = new HashMap<>();
    private static Environment aEnvironment = new Environment();

    private Environment() {
    }

    static Environment getEnvironment() {
        return aEnvironment;
    }

    Object get(Token pToken) {
        if (aValuesTable.containsKey(pToken.getLexeme()))
            return aValuesTable.get(pToken.getLexeme());
        throw new RuntimeError(pToken, "Undefined variable '" + pToken.getLexeme() + "'.");
    }

    void define(String pKey, Object pValue) {
        aValuesTable.put(pKey, pValue);
    }

    void update(Token pToken, Object pValue) {
        if (aValuesTable.containsKey(pToken.getLexeme())) {
            aValuesTable.put(pToken.getLexeme(), pValue);
            return;
        }
        throw new RuntimeError(pToken, "Undefined variable '" + pToken.getLexeme() + "'.");
    }
}
