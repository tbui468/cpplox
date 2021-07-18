#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "Object.h"
#include "Token.h"

namespace lox {

  class Environment {
    public:
      Environment(): m_enclosing(nullptr) {}
      Environment(std::shared_ptr<Environment> enclosing): m_enclosing(enclosing) {}
      void define(const std::string& name, std::shared_ptr<Object> value);
      void assign(Token name, std::shared_ptr<Object> value);
      std::shared_ptr<Object> get(const Token& name);
      void print_data() const;
    public:
      std::unordered_map<std::string, std::shared_ptr<Object>> m_values;
      const std::shared_ptr<Environment> m_enclosing;
  };

}

#endif //ENVIRONMENT_H
