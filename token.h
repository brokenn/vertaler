#ifndef TOKEN_H
#define TOKEN_H

#include <any>
#include <iostream>
#include <string>

/*
 * Types of Token
 */
enum class TokenType {
  /*
   * Single char tokens
   */
  TOK_LEFT_PAREN,
  TOK_RIGHT_PAREN,
  TOK_LEFT_BRACE,
  TOK_RIGHT_BRACE,
  TOK_COMMA,
  TOK_DOT,
  TOK_MINUS,
  TOK_PLUS,
  TOK_SEMICOLON,
  TOK_SLASH,
  TOK_STAR,
  
  /*
   * One or two char tokens
   */
  TOK_BANG,
  TOK_BANG_EQUAL,
  TOK_EQUAL,
  TOK_EQUAL_EQUAL,
  TOK_GREATER,
  TOK_GREATER_EQUAL,
  TOK_LESS,
  TOK_LESS_EQUAL,

  /*
   * Literals
   */
  TOK_IDENTIFIER,
  TOK_STRING,
  TOK_NUMBER,

  /*
   * Keywords
   */
  TOK_AND,
  TOK_CLASS,
  TOK_ELSE,
  TOK_FALSE,
  TOK_FUN,
  TOK_FOR,
  TOK_IF,
  TOK_NIL,
  TOK_OR,
  TOK_PRINT,
  TOK_RETURN,
  TOK_SUPER,
  TOK_THIS,
  TOK_TRUE,
  TOK_VAR,
  TOK_WHILE,
  TOK_EOF
};


/* 
 * Convert TokenType to string
 */
std::string to_string (const TokenType t);

/*
 * Represent a single token
 */
class Token {
public:

  /*
   * Construct token with value
   */
  template <typename T>
  Token (const TokenType type, std::string lexeme, int line, T && v);

  /*
   * Construct Token without value
   */
  Token (const TokenType type, std::string lexeme, int line);

  /*
   * Convert token to string representation
   */
  std::string str () const;

  TokenType m_type;
  std::string m_lexeme;
  int m_line;
  std::any m_value;
};

std::ostream &operator<< (std::ostream &os, const Token &token);

#endif
