cmake_minimum_required(VERSION 3.22.2)

project(hexapawn VERSION 0.1.1 LANGUAGES CXX)
include(cmake/general.cmake)
include(cmake/functions.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                                Conan                                 #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
include(cmake/conan.cmake)
conan_cmake_configure(
    BASIC_SETUP
    CMAKE_TARGETS
    REQUIRES fmt/9.1.0
    GENERATORS cmake_find_package
)
conan_cmake_autodetect(settings)
conan_cmake_install(
    PATH_OR_REFERENCE ${CMAKE_BINARY_DIR}
    GENERATOR cmake_find_package
    BUILD missing
    SETTINGS ${settings}
)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})

find_package(fmt REQUIRED)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                           Enable warnings                            #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
add_library(project_warnings INTERFACE)
include(cmake/compiler_warnings.cmake)
set_project_warnings(project_warnings)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                             perlin_noise                             #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
add_executable(hexapawn)
target_sources(hexapawn PRIVATE
        src/main.cpp
        src/engine.cpp
        )
target_compile_features(hexapawn PUBLIC cxx_std_20)
target_compile_options(hexapawn PRIVATE -ffast-math)
target_link_libraries(hexapawn
    PRIVATE
        fmt::fmt
        project_warnings
)
target_include_directories(hexapawn PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

enable_lto(hexapawn)
