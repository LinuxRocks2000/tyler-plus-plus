# Tyler++: The language you wish you didn't code in
Yeah. It's that stupid.

Tyler++ is designed as an esoteric joke language that *works* - it follows the scheme of high level Assembly. It also supports named variables and functions for extra punk'ditude.  

Structure: Every "line" is delimited by `;`, cpp style. The first thing on every line *must* be a "method" - methods do things in memory. Everything after the method is automatically pushed to the stack, to be used as "arguments" - note that this means there is a short form and a long form to every method call.

## Table of methods:

* `pStack`: Print the contents of the stack. It's nicely structured and easy to understand. Usage: `pStack;`
* `push`: Push a value to the stack. Can be any type. Usage: `push thing1 thing2 thing3 ... thingN;`
* `call`: Call a function. Can be a function on stack or an identifier on stack. Usage: `call;`, `call myFunctionName;`.
* `store`: Store a named value, like a conventional variable. Usage: `store storeName;`, `store thing storeName;`. NOTE: With the first mode of usage, this will remove the top value from stack.
* `pGlobals`: Print all named values. Usage: `pGlobals;`.
* `def`: Begin defining a function. Can be named or not. Usage: `def;`, `def functionName;`.
* `endDef`: End a function definition. Usage: `endDef;`.
* `while`: Loop a function on stack until it `break`s, or loop a function by name until it `break`s. Usage: `while;`, `while myFunctionName;`.
* `for`: Loop a function on stack or by name a set number of times. Usage: `for;`, `for 20; // or any other number`.
* `dupl`: Duplicate a value on stack. Usage: `dupl;`.
* `break`: Break a `while` loop. Doesn't work on for loops as of yet. Usage: `break;`.
* `getStored`: Get a stored value, either a function or a variable. Usage: `getStored myFunctionName;`, `getStored myVariableName;`.
* `add`: Add the two top numbers on stack and push the result. Usage: `add 1 2;`, `add 10;`, `add;`.
* `sub`: Subtract the top two numbers on stack and push the result. Usage same as `add`.
* `div`: Divide the top two numbers on stack and push the result. Usage same as `add`.
* `mul`: Multiply the top two numbers on stack and push the result. Usage same as `add`.
* `equals`: Check if the top two values on stack are equal and push the result (a `boolean`). Usage: `equals 10 10;`, `equals 10;`, `equals;`.
* `and`: Binary AND the top two booleans on stack. Usage obvious.
* `or`: Binary OR the top two booleans on stack. Usage obvious.
* `not`: Binary NOT the top boolean on stack. Usage obvious.
* `if`: Run a function if the top boolean is true. Usage: `if callMyFunctionNow;`.
* `pop`: Remove the top value of the stack. Like `store` but without naming it.

## Types:

Types are quite simple. There are 6 types with no composites (classes). They are usually determined dynamically.

* `boolean`: `true` or `false`. C++ `bool`s under the hood.
* `number`: A number with optional decimal. C++ `double` under the hood.
* `string`: Any string. C++ `std::string` under the hood.
* `function`: A function defined at runtime.
* `External function`: A function defined in C++.
* `symbol`: A variable/function name. No spaces!

## Functions:

Functions are pretty cool. They can be either virtual (exist only on the stack) or named (exist as named variables as well). An example of a virtual function is,

```
def; // The next things are stored as a function
	call "Hello, World\n" print; // Don't run until we call it from stack
endDef; // End the function definition
```

Note: functions aren't popped out of stack when you `call`, `if`, `for`, or `while`.  If you want a virtual function out of the way, you have to `pop` it. (Or, preferably, `store` it).

Functions are how you do logic. Such as a for loop is executed like this:

```
def
	call "Hello, World\n" print;
endDef;
for 10; // Prints "Hello World" 10 times
```

A much higher complexity example is the while iterator,

```
store 0 iterator; // Store 0 to iterator
def; // Begin defining a virtual function
    call "Hello, World\n" print;
    getStored iterator; // Push the stored value in "iterator" to the stack
    add 1; // Add 1 to that value in stack
    store iterator; // Store it back to iterator
    getStored iterator; // Get the stored value *back* to stack to use it
    equals 10; // Check if it's equal to 10
    breakIf; // If the boolean on stack is true, this breaks
endDef;

while; // Run the virtual function on stack until it breaks
```

## The C++ Binding:

Check `examples/run.cpp` for well-commented example usage of the binding.

## Oh, yeah, BTW:

IF you want to use it (IDK why you would), just copy `tyler++better.hpp` into your project and use it. Everything is under the namespace `tpp`, but it's *probably* safe to use `using namespace tpp;` because I prefix most names and stuff. There's some standard library EXTERN_ functions; the ones you'll want are `EXTERN_print` and `EXTERN_input`. `EXTERN_printV` prints like `pStack`, and `EXTERN_stringMerge` merges strings.

This is not memory safe. Just gonna be real honest. Nothing is ever deleted so there's huge memory leaks. My next goal is reference counting; for now, just... hope.
