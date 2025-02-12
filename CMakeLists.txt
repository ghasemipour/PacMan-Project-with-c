cmake_minimum_required(VERSION 3.21)
project(PacmanEdition25 C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "D:/SDL_Lib/SDL2-2.30.10/x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "D:/SDL_Lib/SDL2_image-2.8.2/x86_64-w64-mingw32")
set(SDL2_TTF_PATH "D:/SDL_Lib/SDL2_ttf-2.22.0/x86_64-w64-mingw32")
set(SDL2_MIXER_PATH "D:/SDL_Lib/SDL2_mixer-2.8.0/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR})


add_executable(PacmanEdition25 main.c)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_MIXER_LIBRARY})



