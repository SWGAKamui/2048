
#include <grid.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int main (int argc, char *argv[]){

  	//init grille
 	grid g =new_grid();
  	// remplissage grille avec 0
 	for (int i = 0; i < GRID_SIDE ; i++){
    	for (int j = 0; j < GRID_SIDE ; j++){
    		set_tile(g,i,j,0);
    	}
    }

	set_tile(g,0,0,2);
	set_tile(g,0,1,4);
	set_tile(g,0,2,2);
	set_tile(g,0,3,4);
	
	set_tile(g,1,0,4);
	set_tile(g,1,1,2);
	set_tile(g,1,2,4);
	set_tile(g,1,3,2);

	set_tile(g,2,0,2);
	set_tile(g,2,1,4);
	set_tile(g,2,2,2);
	set_tile(g,2,3,4);

	set_tile(g,3,0,4);
	set_tile(g,3,1,2);
	set_tile(g,3,2,4);
	set_tile(g,3,3,2);



	if(!can_move(g,UP) && !can_move(g,DOWN) && !can_move(g,RIGHT) && !can_move(g,LEFT) && game_over(g)){
		return EXIT_SUCCESS;
	}
	else{
		return EXIT_FAILURE;
	}

	return 0;
}
