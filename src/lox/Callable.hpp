#ifndef CALLABLE_H
#define CALLABLE_H

#include <vector>
#include "Object.h"
#include "Interpreter.h"

namespace lox {

  class Callable: public Object {
    public:
      Callable(): Object() {}
      virtual ~Callable() {}
      virtual Object call(Interpreter& interp, const std::vector<Object>& arguments) = 0;
      virtual int arity() = 0;
      virtual std::string to_string() = 0;
  };

}


#endif //CALLABLE_H
