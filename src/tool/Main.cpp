#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <algorithm>

namespace genast {

  //could probably make this class static since we'll never need more than a single instance
  class GenAST {
    public:
      void define_ast(const std::string& out_dir, const std::string& base_name, const std::array<std::string, 4>& types) {
        std::ofstream outfile(out_dir + "/" + base_name + ".hpp");
        std::string upper_name = base_name;
        std::transform(upper_name.begin(), upper_name.end(), upper_name.begin(), 
                        [](char c) -> char { return char(std::toupper(int(c))); });
       
        //create base class 
        {
          //header guards
          outfile << "#ifndef " << upper_name << "_H" << '\n';
          outfile << "#define " << upper_name << "_H" << "\n\n";
          outfile << "class " << base_name << " {" << '\n';
          outfile << "};\n";

          outfile << "\n";
          outfile << "#endif " << "//" << upper_name << "_H" << std::endl;
        }

        //create subclasses
        {
          for (std::string type: types)
          {
            split(type);   
            //get class name
            //get field list
            //define_type(outfile , base_name, class_name, field_list);
          }
          /*
          "Binary : Expr left, Token operator, Expr right",
          "Grouping : Expr expression",
          "Literal : std::string value",
          "Unary : Token operator, Expr right",*/
          //for each string in type
            //use first word as subclass name (be sure to extend Expr)
            //split words on right side of : by ', ' and set them to member fields

        }

        outfile.close();
      }
    private:
      //void define_type(const std::ofstream& out, const std::string& base_name, const std::string& class_name, const std::string& field_list) {

      //}

      //std::vector<std::string> split(const std::string& text) {
      void split(const std::string& text) {
        std::cout << text.find(":") << std::endl;
      }
  };


}

int main(int argc, char** argv) {
  if (argc == 2)
  {
    std::string out_dir = argv[1];
    genast::GenAST gen_ast;
    std::array<std::string, 4> types = {
      "Binary : Expr left, Token operator, Expr right",
      "Grouping : Expr expression",
      "Literal : std::string value",
      "Unary : Token operator, Expr right",
    };
    gen_ast.define_ast(out_dir, "Expr", types);
  }
  else
  {
    std::cout << "Usage: genast <output directory>" << std::endl;
    return 1;
  }

  return 0; 
}
