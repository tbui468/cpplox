#ifndef FUNCTION_H
#define FUNCTION_H

#include "Callable.hpp"

namespace lox {

  //class Function: public Callable, public Object {
  class Function: public Callable {
    public:
      Function(): Callable() {}
      ~Function() {}
      virtual Object call(Interpreter& interp, const std::vector<Object>& arguments) override {
        return Object(true); //TEMP
      }
      virtual int arity() override {
        return 0; //TEMP
      }
  };

}


#endif //FUNCTION_H
