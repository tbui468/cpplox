#ifndef LOX_INSTANCE_H
#define LOX_INSTANCE_H

#include "Object.h"
#include "LoxClass.hpp"

namespace lox {

  class LoxInstance: public Object {
    public:
      LoxInstance(std::shared_ptr<LoxClass> klass): Object(), m_class(klass) {}
      ~LoxInstance() {}
    private:
      std::shared_ptr<LoxClass> m_class;
  };

}

#endif //LOX_INSTANCE_H
