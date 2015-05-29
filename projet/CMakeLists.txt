cmake_minimum_required (VERSION 2.6)
project ("2048")
add_definitions(-std=c99 -g -Wall -Werror)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2_REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2_image_REQUIRED sdl2_image)
PKG_SEARCH_MODULE(SDL2_ttf_REQUIRED SDL2_ttf)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} 
					${SDL2_IMAGE_INCLUDE_DIR} 
					${SDL2_TTF_INCLUDE_DIR})




include_directories(${2048_SOURCE_DIR})
include_directories(${2048_SOURCE_DIR}/tests)
include_directories(${2048_SOURCE_DIR}/strat)
enable_testing()

add_subdirectory(tests)
add_subdirectory(strat)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

add_library(grid ${2048_SOURCE_DIR}/grid.c)

add_executable(testgrid ${2048_SOURCE_DIR}/grid.h testgrid.c)
target_link_libraries(testgrid ncurses curses grid)

add_executable(testsdl ${2048_SOURCE_DIR}/grid.h testsdl.c)
target_link_libraries(testsdl grid ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARY}
  ${SDL2_TTF_LIBRARY} SDL2 SDL2_ttf)


