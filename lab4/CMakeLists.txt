add_executable(lab4 main.c
    src/parent.c include/parent.h
)

target_include_directories(lab4 PRIVATE include)

target_link_libraries(lab4 PRIVATE Threads::Threads)