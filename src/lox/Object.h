#ifndef OBJECT_H
#define OBJECT_H

#include "TokenType.hpp"
#include <string>

namespace lox {

  class Object {
    public:
      Object(const std::string& s);
      Object(double number);
      Object(bool value);
      Object(); //of type nil
      
      bool is_bool() const;
      bool is_true() const;
      bool is_nil() const;
      bool is_number() const;
      bool is_string() const;
      double get_number() const;
      bool get_bool() const;
      void set_number(double n);
      std::string get_string() const;
    private:
      enum class Type{
        True,
        False,
        Nil,
        Number,
        String
      };
      Type m_type; 
      std::string m_string;
      double m_number;
  };

}


#endif //OBJECT_H
