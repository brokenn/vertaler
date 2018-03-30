#include <cassert>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <streambuf>
#include <string>
#include <vector>

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
 * Represent a single token
 */
class Token {
public:
  Token (const TokenType type, std::string lexeme, int line)
      : m_type (type),
	m_lexeme (lexeme),
	m_line (line) {}

  std::string str () const
  {
    return "Token (" + to_string (m_type) + ", " + std::to_string (m_line) +
           ", '" + m_lexeme + "')";
  }

  TokenType m_type;
  std::string m_lexeme;
  int m_line;
};

std::ostream &operator<< (std::ostream &os, const Token &token) {
  os << token.str ();
  return os;
}


/*
 * Scanner, tokenizes code
 */
class Scanner {

public:
  Scanner (const std::string &code) : m_code (code) {}

  /*
   *
   */ 
  std::ostream & error ()
  {
    m_error = true;
    std::cerr << "line " << m_line << ":";
    return std::cerr;
  }

  /*
   * EOF
   */ 
  bool is_at_end () const
  {
    return m_current >= static_cast<int> (m_code.size ());
  }

  /*
   * Consume and advance to next character
   */ 
  char advance ()
  {
    return m_code[m_current++];
  }

  /*
   * Peek at the current character
   */
  char peek () const
  {
    return is_at_end () ? '\0' : m_code[m_current];
  }

  /*
   * Consume character if it matches
   */
  bool match (char m)
  {
    if (is_at_end ())
    {
      return false;
    }

    if (m_code[m_current] != m)
    {
      return false;
    }
  
    /*
     * Character matched, advance and return true
     */
    ++m_current;
    return true;
  }


  /*
   * Create token of type
   */
  void add_token (TokenType type)
  {
    m_tokens.push_back
      (Token (type,
              m_code.substr (m_start, m_current - m_start),
              m_line));
  }

  /*
   * Find next token
   */
  void scan_token () {
    /*
     * Read one character
     */
    char c = advance ();

    switch (c) {
    
    /*
     * Single char tokens
     */
    case '(':
      add_token (TokenType::TOK_LEFT_PAREN);
      break;
    case ')':
      add_token (TokenType::TOK_RIGHT_PAREN);
      break;
    case '{':
      add_token (TokenType::TOK_LEFT_BRACE);
      break;
    case '}':
      add_token (TokenType::TOK_RIGHT_BRACE);
      break;
    case ',':
      add_token (TokenType::TOK_COMMA);
      break;
    case '.':
      add_token (TokenType::TOK_DOT);
      break;
    case '-':
      add_token (TokenType::TOK_MINUS);
      break;
    case '+':
      add_token (TokenType::TOK_PLUS);
      break;
    case ';':
      add_token (TokenType::TOK_SEMICOLON);
      break;
    case '*':
      add_token (TokenType::TOK_STAR);
      break;

    /*
     * Single-or-two-char tokens
     */ 
    case '!':
      add_token (match ('=') ?
                 TokenType::TOK_BANG_EQUAL :
                 TokenType::TOK_BANG);
      break;
    case '=':
      add_token (match ('=') ?
                 TokenType::TOK_EQUAL_EQUAL :
                 TokenType::TOK_EQUAL);
      break;
    case '<':
      add_token (match ('=') ?
                 TokenType::TOK_LESS_EQUAL :
                 TokenType::TOK_LESS);
      break;
    case '>':
      add_token (match ('=') ?
                 TokenType::TOK_GREATER_EQUAL :
                 TokenType::TOK_GREATER);
      break;
    
    /*
     * Division '/' or comment '// ...'
     */
    case '/':
      /*
       * If it is a comment, consume to the end of the line
       */
      if (match ('/'))
      {
        while (peek () != '\n' && !is_at_end ())
        {
          advance ();
        }
      }
      else
      {
        /*
         * Was actually just /, not a comment
         */
        add_token (TokenType::TOK_SLASH);
      }
      break;

    default:
      error () << "Unexpected Character '" << c << "'" << std::endl;
    }
  }

  /*
   * Convert text to tokens
   */
  std::vector<Token> scan_tokens()
  {
    while (!is_at_end ())
    {
      m_start = m_current;
      scan_token ();
    }

    /*
     * Final EOF token to indicate end-of-text
     */
    m_tokens.push_back (Token (TokenType::TOK_EOF, "", m_line));
    return m_tokens;
  }

  std::string m_code;
  std::vector<Token> m_tokens;

  /*
   * Set to true if any errors encountered in scanning
   */
  bool m_error = false;

  int m_line = 1;
  int m_start = 0;
  int m_current = 0;
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
 * Main
 */
int main (int argc, char **argv)
{
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
