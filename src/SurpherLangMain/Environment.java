package SurpherLangMain;

import java.util.HashMap;
import java.util.Map;

class Environment {
    private final Map<String, Object> aValuesTable = new HashMap<>();
    private final Environment aEnclosing;

    Environment() {
        aEnclosing = null;
    }

    Environment(Environment pEnclosing){
        aEnclosing = pEnclosing;
    }

    Object get(Token pToken) {
        if (aValuesTable.containsKey(pToken.getLexeme())) return aValuesTable.get(pToken.getLexeme());
        if (aEnclosing != null) return aEnclosing.get(pToken);
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
        if(aEnclosing != null){
            aEnclosing.update(pToken, pValue);
            return;
        }
        throw new RuntimeError(pToken, "Undefined variable '" + pToken.getLexeme() + "'.");
    }
}
