#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Token.h"
#include "TokenType.h"

namespace lox {

class Scanner {
  public:
    Scanner(const std::string& source);
    ~Scanner() {}
    Scanner(const Scanner&) = delete;
    Scanner(Scanner&&) = delete;
    Scanner& operator=(const Scanner&) = delete;
    Scanner& operator=(Scanner&&) = delete;
    std::vector<Token> scan_tokens();
    void scan_token();
  private:
    bool is_at_end() const;
    char advance();
    void add_token(TokenType type);
    void add_token(TokenType type, const std::string& literal);
    bool match(char expected);
    char peek_one() const;
    char peek_two() const;
    bool is_digit(char c) const;
    bool is_alpha(char c) const;
    bool is_alpha_numeric(char c) const;
    void find_number();
    void find_string();
    void find_identifier();
  private:
    std::string m_source;
    std::vector<Token> m_tokens;
    size_t m_start = 0;
    size_t m_current = 0;
    int m_line = 1;
    std::unordered_map<std::string, TokenType> m_reserved;
};

}

#endif //SCANNER_H
