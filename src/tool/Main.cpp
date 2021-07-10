#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>



namespace genast {

  //could probably make this class static since we'll never need more than a single instance
  class GenAST {
    public:
      void define_ast(const std::string& out_dir, const std::string& base_name, const std::array<std::string, 4>& types) {
        std::ofstream outfile(out_dir + "/" + base_name + ".hpp");
        std::string upper_name = base_name;
        std::transform(upper_name.begin(), upper_name.end(), upper_name.begin(), 
            [](char c) -> char { return char(std::toupper(int(c))); });

        {
          //header guards
          outfile << "#ifndef " << upper_name << "_H" << '\n';
          outfile << "#define " << upper_name << "_H" << "\n";
          outfile << "#include \"Token.hpp\"\n";
          outfile << "namespace lox {" << "\n\n";

          outfile << "struct " << base_name << " {" << '\n';
          outfile << "};\n";

          outfile << "\n";
        }

        //create subclasses
        {
          for (std::string type: types)
          {
            std::vector<std::string> spl = split_once(type, ":");   
            std::string class_name = spl.at(0);
            std::string fields_string = spl.at(1);
            define_type(outfile , base_name, class_name, fields_string);
          }

        }

        outfile << "}\n\n";
        outfile << "#endif " << "//" << upper_name << "_H" << std::endl;

        outfile.close();
      }
    private:
/*
struct Binary: public Expr {
  Binary(Expr left, Token perator, Expr right): left(left), perator(perator), right(right) {} //add this!!!
  Expr left;
  Token perator;
  Expr right;
};*/
      void define_type(std::ofstream& outfile, const std::string& base_name, const std::string& class_name, const std::string& fields_string) {
        std::vector<std::string> fields = split(fields_string, ",");
        std::vector<std::string> classes;
        std::vector<std::string> variables;
        for(const std::string& f: fields) {
          std::vector<std::string> spl = split(f, " ");
          classes.push_back(spl.at(0));
          variables.push_back(spl.at(1));
        }
        outfile << "\n\n";
        outfile << "struct " << class_name << ": public " << base_name << " {" << '\n';
        //constructor
        outfile << "  " << class_name << "(";
        for (int i = 0; i < fields.size(); i++) {
          outfile << classes.at(i) << " " << variables.at(i);
          if (i != fields.size() - 1)
            outfile << ", ";
        }
        outfile << "): ";

        for(int i = 0; i < fields.size(); i++) {
          outfile << variables.at(i) << "(" << variables.at(i) << ")";
          if (i != fields.size() - 1)
           outfile << ", ";
          else
           outfile << " {}\n"; 
        }

        for(const std::string& f: fields) {
          outfile << "  " << f << ";\n";
        }
        outfile << "};\n";
      }
      void ltrim(std::string& s) {
        s.erase(s.begin(), 
            std::find_if(s.begin(), s.end(), [](char c){ return !std::isspace(c); }));
      }
      void rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](char c) { return !std::isspace(c); }).base(), s.end());
      }
      void trim(std::string& s) {
        ltrim(s);
        rtrim(s);
      }
      std::vector<std::string> split(std::string text, std::string del) {
        size_t pos = 0;
        std::vector<std::string> out;
        std::string token;
        while((pos = text.find(del)) != std::string::npos) { //note: setting pos, and then checking for equality between pos and npos
          token = text.substr(0, pos);
          trim(token);
          out.push_back(token);
          text.erase(text.begin(), text.begin() + pos + del.length());
        }
        trim(text);
        if (text != "") {
          out.push_back(text);
        }
        return out;
      }
      //only split by delimiter 1 time
      std::vector<std::string> split_once(std::string text, std::string del) {
        size_t pos = text.find(del);
        std::string token = text.substr(0, pos);
        text.erase(text.begin(), text.begin() + pos + del.length());
        std::vector<std::string> out;
        trim(token);
        out.push_back(token);
        trim(text);
        out.push_back(text);
        return out;
      }
  };


  }

  int main(int argc, char** argv) {
    if (argc == 2)
    {
      std::string out_dir = argv[1];
      genast::GenAST gen_ast;
      std::array<std::string, 4> types = {
        "Binary : Expr left, Token oprtr, Expr right",
        "Grouping : Expr expression",
        "Literal : std::string value",
        "Unary : Token oprtr, Expr right",
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
