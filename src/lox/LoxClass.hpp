#ifndef CLASS_H
#define CLASS_H

#include "Callable.hpp"
#include "Stmt.h"
#include "Environment.h"
#include "Object.h"

namespace lox {

  class LoxClass: public Object {
    public:
      LoxClass(const std::string& name);
      ~LoxClass() {}
      std::string to_string() const;
    private:
      std::string m_name;
  };


}


#endif //CLASS_H
