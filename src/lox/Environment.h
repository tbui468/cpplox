#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include "Object.h"
#include "Token.h"

namespace lox {

  class Environment {
    public:
      void define(std::string name, Object value);
      void assign(Token name, Object value);
      Object get(Token name);
    private:
      std::unordered_map<std::string, Object> m_values;
  };

}

#endif //ENVIRONMENT_H
