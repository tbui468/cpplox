set(Sources
    Main.cpp
    Lox.cpp)

set(Headers
    Lox.hpp
    ResultCode.hpp
    )

add_executable(
    Lox
    ${Headers}
    ${Sources}
    )
