This project is an interpreter of a simplified version of C programming language, written in C++. Tests are made using by Google Test.

Supported types:
int (64-bit integer number)
double (like in c)
Types can be converted explicitly and conversion from int to double can be implicit.

Supported features:
–variables of types int, double. Length of life of those object is similar to C
–operators: unary prefix, arithmetic, logical, comparsion, assignment
–block instruction and parenthesises are supported
–if and loops for and while are supported
–comments can be made in single line or multi-line like in C.
–functions can be defined with or without returned type. In latter case formal type "void" is used like in C
–int main() function is required as entry point
–only single file source codes and passing source code as parameter are supported

Phases of interpretation
Scaner - converts code into tokens
Parser - converts tokens into abstract syntax tree (AST)
MappedSyntaxTree - last phase of syntax analysis
Interpreter - executes abstract syntax tree (AST)

Example of using the language:

int square(int a) {
  return a*a;
}
int main() {
  int a=scan();
  for(int i=0;i<5;i++) {
    a+=square(i);
  }
  //program should print scan()+30
  print("Value: "+a);
}


int Fibonacci(int a) {
  if(a==0 || a==1) {return 1;}
  return Fibonacci(a-1)+Fibonacci(a-2);
  }
int main() {
  int a=scan();
  //if a>92, there will be error
  print("Value: "+Fibonacci(a));
}

Error handling:
Scaner phase - shows erronous line (part of text) with number of line
Parser phase - shows type of error with number of line
Mapping the tree phase - shows type of error
Interpreter phase - exception std::runtime_error
