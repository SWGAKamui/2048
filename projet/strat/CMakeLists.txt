add_executable(teststrat ${2048_SOURCE_DIR}/grid.h strategy.h strategy.c fast.c fast.h fast2.c fast2.h hard.h hard.c teststrat)
target_link_libraries(teststrat grid ncurses curses)

#add_test(Strat teststrat)

add_library(fast SHARED fast.c)
add_library(fast2 SHARED fast2.c)
add_library(hard SHARED hard.c)





