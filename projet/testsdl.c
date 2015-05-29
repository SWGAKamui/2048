#include <grid.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

//Init variable globale

int sizex = 100*GRID_SIDE;
int sizey = 100*GRID_SIDE;
int sizeZ = 500;
SDL_Color couleur = {255, 255, 255};

//Init fonction qui se situe après le main
void drawGrid(SDL_Renderer * renderer);
void Remplire_Zero(grid g);
void cleanSDL(SDL_Window *window, SDL_Renderer * renderer);
void drawData(SDL_Renderer * renderer, SDL_Texture * texture,SDL_Surface * ecran, TTF_Font * police, grid g);
void drawScore(SDL_Renderer * renderer, SDL_Texture * texture,SDL_Surface * ecran, TTF_Font * police, grid g);
void drawOver(SDL_Renderer * renderer, SDL_Texture * texture,SDL_Surface * ecran, TTF_Font * police);


int main (void){
  //init le random en fonction de l'heure actuelle
  srand((unsigned int)time(NULL));
  //init fenetre
  SDL_Event  event;
  SDL_Init(SDL_INIT_EVERYTHING);
  //taille win (sizex+sizeZ, sizey) pour laisser de l'espace pour le score
  SDL_Window * window = SDL_CreateWindow("2048", SDL_WINDOWPOS_CENTERED,
					 SDL_WINDOWPOS_CENTERED, sizex+sizeZ,sizey, SDL_WINDOW_SHOWN);
	
  SDL_Renderer* renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_PRESENTVSYNC);
  //255 = blanc
  SDL_SetRenderDrawColor(renderer, 255,255,255,255);

  //====================================================
  TTF_Init();
  SDL_Surface  *ecran = NULL;
    
  ecran=SDL_GetWindowSurface(window);
  TTF_Font *police = NULL;
  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,ecran);
  /* Chargement de la police */
  police = TTF_OpenFont("../DejaVuSans.ttf", 65);
   
  //====================================================

  //init grille 	
  grid g =new_grid();
  Remplire_Zero(g);

  bool continuer = true;

  while (continuer == true){
    while( SDL_PollEvent( &event ) ){			
      switch(event.type)
	{
	  //si on clique sur le bouton "fermer" de la fenetre
	case SDL_QUIT:
	  continuer = false;
	case SDL_KEYDOWN:
	  switch (event.key.keysym.sym)
	    {
	    case SDLK_q: // Appui sur la touche q, on arrête le programme 
	      continuer = false;
	      break;
	    case SDLK_r: // Appui sur la touche r, on rejoue
	      //reinit fenetre
	      SDL_SetRenderDrawColor(renderer, 0,0,0,255);
	      //efface la fenetre
	      SDL_RenderClear(renderer);
	      //reinit grille
	      Remplire_Zero(g);
	      SDL_SetRenderDrawColor(renderer, 255,255,255,255);
	      //affiche grille
	      SDL_RenderPresent(renderer);
	      break;		   
	    case SDLK_LEFT:
	      if (can_move(g,LEFT)){
		play(g,LEFT);	
	      }

	      break;
	    case SDLK_RIGHT:
	      if (can_move(g,RIGHT)){
		play(g,RIGHT);	
	      }
	      if (game_over(g)){
								
	      }
	      break;
	    case SDLK_UP:
	      if (can_move(g,UP)){
		play(g,UP);	
	      }
	      if (game_over(g)){
								
	      }
	      break;
	    case SDLK_DOWN:
	      if (can_move(g,DOWN)){
		play(g,DOWN);	
	      }
	      if (game_over(g)){
								
	      }
	      break;
	    default:
	      break;
	    }
	default:
	  break;
	}
    }
    //toujours afficher entre renderclear et renderpresent
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    /*=================Affichage info de base====================================*/
    drawData(renderer,texture,ecran, police, g);
    drawScore(renderer,texture,ecran,police,g);
    drawGrid(renderer);
    if (game_over(g)){
      drawOver(renderer, texture,ecran, police);
    }
    /*======================Rendu===============================*/
    SDL_RenderPresent(renderer);

  }
  //liberation mémoire
  delete_grid(g); 
  SDL_FreeSurface(ecran);
  TTF_CloseFont(police);
  SDL_DestroyTexture(texture);
  SDL_SetRenderDrawColor(renderer, 0,0,0,255);
  SDL_RenderClear(renderer);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);	
  SDL_Quit();

  return 0;  
}

void drawScore(SDL_Renderer * renderer, SDL_Texture * texture,SDL_Surface * ecran, TTF_Font * police, grid g){
  int tailleCase = sizey/GRID_SIDE;
  SDL_Surface * texte=NULL;
  // on prend une variable assez grande pour laisser de la place pour le score
  size_t taillechar=20;
  char score[taillechar];
  char messageQ[taillechar];
  char messageR[taillechar];
  SDL_Rect position;
  position.x = sizex +sizeZ/10;
  position.y = 10;
  
  
  //=======================SCORE==============================
	
  snprintf(score, taillechar,"score[%ld]", grid_score(g));
  texte = TTF_RenderText_Solid(police, score, couleur);
  //position prédéfinie en fonction de GRID_SIDE
  position.h=texte->h;
  position.w=texte->w;
  position.y=10;
  SDL_Texture * txt =  SDL_CreateTextureFromSurface(renderer,texte);
  SDL_RenderCopy(renderer,texture, NULL,&position); 
  SDL_RenderCopy(renderer,txt,NULL, &position);
  SDL_DestroyTexture(txt);
  SDL_FreeSurface(texte);
  //=====================INPUT R==============================
  SDL_Surface * texteQ=NULL;
  snprintf(messageQ, taillechar,"Quitter q");
  texteQ = TTF_RenderText_Solid(police, messageQ, couleur);
  position.h=texteQ->h;
  position.w=texteQ->w;
  position.y=tailleCase;
  SDL_Texture * txtQ =  SDL_CreateTextureFromSurface(renderer,texteQ);
  SDL_RenderCopy(renderer,texture, NULL,&position); 
  SDL_RenderCopy(renderer,txtQ,NULL, &position);
  SDL_DestroyTexture(txtQ);
  SDL_FreeSurface(texteQ);
  //=====================INPUT Q==============================
  SDL_Surface * texteR=NULL;
  snprintf(messageR, taillechar,"Reset r");
  texteR = TTF_RenderText_Solid(police, messageR, couleur);
  position.h=texteR->h;
  position.w=texteR->w;
  position.y=tailleCase*2;
  SDL_Texture * txtR =  SDL_CreateTextureFromSurface(renderer,texteR);
  SDL_RenderCopy(renderer,texture, NULL,&position); 
  SDL_RenderCopy(renderer,txtR,NULL, &position);
  SDL_DestroyTexture(txtR);
  SDL_FreeSurface(texteR);
}

void drawOver(SDL_Renderer * renderer, SDL_Texture * texture,SDL_Surface * ecran, TTF_Font * police){
  SDL_Surface * texte=NULL;
  size_t taillechar=20;
  char gameOver [taillechar];
  SDL_Rect position;
  position.x = 10;
  //=====================Game_Over==============================
  SDL_Color couleur = {198, 8, 0};
  snprintf(gameOver, taillechar,"Game_Over");
  texte = TTF_RenderText_Solid(police, gameOver, couleur);
  position.h=texte->h;
  position.w=texte->w;
  position.y=sizey/2-40;
  SDL_Texture * txt =  SDL_CreateTextureFromSurface(renderer,texte);
  SDL_RenderCopy(renderer,texture, NULL,&position); 
  SDL_RenderCopy(renderer,txt,NULL, &position);	    
  SDL_DestroyTexture(txt);
  SDL_FreeSurface(texte);
}
void drawData(SDL_Renderer * renderer, SDL_Texture * texture,SDL_Surface * ecran, TTF_Font * police, grid g){
  SDL_SetRenderDrawColor(renderer, 255,255,255,255);
  size_t taillechar=15;
  char c [taillechar];
  int tailleCase = sizey/GRID_SIDE;
  SDL_Rect position;
  


  for (int i = 0; i<GRID_SIDE;i++){
    for(int j = 0; j<GRID_SIDE;j++){
      SDL_Surface * texte=NULL;
      snprintf(c,taillechar, " ");
      /*=====================================================*/
      if (get_tile(g, j, i)!=0){
	snprintf(c,taillechar, "%d", get_tile(g, j, i));
      }
      texte = TTF_RenderText_Solid(police, c, couleur);
      SDL_Texture * txt =  SDL_CreateTextureFromSurface(renderer,texte);
      /*=====================================================*/
      if(texte->h>texte->w){
	position.w=(texte->w*148)/texte->h;
	position.h=tailleCase;
      }
      if(texte->h<=texte->w){
	position.w=tailleCase;
	position.h=(texte->h*tailleCase)/texte->w;
      }
      position.x = (tailleCase-position.w)/2+2+i*tailleCase;
      position.y = (tailleCase-position.h)/2+2+j*tailleCase;
      /*=====================================================*/
      SDL_RenderCopy(renderer,texture, NULL,&position); 
      SDL_RenderCopy(renderer,txt,NULL, &position);
      SDL_DestroyTexture(txt);
      SDL_FreeSurface(texte);
    }
  }
}

void drawGrid(SDL_Renderer * renderer){
  SDL_SetRenderDrawColor(renderer, 255,255,255,255);
  int step=0;
  int tailleCase = sizex/GRID_SIDE;
  /*==========================HORIZONTAL================================*/
  for (int i = 0; i<= GRID_SIDE; i++){
    SDL_RenderDrawLine(renderer, 0, step, sizex, step);
    SDL_RenderDrawLine(renderer, 0, step+1, sizex, step+1);
    SDL_RenderDrawLine(renderer, 0, step+1, sizex, step+1);
    step+=sizex/GRID_SIDE;
    if(i==GRID_SIDE){
      step-=tailleCase;
      SDL_RenderDrawLine(renderer, 0, step, sizex, step);
      SDL_RenderDrawLine(renderer, 0, step-1, sizex, step-1);
      SDL_RenderDrawLine(renderer, 0, step-1, sizex, step-1);
    }
  }
  /*==========================VERTICAL================================*/
  step=0;
  for (int i = 0; i<= GRID_SIDE; i++){
    SDL_RenderDrawLine(renderer, step, 0, step, sizey);
    SDL_RenderDrawLine(renderer, step+1, 0, step+1, sizey);
    SDL_RenderDrawLine(renderer, step+1, 0, step+1, sizey);
    step+=sizex/GRID_SIDE;
    if(i==GRID_SIDE){
      step-=tailleCase;
      SDL_RenderDrawLine(renderer, step, 0, step, sizey);
      SDL_RenderDrawLine(renderer, step-1, 0, step-1, sizey);
      SDL_RenderDrawLine(renderer, step-1, 0, step-1, sizey);
    }
  }
}


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
