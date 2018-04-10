#include "token.h"

#include <cassert>

/* 
 * Convert TokenType to string
 */
std::string to_string (const TokenType t) {
  switch (t) {
  case TokenType::TOK_LEFT_PAREN:
    return "LEFT_PAREN";
  case TokenType::TOK_RIGHT_PAREN:
    return "RIGHT_PAREN";
  case TokenType::TOK_LEFT_BRACE:
    return "LEFT_BRACE";
  case TokenType::TOK_RIGHT_BRACE:
    return "RIGHT_BRACE";
  case TokenType::TOK_COMMA:
    return "COMMA";
  case TokenType::TOK_DOT:
    return "DOT";
  case TokenType::TOK_MINUS:
    return "MINUS";
  case TokenType::TOK_PLUS:
    return "PLUS";
  case TokenType::TOK_SEMICOLON:
    return "SEMICOLON";
  case TokenType::TOK_SLASH:
    return "SLASH";
  case TokenType::TOK_STAR:
    return "STAR";
  case TokenType::TOK_BANG:
    return "BANG";
  case TokenType::TOK_BANG_EQUAL:
    return "BANG_EQUAL";
  case TokenType::TOK_EQUAL:
    return "EQUAL";
  case TokenType::TOK_EQUAL_EQUAL:
    return "EQUAL_EQUAL";
  case TokenType::TOK_GREATER:
    return "GREATER";
  case TokenType::TOK_GREATER_EQUAL:
    return "GREATER_EQUAL";
  case TokenType::TOK_LESS:
    return "LESS";
  case TokenType::TOK_LESS_EQUAL:
    return "LESS_EQUAL";
  case TokenType::TOK_IDENTIFIER:
    return "IDENTIFIER";
  case TokenType::TOK_STRING:
    return "STRING";
  case TokenType::TOK_NUMBER:
    return "NUMBER";
  case TokenType::TOK_AND:
    return "AND";
  case TokenType::TOK_CLASS:
    return "CLASS";
  case TokenType::TOK_ELSE:
    return "ELSE";
  case TokenType::TOK_FALSE:
    return "FALSE";
  case TokenType::TOK_FUN:
    return "FUN";
  case TokenType::TOK_FOR:
    return "FOR";
  case TokenType::TOK_IF:
    return "IF";
  case TokenType::TOK_NIL:
    return "NIL";
  case TokenType::TOK_OR:
    return "OR";
  case TokenType::TOK_PRINT:
    return "PRINT";
  case TokenType::TOK_RETURN:
    return "RETURN";
  case TokenType::TOK_SUPER:
    return "SUPER";
  case TokenType::TOK_THIS:
    return "THIS";
  case TokenType::TOK_TRUE:
    return "TRUE";
  case TokenType::TOK_VAR:
    return "VAR";
  case TokenType::TOK_WHILE:
    return "WHILE";
  case TokenType::TOK_EOF:
    return "EOF";
  }

  /*
   * Unreachable
   */
  assert (false);
  return "";
}



/*
 * Construct token with value
 */
template <typename T>
Token::Token (const TokenType type, std::string lexeme, int line, T && v)
    : m_type (type)
    , m_lexeme (std::move (lexeme))
    , m_line (line)
    , m_value (v)
{
}

/*
 * Construct Token without value
 */
Token::Token (const TokenType type, std::string lexeme, int line)
  : Token (type, std::move (lexeme), line, std::any {})
{
}

std::string Token::str () const
{
  std::string s = "Token (";
  s += to_string (m_type);
  s += ", ";
  s += std::to_string (m_line);
  s += ", ";
  s += m_lexeme;
  if (m_value.has_value ())
  {
    s += ", ";
    if (m_type == TokenType::TOK_STRING)
    {
      s += "'" + std::any_cast<std::string> (m_value) + "'";
    }
    else if (m_type == TokenType::TOK_NUMBER)
    {
      s += std::to_string (std::any_cast<double> (m_value));
    }
    else
    {
      s += "UNKNOWN_VALUE_TYPE";
    }
  }
  s += ")";
  return s;
}

std::ostream &operator<< (std::ostream &os, const Token &token) {
  os << token.str ();
  return os;
}

template Token::Token (const TokenType, std::string, int, const double &);
template Token::Token (const TokenType, std::string, int, const std::string &);
