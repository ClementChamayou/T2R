/*----------------------------------
*
*     Structure of the game's data
*
* ----------------------------------
* File : dataStruct.c
* Author : Clément Chamayou
* Date : 11 janvier 2021
*/

#ifndef _DATASTRUCT_H_
#define _DATASTRUCT_H_

#include "TicketToRideAPI.h"

typedef struct {
    int nbCities;
    int nbTracks;
    int * arrayTracks;
} t_board;

typedef struct {    
    int nbWagon;                    /* number of remaining wagons */
    char name[30];                  /* name of the player */
    int nbCards;                    /* number of cards at hand */
    int nbObjective;                /* number of objectives per player */
    int cards[10];                  /* contains the number of cards per color, cards[GREEN] is the number of green cards */
    int missingCards[10];           /* number of cards per color missing to complete a track */
    int doneObj;                    /* number of abjectives achieved */
    t_objective objectives[15];
    t_color startCards[4];          /* cards the player has at hand at the beginning */

}t_player;

typedef struct{
	int city1, city2;				/* id of the cities */
	int length;						/* length of the track */
	t_color color1, color2;			/* colors */
	int taken;						/* tell if the road is taken, 0 -> by nobody, 1-> by you, -1 by the opponent */
}t_track;

typedef struct{
    char name[20];                   /* name of the game*/
    int playing;                     /* player = 0 => we play, player => 1 the opponent plays*/
    int noPlayer;                    /* our number */
    int replay;                      /* tells whether the last player has to play again*/
    int lap;
    int trackLength[80];             /* array containing the length between a city and it's destination */
    int Prec[80];                    /* array of cities, allows to retrieve the path leading to an objective*/
    int lenPath;                     /* length of the array path[10] */
    int nbTracksNeeded;              /* number of tracks between two cities of an objective */
    t_track path[10];                /* routes leading to an objectives */
    t_track Tracks[80][80];          /* array of tracks */
    t_color faceUp[5];               /* cards facing up */
    t_return_code retCode;           /* tells whether the move was a winning one, a losing one or a normal one */
    t_color lastMove;                /* last card played, equals NONE if no need to replay*/
    t_drawObjectives drawObjectives;
    t_move move;                     /* current move */
    t_board board;                   /* game board */
    t_player player[2];              /* us and the opponent */
    
}t_game;

typedef struct{
    char * serverName;
    unsigned int port[3];
    char * botName;
}t_server;

/* check who wins and print a comment */
void endComment(t_game game);

/* Function creating the game */
void createGame(t_game * game);

/* Function displaying the objectives drawn*/
void displayObjective(t_objective objTemp[], int i);

/* Function that puts the objectives chosen in the player's hand*/
void fillObjectives(t_game * game,t_objective objTemp[3], int objectiveCards[3], int pos);

/* Function filling the different caracteristics of a player*/
void createPlayers(t_game * game);

/* Function adding a card to the player's desk*/
void addCard(t_player * player, t_color card);

/*Function removing a card from the player's deck */
void removeCard(t_player * player, t_color card, int nbLocomotives);

/* Function initialising the different tracks. The tracks that doesn't exist have a lenght of INT_MAX*/
void initTracks(t_track Tracks[80][80],int * arrayTracks,int nbTracks, int nbCities);

/* Gives the minimum distance between a departure city and the others*/
int minimumDistance(int D[80],int visited[50],int nbCities);

/* Function determining the shorter road to get to an objective */
int Dijkstra(int Prec[80],t_track Tracks[80][80],int src, int dest, int nbCities);

/* Function giving the roads we need to take in order to achieve an objective 
    Returns the length of the number of roads we need to take as well as those roads
*/
int givePath(int src, int dest, int nbTracksNeeded, t_track path[10], t_track Tracks[80][80],int Prec[80]);

/* Function uptdating the different variables of the game */
void updateGame(t_game * game);

/* Function that allows to check whether every parameter had been correctly filled or not */
void displayParam(t_game game);

/* Function displaying the parameters of the player in game */
void displayPlayersParam(t_game * game);
#endif