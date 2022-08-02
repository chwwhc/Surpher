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
<li>Runtime optimization for recursive functions by caching</li>
<li>Maximum recursion depth of 4096 to avoid stackoverflow error</li>
<li>Virtual methods in classes</li>
<li>Static and non-static methods in classes</li>
<li>Namespaces to avoid name collision</li>
</ul>

## How to use
Run the following command to open the REPL:
```
./Surpher
```
In the REPL session, 
run the following command to execute a script that's in the same directory:
```
import "some_script.sfr";
```
In the REPL session,
run the following command to exit:
```
!quit
```

## Full grammar specification
``` 
program        → declaration* EOF ;

declaration    → "const"? ( classDecl | funDecl | varDecl | namespaceDecl )
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
importStmt     → "import" STRING ;
block          → "{" declaration* "}" ;

expression     → assignment ;

assignment     → ( call "." )? IDENTIFIER "=" assignment
               | ternary ;
             
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

unary          → ( "!" | "-" ) unary | call ;
call           → primary ( "(" arguments? ")" | "." IDENTIFIER )* ;
primary        → "true" | "false" | "nil" | "this"
               | NUMBER | STRING | IDENTIFIER | "(" expression ")"
               | "super" "." IDENTIFIER | "\" IDENTIFIER* "->" expression ;
               
function       → "virtual"? "class"? IDENTIFIER "(" parameters? ")" block ;
parameters     → IDENTIFIER ( "," IDENTIFIER )* ;
arguments      → expression ( "," expression )* ;

NUMBER         → DIGIT+ ( "." DIGIT+ )? ;
STRING         → "\"" <any char except "\"">* "\"" ;
IDENTIFIER     → ALPHA ( ALPHA | DIGIT )* ;
ALPHA          → "a" ... "z" | "A" ... "Z" | "_" ;
DIGIT          → "0" ... "9" ;
```

## Example programs
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

### 3. Curried function
```   
// original function that takes 3 inputs
fun original_fun(a, b, c){
    return a + b + c;
}

/* 
    partial1 is the same as
    fun partial1(b, c){
        return 1 + b + c;
    }
*/
var partial1 = original_fun(1);

/*
    partial2 is the same as 
    fun partial2(c){
        return 1 + 2 + c;
    }
*/
var partial2 = partial1(2);

print partial2(3);
```

### 4. Namespace and modifier
```
// always use 'const' before 'namespace'!!!
namespace foo {
    var x = 2;
}
foo = 2; // the namespace foo would become 2

const namespace bar {
    var x = 2;
}
bar = 2; // runtime error

// nested namespace is allowed
namespace foofoo {
    namespace barbar{
    }
}
```

## References
<ul>
<li><a href="https://craftinginterpreters.com/">Crafting Interpreters</a></li>
</ul>


