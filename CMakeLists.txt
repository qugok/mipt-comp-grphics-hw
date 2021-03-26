cmake_minimum_required (VERSION 3.0)

project (CG_Hometasks)

find_package(OpenGL REQUIRED)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130
        )

add_subdirectory (external)

include_directories(
        external/AntTweakBar-1.16/include/
        external/glfw-3.1.2/include/
        external/glm-0.9.7.1/
        external/glew-1.13.0/include/
        external/assimp-3.0.1270/include/
        external/bullet-2.81-rev2613/src/
        practice
        common
)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

# task 2
add_executable(task2
        practice/2/task.cpp
        common/shader.cpp
        common/shader.hpp
        )
target_link_libraries(task2
        ${ALL_LIBS}
        )

# task 3
add_executable(task3
        practice/3/task.cpp
        common/shader.cpp
        common/shader.hpp
        )
target_link_libraries(task3
        ${ALL_LIBS}
        )

# task 4
add_executable(task4
        practice/4/task.cpp
        common/shader.cpp
        common/shader.hpp
        )
target_link_libraries(task4
        ${ALL_LIBS}
        )


## test
#add_executable(test
#        practice/test/test.cpp
#        common/shader.cpp
#        common/shader.hpp
#        )
#target_link_libraries(test
#        ${ALL_LIBS}
#        )


