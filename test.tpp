store 0 iterator;
def;
    call "Hello, World\n" print;
    getStored iterator;
    add 1;
    store iterator;
    getStored iterator;
    equals 10;
    breakIf;
endDef;

while;
