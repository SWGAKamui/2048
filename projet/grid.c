
#include "grid.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// définition des fonctions qui n'existent pas dans le .h
void exchange_value(grid g, int y, int x, int y1, int x1);
int free_case(grid g, int y, int x, dir d);
void fusion_case(grid g, int y, int x, int y1, int x1);
void change_tile(grid g, int y, int x, dir d);

// implémentation des fonctions

struct grid_s{
  /* On définit la structure de grid qui contient un tableau
     d'entiers "tiles" et le score */
  int * tiles;
  unsigned long score;
  /* Pour simplifier la recherche de tuile vide, on crée un 
     tableau qui va contenir tout les indices des tuiles vides.
     Le tableau est de la taille de la grille et on met à jour la
     taille pour "supprimer" des cases. */
  /* tableau d'indice des tuiles vides */
  int * free_tiles;
  /* nombre de tuiles vide */
  int free_length;
};


grid new_grid (){
  /* On alloue de la mémoire avec deux arguments:
     - Le nombre d'objets
     - Nombre d'octets (sizeof)
     cela pointe vers un tableaux d'objets.
  */

  //la grille est initialisee en commençant par la première colonne.
  /*
    | 0| 4| 8|12|
    | 1| 5| 9|13|
    | 2| 6|10|14|
    | 3| 7|11|15|
  */
  grid g=calloc (1,sizeof(*g));
  g->tiles=calloc(GRID_SIDE*GRID_SIDE, sizeof (tile));
  g->tiles[0]=0;
  g->score=0;
  /* On alloue de la mémoire vers le tableau et à t0 on a bien toutes les tuiles = 0 */
  g->free_tiles=calloc(GRID_SIDE*GRID_SIDE, sizeof(tile));
  /* On met tout les indices des cases dans le tableau free_tiles */
  for (int i=0; i<GRID_SIDE*GRID_SIDE; ++i){
    g->free_tiles[i]=i;
  }
  g->free_length= GRID_SIDE*GRID_SIDE;
  return g;
}


void delete_grid (grid g){
  /*Pour chaque allocation effectuée,
    il faut le free lui correspondant*/
  free(g->free_tiles);
  free(g->tiles);
  free(g);
  /* avec valgrind on obtient beaucoup plus d'allocation que de free,
     mais aucune fuite de mémoire : c'est à cause de la bibliothèque graphique sdl */
}

void copy_grid (grid src, grid dst){
  /*Copie d'une grille vers une autre grille déjà allouée  :
    - on copies les valeurs de chaque case
    - on copie le score
    - on copie le tableau des cases vides et sa taille*/
  for (int i=0; i<= GRID_SIDE*GRID_SIDE;i++){
    dst->tiles[i]=src->tiles[i];
  }
  dst->score = src->score;
  
  int cpt=0;
  for(int i=0; i<GRID_SIDE*GRID_SIDE; ++i){
    if(src->tiles[i]==0){
      dst->free_tiles[cpt]=i;
      cpt++;
    }
  }
  dst->free_length=cpt;
}

/*Pour get_tile et set_tile, nous avons fait le choix d'utiliser un tableau à une dimension
  pour nous simplifier la tâche vis à vis des allocations.
  Avec la formule x + ( GRID_SIDE * y ) on prend la ligne x et on l'additionne a GRID_SIDE
  x la ligne y ; ainsi on récupère le numéro de case dans le tableau de la tuile x et y 
  vont de 0 à GRID_SIDE-1 */


tile get_tile (grid g, int x, int y){
  /*Cette fonction est la seule manière de récupérer les valeurs de chaques tuiles pour 
    qu'elles soient visible par d'autres fonctions*/
  return g->tiles[x+(GRID_SIDE*y)];
}



void set_tile (grid g, int x, int y, tile t){
  /*Cette fonction est la seule manière de définir les valeurs de chaque tuile */
  g->tiles[x+(GRID_SIDE*y)]=t;
}


bool can_move (grid g,dir d){

if (d==UP){
	for(int i = 0; i < GRID_SIDE; i++){
		for(int j = 0; j <GRID_SIDE-1; j++){
			//retourne true si il y alternance entre 0 et une autre valeur
			if(get_tile(g, i, j) == 0 && get_tile(g, i, j+1) != 0)
				return true;
			//retourne true si la fusion est possible entre deux tuiles
			if(get_tile(g, i, j) != 0 && get_tile(g, i, j) == get_tile(g, i, j+1))
				return true;

		}
	}
return false;
}

if(d==DOWN){
	for(int i = 0; i < GRID_SIDE; i++){
		for(int j = GRID_SIDE-1; j >0; j--){
			//retourne true si il y alternance entre 0 et une autre valeur
			if(get_tile(g, i, j) == 0 && get_tile(g, i, j-1) != 0)
				return true;
			//retourne true si la fusion est possible entre deux tuiles
			if(get_tile(g, i, j) != 0 && get_tile(g, i, j) == get_tile(g, i , j-1))
				return true;
		}
	}
return false;
 }

if (d==RIGHT){
	for(int i = 0; i < GRID_SIDE; i++){
		for(int j = GRID_SIDE-1; j > 0; j--){
		//retourne true si il y alternance entre 0 et une autre valeur
			if(get_tile(g, i, j) == 0 && get_tile(g, i, j-1) != 0)
				return true;
		//retourne true si la fusion est possible entre deux tuiles
			if(get_tile(g, i, j) != 0 && get_tile(g, i, j) == get_tile(g, i, j-1))
				return true;
		}
	}
return false;
 }

if (d==LEFT){
	for(int i = GRID_SIDE-1; i >=0; i--){
		for(int j = 0; j < GRID_SIDE-1; j++){
			//retourne true si il y alternance entre 0 et une autre valeur
			if((get_tile(g,i,j) == 0) && (get_tile(g, i, j+1) != 0))
				return true;
			// retourne true si la fusion est possible entre deux tuiles
			if(get_tile(g, i, j) != 0 && get_tile(g, i, j) == get_tile(g, i, j+1))
				return true;
		}
	}
return false;
 }
return false;
}



bool game_over (grid g){
  if(!can_move(g, UP) && !can_move(g, LEFT) && !can_move(g, DOWN) && !can_move(g, RIGHT)){
    return true; // aucun mouvement n'est possible : le jeu est fini
  }
  return false;
}


unsigned long int grid_score (grid g){
  //retourne le score
  return g->score;
}

void reset (grid g){
  //met le compteur score a 0 lors d'un reset de la grille
  g->score=0;
  int cpt=0;
  for(int i=0; i<GRID_SIDE*GRID_SIDE; ++i){
    if(g->tiles[i]==0){
      g->free_tiles[cpt]=i;
      cpt++;
    }
  }
  g->free_length = GRID_SIDE*GRID_SIDE -2;
}


/*Les fonctions exchange_value, free_case, fusion_case, change_tile et non_zero_search ont
  été rajoutées afin d'optimiser le do_move */

//echange les valeurs entre deux cases (fonction appelée s'il y a alternance entre 0 et une autre valeur)
void exchange_value(grid g, int y, int x, int y1, int x1){
  int tmp = get_tile(g, y, x);
  set_tile(g, y, x, get_tile(g, y1, x1));
  set_tile(g, y1, x1, tmp); 
}


//recherche les cases vides de la grille après le mouvement pour savoir où rajouter une nouvelle valeur
int free_case(grid g, int y, int x, dir d){
  int minimum = 100; int maximum = -1;
  if(d == LEFT || d == RIGHT){
    for(int x = 0; x < GRID_SIDE; x++){
      if(get_tile(g, y, x) == 0){
	maximum = x;
	if(minimum == 100)
	  minimum = x;
      } 
    }
  }
  else{
    for(int y = 0; y < GRID_SIDE; y++){
      if(get_tile(g, y, x) == 0){
	maximum = y;
	if(minimum == 100)
	  minimum = y;
      }
    }
  }
  if(d == LEFT || d == UP)
    return minimum;
  else
    return maximum;
}


/* Fusionne les cases qui ont la meme valeur : 
   - valeur d'une case doublée
   - valeur de l'autre case mise à zero
   - mise à jour du score en ajoutant la valeur de la fusion */
void fusion_case(grid g, int y, int x, int y1, int x1){
  if(get_tile(g, y, x) == get_tile(g, y1, x1) && (y != y1 || x != x1)){
    set_tile(g, y, x, get_tile(g, y, x) *2); 
    set_tile(g, y1, x1, 0);
    g->score += get_tile(g, y, x);
  }
}

/* Fait une recherche des tuiles contenant des valeurs différentes de zéro dans 
   le but de les fusionner ou de les décaler selon  */
int non_zero_search(grid g, int y, int x, dir d){
  switch(d){
  case LEFT:
    if(x != GRID_SIDE-1)
      x++;
    while((x < GRID_SIDE) && (get_tile(g, y, x) == 0)){
      x++;
    }
    return x;
    break;
  case RIGHT:
    if(x != 0)
      x--;
    while((x >=  0) && (get_tile(g, y, x) == 0)){
      x--;
    }
    return x;
    break;
  case DOWN:
    if(y != 0)
      y--;
    while((y >=  0) && (get_tile(g, y, x) == 0)){
      y--;
    }
    return y;
    break;
  case UP:
    if(y != GRID_SIDE-1)
      y++;
    while((y < GRID_SIDE) && (get_tile(g, y, x) == 0)){
      y++;
    }
    return y;
    break;
  }
  return EXIT_FAILURE;
}


/* cherche s'il sagit du'un tuile vide et s'il faut fusionner et/ou déplacer la tuile*/
void change_tile(grid g, int y, int x, dir d){
  // le if ne fait rien si la valeur de la tuile est égale à 0.
  if(get_tile(g, y, x) != 0){
    int r = non_zero_search(g, y, x, d);
    // ce if verifie si l'indice trouve est dans l'intervalle {0,..,GRID_SIDE}
    if(r < GRID_SIDE && r >= 0){
      // ce if fait des fusions par rapport au mouvement souhaité
      if(d == LEFT || d == RIGHT)
	fusion_case(g, y, x, y, r);
      else
	fusion_case(g, y, x, r, x); 
    }
    r = free_case(g, y, x, d);
    // ce if fait des echanges entre les valeurs de deux tuiles s'il le faut 
    if((r < x && d == LEFT) || (r > x && d == RIGHT))
      exchange_value(g, y, x, y, r);
    else if((r > y && d == DOWN) || (r < y && d == UP))
      exchange_value(g, y, x, r, x); 
  }
}



// appelle change_tile pour chaque case selon la direction demandée
void do_move (grid g, dir d){
  switch (d){
  case LEFT:
    for(int y = 0; y < GRID_SIDE; y++){
      for(int x = 0; x < GRID_SIDE; x++){
	change_tile(g, y, x, d);
      }
    }
    break;
  case RIGHT:
    for(int y = 0; y < GRID_SIDE; y++){
      for(int x = GRID_SIDE-1; x >= 0; x--){
	change_tile(g, y, x, d);
      }
    }
    break;
  case DOWN:
    for(int x = 0; x < GRID_SIDE; x++){
      for(int y = GRID_SIDE - 1; y >= 0; y--){
	change_tile(g, y, x, d); 
      }
    }
    break;
  case UP:
    for(int x = 0; x < GRID_SIDE; x++){
      for(int y = 0; y < GRID_SIDE; y++){
	change_tile(g, y, x, d);
      }
    }
    break;
  }
}

// on ajoute une tuile et on met à jour la liste des tuiles vides ainsi que sa taille
void add_tile (grid g){
  // choix de la tuile libre aleatoirement a l'aide de rand()
  int free = rand()%(g->free_length);
  int indice_free = g->free_tiles[free];
  //On decale les indices des cases libres à partir l'indice de la case choisie pour "l'effacer" du tableau
  for (int i =free; i<(g->free_length-1); ++i){
    g->free_tiles[i]=g->free_tiles[i+1];
  }
  g->free_length--;
  
  //Choix de 2 ou de 4 : on choisit aléatoirement un nombre entre 0 et 10, si 9 (nb choisi arbitrairement) est choisi, on ajoutera un 4 (probabilité de 1/10), sinon on ajoutera un 2 à la grille
  int new_tile = rand()%10;
  if(new_tile == 9){
    new_tile = 4;
  }
  else if (new_tile!=9){
    new_tile = 2;
  }
  g->tiles[indice_free]=new_tile;
}

// fonction qui centralise toutes les fonctions pour chaque mouvement choisi.
void play (grid g, dir d){
  do_move(g,d);
  //On met à jour la liste free_tiles :on cherche tout les 0 de la grille pour mettre les indices des cases correspondante dedans.
  int cpt=0;
  for(int i=0; i<GRID_SIDE*GRID_SIDE; ++i){
    if(g->tiles[i]==0){
      g->free_tiles[cpt]=i;
      cpt++;
    }
  }
  //On sauvegarde la nouvelle "taille" de la grille, pour ne pas déborder sur d'anciennes valeurs
  g->free_length=cpt;
  // s'il y a encore de la place dans la grille, on ajoute une nouvelle valeur
  if(g->free_length!=0){
    add_tile(g);     
  }

}




