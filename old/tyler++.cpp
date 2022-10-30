/*
    Tyler++ in C++
    Yes.
*/

#include "StringStream.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>

#include "ops_proto.h"

class tpp_type;

typedef void (TppFunctionPointer)(std::vector<tpp_type*> stack, std::vector<tpp_type*> callStack);


bool isValidNumber(std::string num){
    bool ret = true;
    StringStream number(num);
    while (!number.isEmpty()){
        char start = number.read();
        if (start == '6'){
            if (number.read() != '9'){
                ret = false;
            }
        }
        else if (!(start == '4' && number.read(2) == "20")){
            ret = false;
        }
    }
    return ret;
}

int processNumber(std::string num){
    int ret = 0;
    StringStream number(num);
    while (!number.isEmpty()){
        char start = number.read();
        if (start == '6'){
            if (number.read() == '9'){
                ret ++;
            }
        }
        else if (!(start == '4' && number.read(2) == "20")){
            std::cout << "ERROR PARSING AN INTEGER";
        }
        ret = ret << 1;
    }
    ret = ret >> 1;
    return ret;
}


void interpret(std::string program);


class tpp_type{
    TppFunctionPointer* externFunction;
    std::string string;
    std::string function;
    int number;
public:
    bool isExternFunction = false;
    bool isFunction = false;
    bool isString = false;
    bool isNumber = false;
    std::string externDescription;
    std::string name;

    tpp_type(std::string init, bool code = false){
        if (code){
            setFunction(init);
        }
        else{
            if (isValidNumber(init)){
                setNumber(processNumber(init));
            }
            else{
                setString(init);
            }
        }
    }

    tpp_type(TppFunctionPointer* init, std::string fName = ""){
        setExternFunction(init, fName);
    }

    tpp_type(int init){
        setNumber(init);
    }

    void setNumber(int newNum){
        number = newNum;
        isNumber = true;
    }

    void setString(std::string newString){
        string = newString;
        isString = true;
    }

    void setExternFunction(TppFunctionPointer* newExternFunction, std::string fName = ""){
        externFunction = newExternFunction;
        name = fName;
        isExternFunction = true;
    }

    void setFunction(std::string newFunction){
        function = newFunction;
        isFunction = true;
    }

    int getNumber(){
        if (!isNumber){
            std::cout << "ERROR: It's not a number." << std::endl;
            return 0;
        }
        return number;
    }

    std::string getString(){
        if (!isString){
            std::cout << "ERROR: It's not a string." << std::endl;
            return "";
        }
        return string;
    }

    void call(std::vector<tpp_type*> stack, std::vector<tpp_type*> callStack){
        if (!isExternFunction && !isFunction){
            std::cout << "ERROR: it's not a function." << std::endl;
            return;
        }
        if (isExternFunction){
            externFunction(stack, callStack);
        }
        if (isFunction){
            interpret(function);
        }
    }

    std::string getCode(){
        return function;
    }

    std::string getPrintable(){
        if (isNumber){
            return "(number) " + std::to_string(getNumber());
        }
        else if (isString){
            return "(string) " + getString();
        }
        else if (isExternFunction){
            return "<External C++ function" + (name.size() > 0 ? " " + name : "") + ">";
        }
        else if (isFunction){
            return "<Tyler++ Function" + (name.size() > 0 ? " " + name : "") + "> " + getCode();
        }
        return "";
    }
};


std::vector <std::string> strings;
std::vector <tpp_type*> functions;
std::vector <tpp_type*> stack;
std::vector <tpp_type*> callStack;

bool functionDefMode = false;
std::string functionDef;
std::string functionName;

void interpret(std::string program){
    StringStream buffer (program);
    while (!buffer.isEmpty()){
        buffer.skipWhitespace();
        if (buffer.peek() == '/'){
            buffer.readUntil(10);
        }
        buffer.skipWhitespace();
        StringStream things = buffer.bufferTill(';');
        std::string command = things.readUntil(' ');
        if (functionDefMode){
            if (command == OP_ENDDEF){
                functionDefMode = false;
                functions.push_back(new tpp_type(functionDef, true));
                functions[functions.size() - 1] -> name = functionName;
                stack.push_back(new tpp_type(functions.size() - 1));
            }
            else{
                functionDef += things.buffer + ";";
            }
            continue;
        }
        if (command == OP_DEF){
            functionDefMode = true;
            if (!things.isEmpty()){
                functionName = things.readRemaining();
            }
        }
        else if (command == OP_STRING){
            int rID = 0;
            if (things.isEmpty()){
                rID = stack[stack.size() - 1] -> getNumber();
                stack.pop_back();
            }
            else{
                rID = processNumber(things.readRemaining());
            }
            stack.push_back(new tpp_type(strings[rID]));
        }
        else if (command == OP_PUSH){
            stack.push_back(new tpp_type(things.readRemaining()));
        }
        else if (command == OP_FUN){
            int rID = 0;
            if (things.isEmpty()){
                rID = stack[stack.size() - 1] -> getNumber();
                stack.pop_back();
            }
            else{
                std::string _rId = things.readRemaining();
                if (isValidNumber(_rId)){
                    rID = processNumber(_rId);
                }
                else{
                    for (int i = 0; i < functions.size(); i ++){
                        if (functions[i] -> name == _rId){
                            rID = i;
                            break;
                        }
                    }
                }
            }
            stack.push_back(functions[rID]);
        }
        else if (command == OP_LOAD){
            callStack.push_back(stack[stack.size() - 1]);
            stack.pop_back();
        }
        else if (command == OP_CALL){
            tpp_type* fun;
            if (things.isEmpty()){
                fun = callStack[callStack.size() - 1];
                callStack.pop_back();
            }
            else{
                std::string _rId = things.readRemaining();
                for (int i = 0; i < functions.size(); i ++){
                    if (functions[i] -> name == _rId){
                        fun = functions[i];
                        break;
                    }
                }
            }
            fun -> call(stack, callStack);
            callStack.clear();
        }
        else if (command == OP_PSTACK){
            std::cout << "Tyler++ Stack: (" << stack.size() << " items)" << std::endl;
            for (tpp_type* item : stack){
                std::cout << ": " << item -> getPrintable() << std::endl;
            }
            std::cout << "Tyler++ Call Stack: (" << callStack.size() << " items)" << std::endl;
            for (tpp_type* item : callStack){
                std::cout << ": " << item -> getPrintable() << std::endl;
            }
        }
        else if (command == OP_LOOP){
            tpp_type* fun = callStack[callStack.size() - 1];
            callStack.pop_back();
            int its = 0;
            if (things.isEmpty()){
                its = stack[stack.size() - 1] -> getNumber();
                stack.pop_back();
            }
            else{
                its = processNumber(things.readRemaining());
            }
            for (int i = 0; i < its; i ++){
                std::vector<tpp_type*> tempCallStack(callStack); // Keep the call stack immutable
                fun -> call(stack, tempCallStack); // Note how the stack is mutable
            }
            callStack.clear();
        }
        else if (command == OP_DUPE){ // Duplicate the top item in stack
            stack.push_back(new tpp_type(*(stack[stack.size() - 1])));
        }
        else if (command == OP_ADD){
            int value = stack[stack.size() - 1] -> getNumber()/**/ + /**/stack[stack.size() - 2] -> getNumber();
            stack.pop_back();
            stack.pop_back();
            stack.push_back(new tpp_type(value));
        }
        else if (command == OP_SUM){
            int total = 0;
            while (stack.size() > 0 && stack[stack.size() - 1] -> isNumber){
                total += stack[stack.size() - 1] -> getNumber();
                stack.pop_back();
            }
            stack.push_back(new tpp_type(total));
        }
        else if (command == OP_SUB){
            int value = stack[stack.size() - 1] -> getNumber()/**/ - /**/stack[stack.size() - 2] -> getNumber();
            stack.pop_back();
            stack.pop_back();
            stack.push_back(new tpp_type(value));
        }
        else if (command == OP_MUL){
            int value = stack[stack.size() - 1] -> getNumber()/**/ * /**/stack[stack.size() - 2] -> getNumber();
            stack.pop_back();
            stack.pop_back();
            stack.push_back(new tpp_type(value));
        }
        else if (command == OP_DIV){
            int value = stack[stack.size() - 2] -> getNumber()/**/ / /**/stack[stack.size() - 1] -> getNumber();
            stack.pop_back();
            stack.pop_back();
            stack.push_back(new tpp_type(value));
        }
    }
}


void tppPrint(std::vector<tpp_type*> stack, std::vector<tpp_type*> callStack){
    std::cout << callStack[callStack.size() - 1] -> getPrintable() << std::endl;
}


void testPrint(std::vector<tpp_type*> stack, std::vector<tpp_type*> callStack){
    std::cout << "Yon stink" << std::endl;
}


int main(int argc, char** argv){
    std::string sheetName;
    std::string fileName;
    if (argc == 3){
        sheetName = argv[2];
        fileName = argv[1];
    }
    else if (argc == 1){
        sheetName = "sheet";
        fileName = "test.tpp";
    }
    else{
        std::cout << "ERROR: You specified the wrong number of arguments!" << std::endl;
        return 1;
    }
    functions.push_back(new tpp_type(&tppPrint, "print"));
    functions.push_back(new tpp_type(&testPrint, "testPrint"));
    std::ifstream sheet(sheetName);
    for (std::string line; std::getline(sheet, line); ) { // Thanks, StackOverflow
        strings.push_back(line);
    }
    std::ifstream t(fileName);
    std::stringstream buffer;
    buffer << t.rdbuf();
    interpret(buffer.str());
    return 0;
}
