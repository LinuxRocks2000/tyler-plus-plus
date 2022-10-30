## Initial draft of tyler++
## str: put a string from the sheet on the memory stack
## fun: put a function on the memory stack
## load: load the next thing from the memory stack to the call stack
## call: call the highest loaded function

strings = ["Hello, world"]

## functions
def tppPrint(stack):
    print(*stack)

functions = [
    {"type": "external", "fun": tppPrint}
]


isFunction = False
curFunction = []
def interpretLine(line, stack, callStack, commands = False):
    global isFunction
    global curFunction
    if not commands:
        commands = line.split(" ")
    if isFunction:
        if commands[0] == "endFun":
            functions.append({"type": "internal", "fun": curFunction})
            stack.append(len(functions) - 1)
            curFunction = []
            isFunction = False
        else:
            curFunction.append(commands)
        return
    if commands[0] == "str":
        num = 0
        if len(commands) > 1:
            num = int(commands[1])
        else:
            num = int(stack.pop())
        stack.append(strings[num])
    elif commands[0] == "push":
        stack.append(*commands[1:])
    elif commands[0] == "fun":
        num = 0
        if len(commands) > 1:
            num = int(commands[1])
        else:
            num = int(stack.pop())
        stack.append(functions[num])
    elif commands[0] == "load":
        callStack.append(stack.pop(0))
    elif commands[0] == "call":
        fun = callStack.pop()
        if type(fun) != dict:
            print("ERROR: The top value of the call stack is not a function. U BAD!")
            return
        if fun["type"] == "external":
            fun["fun"](callStack)
        elif fun["type"] == "internal":
            for routine in fun["fun"]:
                interpretLine("", stack, callStack, routine)
        callStack.clear()
    elif commands[0] == "def":
        isFunction = True

def interpret(program):
    curLine = ""
    phase = 0
    stack = []
    callStack = []
    comment = False
    for x in program:
        if x == "/":
            comment = True
        if comment:
            if x == "\n":
                comment = False
            continue
        if phase == 0:
            if x != " ":
                curLine += x
                phase = 1
        elif phase == 1:
            if x != ";":
                curLine += x
            else:
                curLine = curLine.replace("\n", "")
                interpretLine(curLine, stack, callStack)
                curLine = ""
                phase = 0
    if curLine != "\n":
        print("ERROR: your syntax SUX")

def runFile(filename, valuesname):
    global strings
    strings = open(valuesname).read().split("\n")
    interpret(open(filename).read())

runFile("test.tpp", "sheet")
