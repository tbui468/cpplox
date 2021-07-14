set(Sources
    Main.cpp
    Lox.cpp
    Token.cpp
    Scanner.cpp
    AstPrinter.cpp
    Parser.cpp
    )

set(Headers
    Lox.hpp
    ResultCode.hpp
    TokenType.hpp
    Token.hpp
    Scanner.hpp
    Expr.hpp
    AstPrinter.hpp
    Parser.hpp
    Object.h
    )

add_executable(
    Lox
    ${Headers}
    ${Sources}
    )
