# Surpher

## Introduction
This repo contains my C++ implementation of Robert Nystrom's programming language `Lox` (link to the original book: [Crafting Interpreters](https://craftinginterpreters.com/), 
link to the GitHub repo: [craftinginterpreters](https://github.com/munificent/craftinginterpreters)). I started this project to learn C++ and, more importantly, to have fun! Because this is my first time using C++ in a serious project (sort of?), 
there are bound to be lots of bad codes. Refer
to them at your own risk ;)

## Overview of the Surpher language
Surpher is a dynamically typed, 
interpreted language that supports object-oriented
programming and first-class functions. The current Surpher interpreter
is a tree-walk interpreter implemented in C++17.

Some add-on features to vanilla Lox:
<ul>
<li>Array expression</li>
<li>Rich native functions</li>
<li>Maximum recursion depth of <strong>8192</strong> to avoid stackoverflow error(temporarily disabled)</li>
<li>Function signatures in classes</li>
<li>Static and non-static methods in classes</li>
<li>Namespaces to avoid name collision</li>
</ul>

## How to use
Load CMake project:
```
user@name:~/.../Surpher$ cmake [options] -S <path-to-source> -B <path-to-build>
```
Compile the project:
```
user@name:~/.../Surpher$ make
```
Run the following command to open the REPL:
```
./Surpher
```
run the following command to execute a script:
```
./Surpher [path to script]
```
In the REPL session,
run the following command to exit:
```
!quit
```

## Full grammar specification
``` 
program        → declaration* EOF ;

declaration    → "fixed"? ( classDecl | funDecl | varDecl | namespaceDecl )
               | statement ;

classDecl      → "class" IDENTIFIER ( "<" IDENTIFIER )?
                 "{" function* "}" ;
funDecl        → "fun" function ;
varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;
namespaceDecl  → "namespace" IDENTIFIER block ;

statement      → exprStmt
               | forStmt
               | ifStmt
               | printStmt
               | returnStmt
               | whileStmt
               | continueStmt
               | breakStmt
               | importStmt
               | haltStmt
               | block ;

exprStmt       → expression ";" ;
forStmt        → "for" "(" ( varDecl | exprStmt | ";" )
                           expression? ";"
                           expression? ")" statement ;
ifStmt         → "if" "(" expression ")" statement
                 ( "else" statement )? ;
printStmt      → "print" expression ";" ;
returnStmt     → "return" expression? ";" ;
whileStmt      → "while" "(" expression ")" statement ;
continueStmt   → "continue" ";" ;
breakStmt      → "break" ";" ;
importStmt     → "import" expression ;
haltStmt       → "halt" expression ;
block          → "{" declaration* "}" ;

expression     → assignment ;
assignment     → (( call "." )? IDENTIFIER | access) "=" assignment
               | array ;
array          → "[" array? "]" | "[" "alloc" ":" expression "]" | ternary ;
ternary        → logical_or "?" ternary ":" ternary ;
logic_or       → logic_and ( "or" logic_and )* ;
logic_and      → bit_wise_or ( "and" bit_wise_or )* ;
bit_wise_or    → bit_wise_xor ( "|" bit_wise_xor )* ;
bit_wise_xor   → bit_wise_and ( "^" bit_wise_and )* ;
bit_wise_and   → equality ( "&" equality )* ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" | "%" ) unary )* ;
unary          → ( "!" | "-" ) unary | access ;
access         → "@" access "->" access | call ;
call           → primary ( "(" arguments? ")" | "." IDENTIFIER )* ;
primary        → "true" | "false" | "nil" | "this"
               | NUMBER | STRING | IDENTIFIER | "(" expression ")"
               | "super" "." IDENTIFIER | "\" IDENTIFIER* "->" expression ;
function       → "sig"? "class"? IDENTIFIER "(" parameters? ")" block ;
parameters     → IDENTIFIER ( "," IDENTIFIER )* ;
arguments      → expression ( "," expression )* ;

NUMBER         → DIGIT+ ( "." DIGIT+ )? ;
STRING         → "\"" <any char except "\"">* "\"" ;
IDENTIFIER     → ALPHA ( ALPHA | DIGIT )* ;
ALPHA          → "a" ... "z" | "A" ... "Z" | "_" ;
DIGIT          → "0" ... "9" ;
```

## Example programs
### 0. Don Knuth's Man or Boy test 
```
var x = \x -> (\ -> x);

fun A(k, x1, x2, x3, x4, x5) {
    fun B() {
        k = k - 1;
        return A(k, B, x1, x2, x3, x4); //decrement k before use
    }
    return k > 0 ? B() : x4() + x5();
}

print A(10, x(1), x(-1), x(-1), x(1), x(0)); // result: -67
```

### 1. Implementing Y-combinator
``` 
// Y-combinator as a lambda expression
var y_combinator = \f -> (\x -> x(x))(\x -> f(\y -> x(x)(y)));

// fibonacci function as a lambda expression
var fibonacci = \f -> (\x -> (x < 2 ? x : f(x - 1) + f(x - 2)));

// compute fibonacci(20) via Y-combinator
print y_combinator(fibonacci)(20);
```

### 2. Class inheritance
```  
// this example is taken from the original book

// base class
class Doughnut {
  cook() {
    print "Fry until golden brown.";
  }
}

// derived class
class BostonCream < Doughnut {
  cook() {
    super.cook();
    print "Pipe full of custard and coat with chocolate.";
  }
}

BostonCream().cook();
```

### 3. Rendering a Mandelbrot set
Please see
```
./example_programs/fractal_renderer/mandelbrot_set_renderer.sfr
```
<img src=pics/mandelbrot.jpg>

## References
<ul>
<li><a href="https://craftinginterpreters.com/">Crafting Interpreters</a></li>
</ul>


