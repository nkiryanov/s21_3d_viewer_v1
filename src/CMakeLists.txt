cmake_minimum_required(VERSION 3.22)

project("3D_Viewer" VERSION 0.0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt specific settings because the project uses it
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Let's nicely support folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add custom cmake utils
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Set install prefix if this is the main app
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/../_install_dir/")

# Set app wide compiler options
add_compile_options(-Wall -Werror -Wextra)

# Docs only available if this is the main app
find_package(Doxygen)

if(Doxygen_FOUND)
    add_subdirectory(docs)
else()
    message(STATUS "Doxygen not found, not building docs")
endif()

# Testing only available if building in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(BUILD_TESTING True)

    add_compile_options(--coverage)
    add_link_options(--coverage)

    list(APPEND CMAKE_CTEST_ARGUMENTS "--rerun-failed" "--output-on-failure")

    enable_testing()
    add_subdirectory("tests")
endif(CMAKE_BUILD_TYPE STREQUAL Debug)

add_subdirectory("src/frontend")
add_subdirectory("src/backend")
add_subdirectory("app")
add_subdirectory("extern")
