set(Sources
    Main.cpp
    Lox.cpp
    Token.cpp)

set(Headers
    Lox.hpp
    ResultCode.hpp
    TokenType.hpp
    Token.hpp
    )

add_executable(
    Lox
    ${Headers}
    ${Sources}
    )
