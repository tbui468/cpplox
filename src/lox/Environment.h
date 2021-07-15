#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include <memory>
#include "Object.h"
#include "Token.h"

namespace lox {

  class Environment {
    public:
      Environment(): m_enclosing(nullptr) {}
      Environment(std::shared_ptr<Environment> enclosing): m_enclosing(std::move(enclosing)) {}
      void define(std::string name, Object value);
      void assign(Token name, Object value);
      Object get(Token name);
    public:
      std::unordered_map<std::string, Object> m_values;
      std::shared_ptr<Environment> m_enclosing;
  };

}

#endif //ENVIRONMENT_H
