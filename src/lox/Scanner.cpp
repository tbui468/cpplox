#include "Scanner.hpp"
#include "Lox.hpp"
#include <iostream>

namespace lox
{

std::vector<Token> Scanner::scan_tokens()
{
  while(!is_at_end())
  {
    m_start = m_current;
    scan_token();
  }
  m_tokens.push_back(Token(TokenType::EOFILE, "", "", m_line));
  return m_tokens;
}


void Scanner::scan_token()
{
  char c = advance();
  switch(c)
  {
    case '(': add_token(TokenType::LEFT_PAREN); break;
    case ')': add_token(TokenType::RIGHT_PAREN); break;
    case '{': add_token(TokenType::LEFT_BRACE); break;
    case '}': add_token(TokenType::RIGHT_BRACE); break;
    case ',': add_token(TokenType::COMMA); break;
    case '.': add_token(TokenType::DOT); break;
    case '-': add_token(TokenType::MINUS); break;
    case '+': add_token(TokenType::PLUS); break;
    case ';': add_token(TokenType::SEMICOLON); break;
    case '*': add_token(TokenType::STAR); break;
    case '!': add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
    case '=': add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
    case '<': add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
    case '>': add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
    case '/':
      if(match('/')) {
        while (peek_one() != '\n' && !is_at_end()) advance();
      } else {
        add_token(TokenType::SLASH);
      }
      break;
    case ' ':
    case '\r':
    case '\t':
      //ignore white space
      break;
    case '\n':
      m_line++;
      break;
    case '"': find_string();
      break;
    default: 
      if (is_digit(c))
      {
        find_number();
      }
      else if (is_alpha(c))
      {
        find_identifier();
      }
      else
      {
        Lox::error(m_line, "Unexpected character."); 
        std::cout << c << std::endl;
      }
      break;
  }
}

bool Scanner::is_at_end()
{
  return m_current >= m_source.length();
}

//get next 
char Scanner::advance()
{
  char c = m_source.at(m_current);
  m_current++;
  return c;
}

//add token with no associated literal
void Scanner::add_token(TokenType type)
{
  add_token(type, "");
}

//add token with associate literal (string or double)
void Scanner::add_token(TokenType type, std::string literal)
{
  std::string text = m_source.substr(m_start, m_current - m_start);
  m_tokens.push_back(Token(type, text, literal, m_line));
}

//check for lexemes with up to two characters
bool Scanner::match(char expected)
{
  if(is_at_end()) return false;
  if(m_source.at(m_current) != expected) return false;

  m_current++;
  return true;
}

char Scanner::peek_one()
{
  if(is_at_end()) return '\0'; //endoffile character.  Is this used???
  return m_source.at(m_current);
}

char Scanner::peek_two()
{
  if(m_current + 1 >= m_source.length()) return '\0';
  return m_source.at(m_current + 1);
}

void Scanner::find_string()
{
  //advance m_current and m_line until next char is endoffile or closing "
  while(peek_one() != '"' && !is_at_end())
  {
    if (peek_one() == '\n') m_line++;
    advance();
  }  

  if (is_at_end())
  {
    Lox::error(m_line, "Unterminated string.");
    return;
  }

  //include the closing "
  advance();
  std::string literal = m_source.substr(m_start + 1, m_current - m_start - 2);
  add_token(TokenType::STRING, literal);
}


bool Scanner::is_digit(char c)
{
  return c >= '0' && c <= '9';
}

void Scanner::find_number()
{
  while (is_digit(peek_one())) advance(); //consume any remaining digits

  if (peek_one() == '.' && is_digit(peek_two()))
  {
    advance(); //consume '.'
    while (is_digit(peek_one())) advance(); //consume any remaining digits
  }

  add_token(TokenType::NUMBER, m_source.substr(m_start, m_current - m_start));
}


bool Scanner::is_alpha(char c)
{
  return (c >= 'a' && c <= 'z') ||
      (c >= 'A' && c <= 'Z') ||
      c == '_';
}

bool Scanner::is_alpha_numeric(char c)
{
  return is_digit(c) || is_alpha(c);
}

void Scanner::find_identifier()
{
  while (is_alpha_numeric(peek_one())) advance();

  std::string text = m_source.substr(m_start, m_current - m_start);
  if (m_reserved.count(text) > 0)
  {
    add_token(m_reserved[text]);
  }
  else
  {
    add_token(TokenType::IDENTIFIER);
  }
}

}
