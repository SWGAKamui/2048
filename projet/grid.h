#ifndef _GRILLE_H_
#define _GRILLE_H_
/**
 * \file grid.h
 * \brief Contains structures and functions needed to play 2048 game.
 **/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/** Grid dimension */
#define GRID_SIDE 4

/**
 * \brief Contain game's status: tiles and current score.
 *
 *		        X
 *               0  1    ... GRID_SIDE-1
 *		+--+--+- ... -+--+
 *		|  |  |  ...  +  | 0
 *		+--+--+- ... -+--+
 *		|  |  |  ...  |  | 1
 *      	+--+--+- ... -+--+         Y
 *		...      ...       ...
 *		+--+--+- ... -+--+
 *		|  |  |  ...  |  | GRID_SIDE-1
 *		+--+--+- ... -+--+
 */
typedef struct grid_s* grid;

/**
 * \brief Log_2-encoded tile : 0 is empty, i is 2**i.
 */
typedef unsigned int tile;


/**
 * \brief List of accepted movement in the game
 */
typedef enum dir_e {UP,LEFT,DOWN,RIGHT} dir;



/**
 * \brief Initialize grid structure
 * \return created an empty grid with score equal to 0
 */
grid new_grid ();

/**
 * \brief Destroy the grid and free allocated memory
 * \param g the grid to destroy
 */
void delete_grid (grid g);

/**
 * \brief Clone the grid
 * \param src the grid to copy
 * \param dst the copied grid
 */
void copy_grid (grid src, grid dst);


/**
 * \brief Get game's score
 * \param g the grid
 * \return the computed score during the game
 */
unsigned long int grid_score (grid g);

void reset (grid g);

/**
 * \brief Get tile  (log_2-encoded) from the grid by specifying his coordinates
 * \param g the grid
 * \param x and y tile's coordinates
 * \return the tile
 * \pre 0 <= x < GRID_SIDE and 0 <= y < GRID_SIDE
 */
tile get_tile (grid g, int x, int y);

/**
 * \brief Change tile's value
 * \param g the grid
 * \param x and y tile's coordinates
 * \param t new tile's value
 */
void set_tile (grid g, int x, int y, tile t);

/**
 * \brief Verify if a given movement is possible
 * \param g the grid
 * \param d movement's direction
 * \return true if the movement is possible, false else
 */
bool can_move (grid g, dir d);

/**
 * \brief Verify game's status, if no more movement is possible the game is over
 * \param g the grid
 * \return true if there is no more possible movements, false else
 */
bool game_over (grid g);

/**
 * \brief Move every tiles of the grid in the direction specified by the user
 * \param g the grid
 * \param d the chosen direction
 * \pre the movement d must be possible (i.e. can_move(g,d) == true).
 */
void do_move (grid g, dir d);

/**
 * \brief Randomly add a tile in the grid in a free space when a movement is finished.
 * With probability 9/10 the new tile has value 2 and with probability 1/10 the new tile has value 4.
 * \param g the grid
 * \pre grid g must contain  at least one empty tile.
 */
void add_tile (grid g);

/**
 * \brief Play a direction in the grid. 
 * \param g the grid
 * \param d the direction
 * \pre the movement d must be possible (i.e. can_move(g,d) == true).
 */
void play (grid g, dir d);


#endif
