set(Sources
    Main.cpp
    Lox.cpp
    Token.cpp
    Scanner.cpp
    )

set(Headers
    Lox.hpp
    ResultCode.hpp
    TokenType.hpp
    Token.hpp
    Scanner.hpp
    )

add_executable(
    Lox
    ${Headers}
    ${Sources}
    )
