## Cmake by xkment06

cmake_minimum_required(VERSION 3.12.0)
project(ivs_proj_2 LANGUAGES CXX)

# set names
set(MathLib math_lib)
set(MathLibTests math_lib_tests)
set(CalculatorGui calculatorGui)
set(Profiling profiling)
set(RandomFloatNumberGenerator randomFloatNumberGenerator)

# set parameters
set(CMAKE_CXX_STANDART 11)
set(POSITION_INDEPENDENT_CODE ON)
    #qt
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# to repair linker
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# setup google tests
enable_testing()
add_subdirectory(googletest-master)

# setup qt components
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

# define files roles
    # math lib
set(MathLib_h
	calc.h
	controller.h
)
set(MathLib_cpp
	calc.cpp
	controller.cpp
)
    # main
set(Gui_ui
    mainwindow.ui
)
set(Gui_cpp
    calc.cpp
    mainwindow.cpp
    main.cpp
)
set(Gui_h
    mainwindow.h
)
    # tests
set(Tests_cpp
    test.cpp
)
    # profiling
set(Profiling_cpp
    profiling.cpp
)
set(RandomFloatNumberGenerator_cpp
    randomFloatNumberGenerator.cpp
)

# qt generated files
qt5_wrap_ui(project_headers_wrapped ${Gui_ui})
qt5_wrap_cpp(project_sources_moc ${Gui_h})

# setup math lib
add_library(${MathLib} STATIC ${MathLib_cpp} ${MathLib_h})


# todo library
# setup executable Gui
add_executable(${CalculatorGui} ${Gui_cpp} ${Gui_h} ${project_headers_wrapped} ${project_sources_moc})
target_link_libraries(${CalculatorGui} PUBLIC
    ${MathLib}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

# setup executable profiling
add_executable(${Profiling} ${Profiling_cpp})
target_link_libraries(${Profiling} PUBLIC
    ${MathLib}
)

# setup executable randomFloatNumberGenerator
add_executable(${RandomFloatNumberGenerator} ${RandomFloatNumberGenerator_cpp})

# setup executable tests
add_executable(${MathLibTests} ${Tests_cpp})
target_link_libraries(${MathLibTests} PUBLIC
    gtest_main
    ${MathLib}
)
add_test(
    NAME ${MathLibTests}
    COMMAND ${MathLibTests}
)


