cmake_minimum_required(VERSION 2.8)

project(glstudy)

include_directories(include)


find_package(glfw3 3.3 REQUIRED)

file(GLOB SOURCES "./src/*.cpp")
set(GLAD_SOURCE "./src/glad.c")

add_executable(main ${SOURCES} ${GLAD_SOURCE})
target_link_libraries(main glfw ${CMAKE_DL_LIBS})

# Not sure if it makes sense to keep shaders separate from other assets
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_custom_target(copy_shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
)
add_dependencies(main copy_assets)
add_dependencies(main copy_shaders)
