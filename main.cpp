#include <cassert>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <streambuf>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "token.h"
#include "scanner.h"

class ExprVisitor;

class Expr
{
  public:
    virtual ~Expr () {};

    virtual void accept_visitor (const ExprVisitor & v) = 0;
};

using ExprPtr = std::shared_ptr<Expr>;

class ExprBinary;
class ExprGrouping;
class ExprUnary;
class ExprLiteral;

class ExprVisitor
{
  public:
    virtual void visitBinary (const ExprBinary & e) const = 0;
    virtual void visitGrouping (const ExprGrouping & e) const = 0;
    virtual void visitUnary (const ExprUnary & e) const = 0;
    virtual void visitLiteral (const ExprLiteral & e) const = 0;

    virtual ~ExprVisitor () {};
};



class ExprBinary : public Expr
{
  public:
 
    ExprBinary (ExprPtr left, Token token, ExprPtr right)
      : m_left (left)
      , m_token (token)
      , m_right (right)
    {
    }

    void accept_visitor (const ExprVisitor & v) override
    {
      v.visitBinary (*this);
    }

    virtual ~ExprBinary ()
    {
    }

    ExprPtr m_left;
    Token m_token;
    ExprPtr m_right;
};


class ExprGrouping : public Expr
{
  public:
    ExprGrouping (ExprPtr expression)
      : m_expression (expression)
    {
    }

    void accept_visitor (const ExprVisitor & v) override
    {
      v.visitGrouping (*this);
    }

    virtual ~ExprGrouping ()
    {
    }

    ExprPtr m_expression;
};

class ExprLiteral : public Expr
{
  public:
    ExprLiteral (std::any value)
      : m_value (value)
    {
    }

    void accept_visitor (const ExprVisitor & v) override
    {
      v.visitLiteral (*this);
    }

    virtual ~ExprLiteral ()
    {
    }

    std::any m_value;
};

class ExprUnary : public Expr
{
  public:
    ExprUnary (Token token, ExprPtr right)
      : m_token (token)
      , m_right (right)
    {
    }

    void accept_visitor (const ExprVisitor & v) override
    {
      v.visitUnary (*this);
    }

    virtual ~ExprUnary ()
    {
    }

    Token m_token;
    ExprPtr m_right;
};



void run (const std::string &code)
{
  /*
   * Create scanner from code
   */
  Scanner scanner (code);

  /*
   * Scan tokens
   */
  const auto tokens = scanner.scan_tokens ();
  
  /*
   * Dump tokens for debugging
   */
  for (auto token : tokens)
  {
    std::cout << token << std::endl;
  }
}

/*
 * REPL prompt
 */
int run_prompt () {
  for (std::string line; std::getline (std::cin, line);)
  {
    run (line);
  }
  return EXIT_SUCCESS;
}

/*
 * Process file
 */
int run_file (const std::string &filename)
{
  std::ifstream is (filename);
  std::string text ((std::istreambuf_iterator<char>(is)),
                    std::istreambuf_iterator<char>());
  run (text);
  return EXIT_SUCCESS;
}

/*
 * AST Printer
 */
class ASTPrinter : public ExprVisitor
{
  public:

    void visitBinary (const ExprBinary & e) const override
    {
      std::cout << "(" <<  e.m_token.m_lexeme << " ";
      e.m_left->accept_visitor (*this);
      std::cout << " ";
      e.m_right->accept_visitor (*this);
      std::cout << ")";
    }

    void visitGrouping (const ExprGrouping & e) const override
    {
      std::cout << "(group ";
      e.m_expression->accept_visitor (*this);
      std::cout << ")";
    }

    void visitUnary (const ExprUnary & e) const override 
    {
      std::cout << "(" << e.m_token.m_lexeme << " ";
      e.m_right->accept_visitor (*this);
      std::cout << ")";
    }

    void visitLiteral (const ExprLiteral & e) const override 
    {
      if (e.m_value.has_value ())
      {
        if (e.m_value.type () == typeid (double))
        {
          std::cout << std::any_cast<double> (e.m_value);
        }
        else if (e.m_value.type () == typeid (std::string))
        {
          std::cout << std::any_cast<std::string> (e.m_value);
        }
        else
        {
          std::cout << "UNKNOWN_LITERAL_TYPE";
        }
      }
      else
      {
        std::cout << "NULL_LITERAL";
      }
    }

};


/*
 * Main
 */
int main (int argc, char **argv)
{


  // Manually make a tree for testing
//  auto e = std::make_shared<ExprBinary> 
//    (std::make_shared <ExprUnary>
  
  auto t1 = Token (TokenType::TOK_MINUS, "-", 0);
  auto t2 = Token (TokenType::TOK_STAR, "*", 0);

  auto e1 = std::make_shared<ExprLiteral> (45.67);
  auto e2 = std::make_shared<ExprGrouping> (e1);
  auto e3 = std::make_shared<ExprLiteral> (123.0);
  auto e4 = std::make_shared<ExprUnary> (t1, e3);
  auto e = std::make_shared<ExprBinary> (e4, t2, e2);

  /*
   * Now print e
   */
  ASTPrinter printer;
  e->accept_visitor (printer);




  if (argc == 1)
  {
    return run_prompt ();
  } 
  else if (argc == 2)
  {
    return run_file (argv[1]);
  }
  else
  {
    std::cerr << "Incorrect use" << std::endl;
    return EXIT_FAILURE;
  }
}
