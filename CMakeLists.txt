cmake_minimum_required(VERSION 3.15)
project(TowerDefense LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tạo executable
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/core/Game.cpp
    src/core/Map.cpp
)

# Include & lib
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib)

# Liên kết SDL2
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    mingw32
    SDL2main
    SDL2
    SDL2_image
)
