project(SimpleCalc)
cmake_minimum_required (VERSION 2.8)

SET(HEADERS
    Expression.h
    Token.h
    Operand.h
    Operation.h
	)

SET(SOURCE
    Expression.cpp
    main.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCE} ${HEADERS})
