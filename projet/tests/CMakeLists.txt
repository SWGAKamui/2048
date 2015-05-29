add_executable(testgrid1 ${2048_SOURCE_DIR}/grid.h testgrid1.c)
target_link_libraries(testgrid1 grid)

add_executable(testgrid2 ${2048_SOURCE_DIR}/grid.h testgrid2.c)
target_link_libraries(testgrid2 grid)

add_executable(testgrid3 ${2048_SOURCE_DIR}/grid.h testgrid3.c)
target_link_libraries(testgrid3 grid)

add_test(testcan_move testgrid1)
add_test(testdo_moveUPLEFTscore testgrid2)
add_test(testdo_moveDOWNRIGHTscore testgrid3)