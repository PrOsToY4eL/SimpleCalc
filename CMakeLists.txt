project(SimpleCalc)
cmake_minimum_required (VERSION 3.5)

set (CMAKE_CXX_STANDARD 14)

SET(HEADERS
    Expression.h
    Token.h
    Operand.h
    Operation.h
	AST.h
	)

SET(SOURCE
    Expression.cpp
	AST.cpp
    main.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCE} ${HEADERS})
