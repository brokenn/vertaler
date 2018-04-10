#include "scanner.h"

Scanner::Scanner (const std::string &code) : m_code (code) {}

std::ostream & Scanner::error ()
{
  m_error = true;
  std::cerr << "line " << m_line << ":";
  return std::cerr;
}

/*
 * EOF
 */ 
bool Scanner::is_at_end () const
{
  return m_current >= static_cast<int> (m_code.size ());
}

/*
 * Consume and advance to next character
 */ 
char Scanner::advance ()
{
  return m_code[m_current++];
}

/*
 * Peek at the current character
 */
char Scanner::peek () const
{
  return is_at_end () ? '\0' : m_code[m_current];
}

/*
 * Peek two chars ahead.
 * This could be rolled into the peek function above, but is
 * implemented as a separate function instead to clarify that this
 * scanner only has lookahead 2 and no more.
 *
 */
char Scanner::peek_next () const
{
  return (m_code.size () - m_current) < 2 ? '\0' : m_code[m_current+1];
}

/*
 * Consume character if it matches
 */
bool Scanner::match (char m)
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
 * Parse a string
 */
void Scanner::string ()
{
  /*
   * Remember line where the string started
   */
  const auto from_line = m_line;

  /*
   * Search for the next quote, incrementing our line counter
   * in the process.
   */
  while (peek () != '"' && !is_at_end ())
  {
    if (peek () == '\n')
    {
      ++m_line;
    }
    advance ();
  }

  /*
   * Unterminated string check
   */
  if (is_at_end ())
  {
    error () << "Unterminated string, started at line "
             << from_line << std::endl;
    return;
  }

  /*
   * Consume closing quote
   */
  advance ();

  /* 
   * Add token with surrounding quotes trimmed
   */
  const auto s = m_code.substr (m_start+1, m_current - m_start-2);

  add_token (TokenType::TOK_STRING, s);
}

bool Scanner::is_digit (char c) const
{
  return c >= '0' && c <= '9';
}

bool Scanner::is_alpha (char c) const
{
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
         c == '_';
}

bool Scanner::is_alphanumeric (char c) const
{
  return is_digit (c) || is_alpha (c);
}


/*
 * Parse number literal
 *  Valid numbers:
 *   1
 *   1.23
 *  Invalid numbers:
 *   .23
 *   1223.
 */
void Scanner::number ()
{
  /*
   * Eat all the leading digits
   */
  while (is_digit (peek ()))
  {
    advance ();
  }

  /*
   * If the next is a period, followed by digits, keep eating
   */
  if (peek () == '.' && is_digit (peek_next ()))
  {
    /*
     * Consume the period
     */
    advance ();

    /*
     * Consume all digits
     */
    while (is_digit (peek ()))
    {
      advance ();
    }
  }
  
  const auto f = std::atof (m_code.substr (m_start, m_current - m_start).c_str ());
  add_token (TokenType::TOK_NUMBER, f);
}

/*
 * Add identifier, which might be a keyword
 */
void Scanner::identifier ()
{
  while (is_alphanumeric (peek ()))
  {
    advance ();
  }

  /*
   * Extract text
   */
  const auto text = m_code.substr (m_start, m_current - m_start);

  /*
   * Define keywords
   */
  static const std::map<std::string, TokenType> keywords = 
    {{"and",    TokenType::TOK_AND},
     {"class",  TokenType::TOK_CLASS},
     {"else",   TokenType::TOK_ELSE},
     {"false",  TokenType::TOK_FALSE},
     {"for",    TokenType::TOK_FOR},
     {"fun",    TokenType::TOK_FUN},
     {"if",     TokenType::TOK_IF},
     {"nil",    TokenType::TOK_NIL},
     {"or",     TokenType::TOK_OR},
     {"print",  TokenType::TOK_PRINT},
     {"return", TokenType::TOK_RETURN},
     {"super",  TokenType::TOK_SUPER},
     {"this",   TokenType::TOK_THIS},
     {"true",   TokenType::TOK_TRUE},
     {"var",    TokenType::TOK_VAR},
     {"while",  TokenType::TOK_WHILE}};

  /*
   * If it is a keyword, add that, otherwise add identifier
   */
  const auto it = keywords.find (text);

  add_token (it == keywords.end () ?
             TokenType::TOK_IDENTIFIER :
             it->second);
}

/*
 * Create token of type
 */
void Scanner::add_token (TokenType type)
{
  m_tokens.push_back
    (Token (type,
            m_code.substr (m_start, m_current - m_start),
            m_line));
}

/*
 * Add token with value
 */
template <typename T>
void Scanner::add_token (TokenType type, T && v)
{
  m_tokens.push_back
    (Token (type,
            m_code.substr (m_start, m_current - m_start),
            m_line,
            v));
}

/*
 * Find next token
 */
void Scanner::scan_token () {
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

  /*
   * Whitespace
   */
  case ' ':
  case '\r':
  case '\t':
    break;

  /*
   * Newline
   */
  case '\n':
    m_line++;
    break;


  /*
   * Strings
   */
  case '"':
    string ();
    break;


  /*
   * Number literals
   */
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    number ();
    break;

  default:
    /*
     * Identifiers or keywords
     */
    if (is_alpha (c))
    {
      identifier ();
    }
    else
    {
      /*
       * Error case
       */
      error () << "Unexpected Character '" << c << "'" << std::endl;
    }
  }
}

/*
 * Convert text to tokens
 */
std::vector<Token> Scanner::scan_tokens()
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


