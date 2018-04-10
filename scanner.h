#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"

#include <map>
#include <vector>
#include <ostream>


/*
 * Scanner, tokenizes code
 */
class Scanner {

public:
  Scanner (const std::string &code);

  /*
   *
   */ 
  std::ostream & error ();

  /*
   * EOF
   */ 
  bool is_at_end () const;

  /*
   * Consume and advance to next character
   */ 
  char advance ();

  /*
   * Peek at the current character
   */
  char peek () const;

  /*
   * Peek two chars ahead.
   * This could be rolled into the peek function above, but is
   * implemented as a separate function instead to clarify that this
   * scanner only has lookahead 2 and no more.
   *
   */
  char peek_next () const;

  /*
   * Consume character if it matches
   */
  bool match (char m);

  /*
   * Parse a string
   */
  void string ();

  bool is_digit (char c) const;

  bool is_alpha (char c) const;

  bool is_alphanumeric (char c) const;


  /*
   * Parse number literal
   *  Valid numbers:
   *   1
   *   1.23
   *  Invalid numbers:
   *   .23
   *   1223.
   */
  void number ();

  /*
   * Add identifier, which might be a keyword
   */
  void identifier ();

  /*
   * Create token of type
   */
  void add_token (TokenType type);

  /*
   * Add token with value
   *
   * Fixme: Belongs in detail, or .inl file
   */
  template <typename T>
  void add_token (TokenType type, T && v);

  /*
   * Find next token
   */
  void scan_token ();

  /*
   * Convert text to tokens
   */
  std::vector<Token> scan_tokens();



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



#endif

