project(SimpleCalc)
cmake_minimum_required (VERSION 2.8)

SET(HEADERS
    Expression.h
    SimpleElement.h
    Symbol.h
    Number.h
	)

SET(SOURCE
    Expression.cpp
    main.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCE} ${HEADERS})
