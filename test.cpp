#include "tyler++better.hpp"


using namespace tpp;


int main(int argc, char** argv){
    TPPInterpreter interpreter;
    interpreter.registerFunction(&EXTERN_print, "print");
    interpreter.registerFunction(&EXTERN_printV, "printV");
    interpreter.registerFunction(&EXTERN_input, "input");
    interpreter.registerFunction(&EXTERN_stringMerge, "stringMerge");
    std::ifstream t("test.tpp");
    std::stringstream buffer;
    buffer << t.rdbuf();
    interpreter.interpret(buffer.str());
    TPP_GARBAGECOLLECT();
    return 0;
}
