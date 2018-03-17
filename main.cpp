#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include <vector>

enum class TokenType
{
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

std::string to_string (const TokenType t)
{
    switch (t)
    {
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
}

class Token
{
    public:
    
    std::string str () const
    {
        return "Token (" + to_string (m_type) + ", " + 
               std::to_string (m_line) + ", '" + m_lexeme + "')";        
    }

    TokenType m_type;
    std::string m_lexeme;
    int m_line;
};

std::ostream & operator << (std::ostream & os, const Token & token)
{
  os << token.str ();
  return os;
}

class Scanner
{
    public:
        
    Scanner (const std::string & code)
        : m_code (code)
    {

    }

    std::vector<Token> tokens () const
    {
        return {};
    }

    std::string m_code;

};

void run (const std::string & code)
{
    std::cout << "RUN: " << code << std::endl;
    Scanner scanner (code);
    const auto tokens = scanner.tokens ();
    for (auto token : tokens)
    {
        std::cout << token << std::endl;
    }
}

int run_prompt ()
{
    for (std::string line; std::getline (std::cin, line); )
    {
        run (line);
    } 
    return 0;
}

int run_file (const std::string & filename)
{
    std::ifstream is (filename);
    std::string text ((std::istreambuf_iterator<char> (is)),
                       std::istreambuf_iterator<char> ());
    run (text);
    return 0;
}

int main(int argc, char **argv)
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
    }
}

