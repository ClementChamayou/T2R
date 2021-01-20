//
// Created by Thib on 14/12/2020.
//

#ifndef T2R_TEST_MOVE_H
#define T2R_TEST_MOVE_H

#include "TicketToRideAPI.h"
#include "game.h"

/* ask for a move */
void askMove(t_move * move);

/* plays the move given as a parameter (send to the server)
 * returns the return code */
t_return_code playOurMove(t_move* move, t_color* lastCard, t_drawObjectives * drawObj);

/* tell if we need to replay */
int needReplay(t_move* move, t_color lastCard);

/* Function that automatically plays a move */
void chooseMove(t_track Tracks[80][80],t_track path[10],int lenPath, t_player * player, t_move * move, t_color faceUp[5], int doneObj, int * nbObjectives, int lap, t_drawObjectives drawObjective);

#endif
