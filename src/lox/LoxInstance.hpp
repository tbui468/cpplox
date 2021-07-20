#ifndef LOX_INSTANCE_H
#define LOX_INSTANCE_H

#include <unordered_map>
#include "Object.h"
#include "LoxClass.hpp"

namespace lox {

  class LoxInstance: public Object {
    public:
      LoxInstance(std::shared_ptr<LoxClass> klass): Object(), m_class(klass) {}
      ~LoxInstance() {}
      std::shared_ptr<Object> get(const Token& name);
      void set(const Token& name, std::shared_ptr<Object> value);
    private:
      std::shared_ptr<LoxClass> m_class;
      std::unordered_map<std::string, std::shared_ptr<Object>> m_fields;
  };

}

#endif //LOX_INSTANCE_H
