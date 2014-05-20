#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

enum Token {
  tok_eof = -1,

  tok_identifier = -2, tok_number = -3
};

static string IdentifierStr;  // Filled in if tok_identifier
static double NumVal;         // Filled in if tok_number

static int gettok() {
  static int LastChar = ' ';

  // Skip any whitespace.
  while (isspace(LastChar))
    LastChar = getchar();

  if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    IdentifierStr = LastChar;
    while (isalnum((LastChar = getchar())))
      IdentifierStr += LastChar;

    return tok_identifier;
  }

  if (isdigit(LastChar) || LastChar == '.') {   // Number: [0-9.]+
    string NumStr;
    do {
      NumStr += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || LastChar == '.');

    NumVal = strtod(NumStr.c_str(), 0);
    return tok_number;
  }

  if (LastChar == '#') {
    // Comment until end of line.
    do LastChar = getchar();
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
    
    if (LastChar != EOF)
      return gettok();
  }
  
  // Check for end of file.  Don't eat the EOF.
  if (LastChar == EOF)
    return tok_eof;

  // Otherwise, just return the character as its ascii value.
  int ThisChar = LastChar;
  LastChar = getchar();
  return ThisChar;
}

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//
namespace {
/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
  virtual ~ExprAST() {}
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
public:
  NumberExprAST(double val) {}
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
  std::string Name;
public:
  VariableExprAST(const std::string &name) : Name(name) {}
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;
public:
  PrototypeAST(const std::string &name, const std::vector<std::string> &args)
    : Name(name), Args(args) {}
  
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
public:
  FunctionAST(PrototypeAST *proto, ExprAST *body) {}
};
} // end anonymous namespace
static int CurTok;
static int getNextToken() {
  return CurTok = gettok();
}

ExprAST *Error(const char *Str) { fprintf(stderr, "Error: %s\n", Str);return 0;}
PrototypeAST *ErrorP(const char *Str) { Error(Str); return 0; }

static ExprAST *ParseExpression();

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
static ExprAST *ParseIdentifierExpr() {
  std::string IdName = IdentifierStr;
  
  getNextToken();  // eat identifier.
  
  //if (CurTok != '(') // Simple variable ref.
    return new VariableExprAST(IdName);
  
  // Call.
  //getNextToken();  // eat (
  //std::vector<ExprAST*> Args;
  //if (CurTok != ')') {
    //while (1) {
      //ExprAST *Arg = ParseExpression();
      //if (!Arg) return 0;
      //Args.push_back(Arg);

      //if (CurTok == ')') break;

      //if (CurTok != ',')
        //return Error("Expected ')' or ',' in argument list");
      //getNextToken();
    //}
  //}

  //// Eat the ')'.
  //getNextToken();
  
  //return new CallExprAST(IdName, Args);
}

/// numberexpr ::= number
static ExprAST *ParseNumberExpr() {
  ExprAST *Result = new NumberExprAST(NumVal);
  getNextToken(); // consume the number
  return Result;
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
static ExprAST *ParsePrimary() {
  switch (CurTok) {
  default: return Error("unknown token when expecting an expression");
  case tok_identifier: return ParseIdentifierExpr();
  case tok_number:     return ParseNumberExpr();
  //case '(':            return ParseParenExpr();
  }
}

/// expression
///   ::= primary binoprhs
///
static ExprAST *ParseExpression() {
  ExprAST *expr = ParsePrimary();
  if (!expr) return 0;
  
  return expr;
}

/// toplevelexpr ::= expression
static FunctionAST *ParseTopLevelExpr() {
  if (ExprAST *E = ParseExpression()) {
    // Make an anonymous proto.
    PrototypeAST *Proto = new PrototypeAST("", std::vector<std::string>());
    return new FunctionAST(Proto, E);
  }
  return 0;
}

static void HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (ParseTopLevelExpr()) {
    fprintf(stderr, "Parsed a top-level expr\n");
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void MainLoop() {
  while (1) {
    fprintf(stderr, "ready> ");
    switch (CurTok) {
    case tok_eof:    return;
    case ';':        getNextToken(); break;  // ignore top-level semicolons.
    default:         HandleTopLevelExpression(); break;
    }
  }
}

int
main(int argc, const char ** argv)
{
  fprintf(stderr, "ready> ");
  getNextToken();
  MainLoop();
  return 0;
}

