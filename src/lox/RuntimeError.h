#ifndef RUNTIME_ERROR_H
#define RUNTIME_ERROR_H

#include <exception>
#include <string>
#include "Token.hpp"

namespace lox {

  class RuntimeError: public std::exception {
    public:
      RuntimeError(Token token, std::string message): m_message(message), m_token(token) {}
      //need a get message function
    private:
      Token m_token;
      std::string m_message;
  };

}


#endif //RUNTIME_ERROR_H
