#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <algorithm>

namespace genast {

  class GenAST {
    public:
    public:
      void define_ast(std::string out_dir, std::string base_name, std::array<std::string, 4> types) {
        std::ofstream outfile(out_dir + "/" + base_name + ".hpp");
        std::string upper_name = base_name;
        std::transform(upper_name.begin(), upper_name.end(), upper_name.begin(), ::toupper);
       
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

        outfile.close();
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
