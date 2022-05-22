package SurpherLangMain;

public class RuntimeError extends RuntimeException {
    private final Token aToken;

    RuntimeError(Token pToken, String pMessage) {
        super(pMessage);
        aToken = pToken;
    }

    Token getToken(){
        return aToken;
    }
}
