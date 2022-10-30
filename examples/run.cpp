#include "../tyler++better.hpp"

using namespace tpp;

void EXTERN_bindingDemonstration(TPP_FUNCTION_ARGS){ // The macro provides TPPInterpreter* interpreter.
    std::cout << "Hello, Binding" << std::endl; // Prints to console
    interpreter -> pushStack(MAKE_TPP_STRING("Hello again\n")); // Push "Hello, again\n" to the stack. Note that it has to be encapsulated in a MAKE_TPP_STRING - this converts it to a tpp_type*.
    interpreter -> call("print"); // Call a function by name, in this case print.
    // The last two lines are eqivalent to `push "Hello again\n"; call print;`, or just `call "Hello again\n" print;` because of the implicit push.
}

void EXTERN_addThings(TPP_FUNCTION_ARGS){
    std::cout << "Adding things!" << std::endl;
    interpreter -> pushStack(MAKE_TPP_NUMBER(3.1415));
    interpreter -> pushStack(MAKE_TPP_BOOLEAN(true));
    interpreter -> pushStack(MAKE_TPP_STRING("A string!"));
    interpreter -> pushStack(MAKE_TPP_SYMBOL("function_name"));
}

int main(){
    std::cout << "File to run: ";
    std::string fileName;
    std::cin >> fileName;
    TPPInterpreter interpreter;
    interpreter.registerFunction(&EXTERN_print, "print"); // Standard library of sorts
    interpreter.registerFunction(&EXTERN_printV, "printV");
    interpreter.registerFunction(&EXTERN_input, "input");
    interpreter.registerFunction(&EXTERN_stringMerge, "stringMerge");

    interpreter.registerFunction(&EXTERN_bindingDemonstration, "bindingDemo"); // Register it. Note the & - this is a function pointer.
    interpreter.registerFunction(&EXTERN_addThings, "addThings");
    std::ifstream t(fileName);
    std::stringstream buffer;
    buffer << t.rdbuf();
    interpreter.interpret(buffer.str());
    return 0;
}

/*
    To create your own binding:
 * First create the binding function. I always prefix with EXTERN_ but you don't have to. It should return void (nuttin') and take TPP_FUNCTION_ARGS as arguments.
 * Next, run interpreter.registerFunction(&your_function, "your_function_name").
 * It's that easy!
 Note: member functions aren't allowed. I'm mean that way. Since T++ is probably gonna be used in FRC programming, this is a hurdle to work around. Muwahahahaha.
*/
