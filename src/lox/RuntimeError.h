#ifndef RUNTIME_ERROR_H
#define RUNTIME_ERROR_H

#include <exception>
#include <string>
#include "Token.h"

namespace lox {

  class RuntimeError: public std::exception {
    public:
      RuntimeError(Token token, std::string message): m_message(message), m_token(token) {}
      inline std::string get_message() const {
        return m_message;
      }
      inline Token get_token() const {
        return m_token;
      }
    private:
      Token m_token;
      std::string m_message;
  };

}


#endif //RUNTIME_ERROR_H
