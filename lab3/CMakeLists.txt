add_executable(lab3 main.cpp include/lab3.h src/l3.cpp)

target_include_directories(lab3 PRIVATE include)

target_link_libraries(lab3 PRIVATE Threads::Threads)