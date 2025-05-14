cmake_minimum_required(VERSION 3.16)
project(MyApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Dodaj katalog źródłowy
include_directories(src)

# Znajdź Catch2
find_package(Catch2 3 REQUIRED)

# Twoja biblioteka
add_subdirectory(src)



# Dodanie zewnętrznej biblioteki libftp.a
add_library(ftplib STATIC IMPORTED GLOBAL)
set_target_properties(ftplib PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/ftplib/src/libftp.a
)
target_include_directories(ftplib INTERFACE ${CMAKE_SOURCE_DIR}/ftplib/src)


# Główna aplikacja
add_executable(app app/main.cpp)
target_link_libraries(app PRIVATE ftplib client)

# Testy
add_executable(tests tests/test_client.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain client ftplib)

# Włączenie testów CTest + automatyczna rejestracja testów
enable_testing()
include(CTest)
include(Catch)
catch_discover_tests(tests)