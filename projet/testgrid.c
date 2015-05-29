#include "grid.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>
#include <time.h>

int HEIGHT = 25;
int WIDTH = 49;

void Remplire_Zero(grid g){
	reset(g);
	// remplissage grille avec 0
	for (int i = 0; i < GRID_SIDE ; i++){
		for (int j = 0; j < GRID_SIDE ; j++){
			set_tile(g,i,j,0);
		}
	}
	add_tile(g);
	add_tile(g);
}
//Affiche la grille
void display_windows(WINDOW * win){
  //Créer une "boite" de coordonnée 0,0
  box(win,0,0);
  //Pour chaque ligne
  for(int i=1; i<GRID_SIDE; ++i){
    //Bouger de pas en pas
    int step= HEIGHT/GRID_SIDE;
    //Créer une ligne horizontale jusqu'à longueur -2
    mvwhline(win, step*i,1,0,WIDTH-2);
  }

  //Pour chaque colonne
  for (int i=1; i<GRID_SIDE ; ++i){
    //Bouger de pas en pas
    int step= WIDTH/GRID_SIDE;
    //Créer une ligne horizontale jusqu'à largeur -2
    mvwvline(win,1,step*i,0,HEIGHT-2);
  }
}

void draw_at_XY(WINDOW * win, int c, int x, int y){
  //Position de la première variable axe x
  int pos0 = HEIGHT/GRID_SIDE;
  //Position de la première variable axe y
  int pos1 = WIDTH/GRID_SIDE;
  //Afficher la variable de la tile

  mvwprintw(win, (pos0/2)+(x*pos0), (pos1/2)+(y*pos1), "    ");
  if(c!=0){
    mvwprintw(win, (pos0/2)+(x*pos0), (pos1/2)+(y*pos1), "%d", c);
  }
}

void draw_at_pos(WINDOW * win, int pos, grid g){
  //Calcul les coordonées par rapport à la position dans la grille
  int y = pos/GRID_SIDE;
  int x= pos-(GRID_SIDE*y);
  draw_at_XY(win, get_tile(g, x, y), x, y);
}

void draw_data(WINDOW * win,grid g){
  for (int i = 0; i < GRID_SIDE*GRID_SIDE; i++){
    //Pour chaque position dans la  grille (0 à 15) mettre la variable qui est dans tile[position]
    draw_at_pos(win, i, g);
  }
}



int main (void){
  	srand((unsigned int)time(NULL));
	// C sert a recuperer la version ascii des inputs
	int c=0;
	//initialise l'ecran
	initscr();
	//curs_set met le curseur en haut à gauche de l'ecran pour ensuite placer l'ecran a partir de ce point.
	curs_set(0);
	//noecho n'affiche pas les inputs exemple fleches du haut "^[[A"
	noecho();
	//permet de recup les touches clavier
	keypad(stdscr, TRUE);
	//reinitialise le terminal pour recevoir la nouvelle fenetre
	refresh();
	//taille win
	//offsetx initialise la dimension de depart niveau colonne
	int offset_x = (COLS - WIDTH)/2;
	//offsety initialise la dimension de depart niveau ligne
	int offset_y = (LINES- HEIGHT)/2;
	// ncurses prend les parametres x et y en inverse
	//on place la nouvelle fenetre avec la taille HEIGHT sur WIDTH a partir de offset_x et offset_y
	WINDOW * win = newwin(HEIGHT,WIDTH,offset_y,offset_x);
	wmove(win, 0,0);

	
	//init grille 	grid g =new_grid();
	grid g =new_grid();
	// remplissage grille avec 0 et deux tiles aléatoire
	Remplire_Zero(g);
	
	display_windows(win);
	draw_data(win,g);
	wrefresh(win);
	

	mvprintw(offset_y- 4, offset_x, "[Score :      ] " );
	mvprintw(offset_y-3, offset_x, "Pour recommencer une partie, appuyez sur r");
	mvprintw(offset_y-2, offset_x, "Pour quitter le jeu, appuyez sur q");


	// Tant que la partie n'est pas fini, attendre input
	while((c=getch())!= 'q') {
	  // afficher grille (les refresh permettent de réafficher la grille)
	  // Differente execution en fonction de la valeur
	  // Si l'ascii est équivalent à l'input gauche
	  // le switch case ne fonctionne pas avec ncurses, on choisit donc d'utiliser des if
		if(c==KEY_LEFT){
			if (can_move(g,LEFT)){
				play(g,LEFT);
				draw_data(win,g);
				wrefresh(win);
				
			}
			if (game_over(g)){

				mvprintw(offset_y-6, offset_x+20, "GAME OVER");
				draw_data(win,g);
				wrefresh(win);
				
			}
		}
		// Si l'ascii est équivalent à l'input droite
		else if(c==KEY_RIGHT){
			if (can_move(g,RIGHT)){
				play(g,RIGHT);
				draw_data(win,g);
				wrefresh(win);
				
			}
			if (game_over(g)){
				mvprintw(offset_y-6, offset_x+20, "GAME OVER");
				draw_data(win,g);
				wrefresh(win);
				

			}
		}
		// Si l'ascii est équivalent à l'input haut
		else if (c==KEY_UP){
			if (can_move(g,UP)){
				play(g,UP);
				draw_data(win,g);
				wrefresh(win);
				
			}
			if (game_over(g)){
				mvprintw(offset_y-6, offset_x+20, "GAME OVER");
				draw_data(win,g);
				wrefresh(win);
				
			}
		}
		// Si l'ascii est équivalent à l'input bas
		else if(c==KEY_DOWN){
			if (can_move(g,DOWN)){
				play(g,DOWN);
				draw_data(win,g);
				wrefresh(win);
				
			}
			if (game_over(g)){
				mvprintw(offset_y-6, offset_x+20, "GAME OVER");
				draw_data(win,g);
				wrefresh(win);
				
			}
		}
		else if (c=='r'){
		  refresh();
		  wrefresh(win);
		  
		  for (int i = 0; i < GRID_SIDE ; i++){
		    for (int j = 0; j < GRID_SIDE ; j++){
		      set_tile(g,i,j,0);
		    }
		  }
		  Remplire_Zero(g);
		  mvprintw(offset_y- 4, offset_x, "[Score :      ] " );
		  mvprintw(offset_y-6, offset_x+20, "          ");
		  //remettre le score a zero
		  draw_data(win,g);
		  wrefresh(win);
		}
		mvprintw(offset_y- 4, offset_x, "[Score : %5d]", grid_score(g));
		mvprintw(offset_y- 3, offset_x, "Pour recommencer une partie, appuyez sur r");

	}

	endwin ();

	delwin(win);
	delete_grid(g);
	return 0;  
}
