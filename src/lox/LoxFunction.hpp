#ifndef FUNCTION_H
#define FUNCTION_H

#include "Callable.hpp"

namespace lox {

  //class Function: public Callable, public Object {
  class LoxFunction: public Callable {
    public:
      LoxFunction(): Callable() {}
      ~LoxFunction() {}
      virtual Object call(Interpreter& interp, const std::vector<Object>& arguments) override {
        return Object(true); //TEMP
      }
      virtual int arity() override {
        return 0; //TEMP
      }
  };

}


#endif //FUNCTION_H
