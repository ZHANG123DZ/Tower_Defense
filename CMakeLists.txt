cmake_minimum_required(VERSION 3.15)
project(TowerDefense LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/core/Game.cpp
    src/core/Map.cpp
    src/core/GameState.cpp
    src/ui/Modal.cpp
    src/ui/Button.cpp
    src/route/Route.cpp
    src/pages/home/Home.cpp
    src/pages/Battlefields/Battlefields.cpp
    src/pages/Battle/Battle.cpp
    src/tower/TowerManager.cpp
    src/tower/Tower.cpp
    src/enemy/EnemyManager.cpp
    src/enemy/Enemy.cpp
    src/tower/Arrow.cpp
    src/tower/ArrowManager.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link đúng thư mục lib của MSYS2
link_directories("C:/msys64/ucrt64/lib")  # Hoặc mingw64/lib tuỳ bạn

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    mingw32
    SDL2main
    SDL2
    SDL2_image
    SDL2_ttf
    SDL2_mixer
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/SDL2.dll"
        "${CMAKE_SOURCE_DIR}/SDL2_image.dll"
        "${CMAKE_SOURCE_DIR}/SDL2_ttf.dll"
        "${CMAKE_SOURCE_DIR}/SDL2_mixer.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
