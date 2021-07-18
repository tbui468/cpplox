#ifndef LOX_RETURN_H
#define LOX_RETURN_H

#include <exception>

namespace lox {
  class LoxReturn: public std::exception {
    public:
      LoxReturn(std::shared_ptr<Object> value): value(value) {}
      std::shared_ptr<Object> value;
  };
}

#endif //RETURN_H
