add_executable(lab2 main.c
    src/parent.c include/parent.h
    src/utils.c include/utils.h 
)

add_executable(child src/child.c
    src/utils.c include/utils.h
)

target_include_directories(lab2 PRIVATE include)

target_include_directories(child PRIVATE include)

add_dependencies(lab2 child)