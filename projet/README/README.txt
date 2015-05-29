Compilation du projet:

Pour compiler l'exécutable et lancer les tests : 
"mkdir build"
"cd build"
"cmake .."
"make"

Pour lancer les tests (à faire dans le dossier build également):

"ctest"

Cette commande testera les différentes fonctions (sauf l'affichage qui doit être fait manuellement) : can_move, do_move et score.

L'executable du jeu se trouve dans le build

L'archive se crée après compilation du projet et se situe dans le dossier build
