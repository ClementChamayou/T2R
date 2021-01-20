/*---------------------------------------------
*
*     Functions added to play to the game
*
* ---------------------------------------------
* File : game.c
* Author : Clément Chamayou
* Date : 16 janvier 2021
*/

#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "TicketToRideAPI.h"
#include <string.h>

/* check who wins and print a comment */
void endComment(t_game game){
    
	if ((game.retCode == WINNING_MOVE && game.playing == 0) || (game.retCode == LOOSING_MOVE && game.playing == 1))
		printf("We are the best !\n");
	else
		printf("The opponent was lucky...\n");

}

/* Function creating the game */
void createGame(t_game * game){

	waitForT2RGame("TRAINING DO_NOTHING timeout=100 start=0 map=USA",game->name,&game->board.nbCities,&game->board.nbTracks);
    game->board.arrayTracks = malloc(5*(game->board.nbTracks)*sizeof(int));
    
    game->playing = getMap(game->board.arrayTracks,game->faceUp,game->player[0].startCards);

    game->lastMove = NONE;
    game->replay = 0;
    game->noPlayer = 0;
    game->lap = 0;
}

/* Function displaying the objectives drawn*/
void displayObjective(t_objective objTemp[],int i){

   /* printf("Objective %d :\n",i+1);
    printf("  City 1 : %d\n",objTemp[i].city1);
    printf("  City 2 : %d\n",objTemp[i].city2);
    printf("  Score : %d\n",objTemp[i].score);
  */
    printf("Objectif %d.\n   %d (", i+1, objTemp[i].city1);
    printCity(objTemp[i].city1);
    printf(") -> (");
    printCity(objTemp[i].city2);
    printf(") %d (%d pts)\n", objTemp[i].city2, objTemp[i].score);


}

/* Function that puts the objectives chosen in the player's hand
   t_objective objTemp[3] : drawn objectives
   int objectiveCards[3] : array telling whether an objective is choosen or not
   int pos : position of the last objective card
*/
void fillObjectives(t_game * game,t_objective objTemp[3], int objectiveCards[3],int pos){

   int offset = 0;

   for (int i=pos; i<pos +3; i++){

       if (objectiveCards[i-pos] != 0){
           game->player[0].objectives[i - offset] = objTemp[i-pos];
       }
       else{
           offset += 1;
       }
       
   }

}

/* Function filling the different caracteristics of a player*/
void createPlayers(t_game * game){

    /* initialising our player */
	game->player[0].nbWagon = 45;

	game->player[0].nbCards = 0;

    game->player[0].nbObjective = 0;

    game->player[0].doneObj = 0;

    /*for (int i=0; i<15; i++){
        game->player[0].objectives[i].city1 = 0;
        game->player[0].objectives[i].city2 = 0;
        game->player[0].objectives[i].score = 0;
    }*/

    for (int i=0; i<10; i++){
        game->player[0].cards[i] = 0;
        game->player[0].missingCards[i] = 0;

    }
    for (int j=0; j<4; j++){
        addCard(&game->player[0],game->player[0].startCards[j]);
    }

    /* initialising the opponent */

    game->player[1].nbWagon = 45;

	game->player[1].nbCards = 4;

    game->player[1].nbObjective = 0;

    for (int i=0; i<10; i++){
        game->player[1].cards[i] = 0;
    }
}

/*Function adding a card to the player's deck */
void addCard(t_player * player, t_color card){

    switch (card){
        case PURPLE:
            player->cards[0] += 1;
            break;
        case WHITE:
            player->cards[1] += 1;
            break;
        case BLUE:
            player->cards[2] += 1;
            break;
        case YELLOW:
            player->cards[3] += 1;
            break;
        case ORANGE:
            player->cards[4] += 1;
            break;
        case BLACK:
            player->cards[5] += 1;
            break;
        case RED:
            player->cards[6] += 1;
            break;
        case GREEN:
            player->cards[7] += 1;
            break;
        case MULTICOLOR:
            player->cards[8] += 1;
            break;
        
    }
    player->nbCards += 1;

}

/* Function removing a card from the player's deck */
void removeCard(t_player * player, t_color card, int nbLocomotives){

    int i = player->nbCards-1;
    int out = 0;

    while(i>=0 && out !=1){
        if(player->cards[i] == card){
            player->cards[i] -= 1;
            out = 1;
        }
        i--;
    }
    player->nbCards -= nbLocomotives;
}

/* Function initialising the different tracks. The tracks that doesn't exist have a lenght of INT_MAX*/
void initTracks(t_track Tracks[80][80],int * arrayTracks,int nbTracks, int nbCities){

    t_track T[80];
    for (int i=0; i<nbTracks; i++){
        T[i].city1 = arrayTracks[5*i];
        T[i].city2 = arrayTracks[5*i+1];
        T[i].length = arrayTracks[5*i+2];
        T[i].color1 = arrayTracks[5*i+3];
        T[i].color2 = arrayTracks[5*i+4];
        T[i].taken = 0;
    }

    /* For each city we assess whether the road exists or not*/
    for (int i=0; i<nbCities; i++){

        for(int j=0; j<nbCities; j++){
            Tracks[i][j].length = __INT_MAX__;
            for (int k=0; k<nbTracks; k++){
                if( (T[k].city1 == i && T[k].city2 == j) || (T[k].city1 == j && T[k].city2 == i) ){
                    Tracks[i][j] = T[k];
                }
            }
        }
    }
}

/* Gives the minimum distance between a departure city and the others
   Returns the index of the closest city
*/
int minimumDistance(int D[80],int visited[50],int nbCities){
    int min = __INT_MAX__;
    int minIndex;

    for (int i=0; i<nbCities; i++){
        if (D[i]<min && visited[i] == 0){
            min = D[i];
            minIndex = i;
        }
    }
    return minIndex;
}

/* Function determining the shorter road to get to an objective */
int Dijkstra(int Prec[80],t_track Tracks[80][80],int src, int dest, int nbCities){

    int D[80];
    int visited[nbCities];
    int u = src;
    int nbTracksNeeded = 0;

    for (int i=0; i<nbCities;i++){
        D[i] = __INT_MAX__;
        visited[i] = 0;
    }
    D[src] = 0;

    while (u != dest) {

        u = minimumDistance(D,visited,nbCities);
        visited[u] = 1;

        for (int v=0; v<nbCities; v++){

            /* we consider the track our's so we put the distance at 0*/
            if (Tracks[u][v].taken == 1){
                Tracks[u][v].length = 0;
            }
            else if (Tracks[u][v].taken == -1){
                Tracks[u][v].length = __INT_MAX__;
            }
            else if (visited[v] == 0 && Tracks[u][v].length != __INT_MAX__ && D[u] + Tracks[u][v].length < D[v] && Tracks[u][v].taken != -1){
                D[v] = D[u] + Tracks[u][v].length;
                Prec[v] = u; 
                nbTracksNeeded += 1;
            }
        }
    }
    return nbTracksNeeded;
}

/* Function giving the roads we need to take in order to achieve an objective 
    Returns the length of the number of roads we need to take as well as those roads
*/
int givePath(int src, int dest, int nbTracksNeeded, t_track path[10], t_track Tracks[80][80],int Prec[80]){
    int v = dest;
    int i = nbTracksNeeded-1;
    int pathLength = nbTracksNeeded;

    while (v != src){

        /* if nobody owns the track then we add it to our objective path */
        if (Tracks[v][Prec[v]].taken == 0){
            path[i] = Tracks[v][Prec[v]];
        }

        /* if it's one of our's then we reduce the size of our path */
        else if (Tracks[v][Prec[v]].taken == 1){
    
            for (int j=i; j<pathLength-1; j++){
                path[j] = path[j+1];
            }
            pathLength -= 1;
        }
        
        v = Prec[v];

        i--;

    }

    t_track tempTrack;

    /* classify the tracks to take from the greater length to the least */

    for(int k=0; k<pathLength-1; k++){

        for(int j=k+1;j<pathLength;j++)

            if ( path[k].length < path[j].length ) {

                tempTrack = path[k];
                path[k] = path[j];
                path[j] = tempTrack;
            }
    }
    return pathLength;
}

/* Function uptdating the different variables of the game */
void updateGame(t_game * game){

    if (game->playing == 0){ /* ie if we play */
        int pos = game->player[0].nbObjective;
        int offset = 0;

        switch (game->move.type){

            case CHOOSE_OBJECTIVES:

                for (int i=pos; i<pos + 3; i++){
                    if (game->move.chooseObjectives.chosen[i-pos] != 0){
                        game->player[0].objectives[i - offset] = game->drawObjectives.objectives[i-pos];
                        game->player[0].nbObjective += 1;
                    }
                    else{
                        offset += 1;
                    }
                }
                break;

            case DRAW_BLIND_CARD :
                addCard(&game->player[0],game->move.drawBlindCard.card);
                game->player[0].missingCards[game->move.drawBlindCard.card] -= 1;
                break;
            case DRAW_CARD :
                addCard(&game->player[0],game->move.drawCard.card);
                for (int i=0; i<5; i++){
                    game->faceUp[i] = game->move.drawCard.faceUp[i];
                }
                break;
            case CLAIM_ROUTE :
                for (int i=0; i<game->move.claimRoute.nbLocomotives; i++){
                    removeCard(&game->player[0],game->move.claimRoute.color,game->move.claimRoute.nbLocomotives);
                }
                game->player[0].nbWagon -= game->move.claimRoute.nbLocomotives;
                game->Tracks[game->move.claimRoute.city1][game->move.claimRoute.city2].taken=1;
                
        }
    }
    else { /* ie the opponent plays */

        switch (game->move.type){
            case DRAW_BLIND_CARD:
                game->player[1].nbCards += 1;
                break;
        
            case DRAW_CARD :
                for (int i=0; i<5; i++){
                        game->faceUp[i] = game->move.drawCard.faceUp[i];
                }
                game->player[1].nbCards += 1;
                break;
        
            case CLAIM_ROUTE :

                for (int i=0; i<game->move.claimRoute.nbLocomotives; i++){
                    removeCard(&game->player[1],game->move.claimRoute.color,game->move.claimRoute.nbLocomotives);
                }
                game->player[1].nbWagon -= game->move.claimRoute.nbLocomotives;
                game->Tracks[game->move.claimRoute.city1][game->move.claimRoute.city2].taken=-1;
                
                break;

            case CHOOSE_OBJECTIVES :
                for (int i=0; i<3; i++){
                    if(game->move.chooseObjectives.chosen[i] != 0)
                        game->player[1].nbObjective += 1;
                }
                
        }
        
    }
    game->lap += 1;
    
}

void displayParam(t_game game){

    printf("-----Game parameters-----\n");
    printf("Name of the game : %s\n", game.name);
    printf("Number of the player playing : %d\n", game.playing);
    printf("Our number : %d\n", game.noPlayer);
    printf("Replay = %d\n", game.replay);
    for (int i=0; i<5; i++){
        printf("FaceUp[%i] = %d\n",i,game.faceUp[i]);
    }
    printf("Return code : %d\n",game.retCode);
    printf("Last move (should be NONE) : %d\n\n", game.lastMove);
 
    printf("-----Board parameters-----\n");
    printf("Number of cities : %d\n",(game.board.nbCities));
    printf("Number of tracks : %d\n\n",(game.board.nbTracks));

    printf("-----Player's parameters-----\n");
    printf("Username : %s\n", game.player[0].name);
    printf("Number of wagons : %d\n",game.player[0].nbWagon);
    printf("Number of cards at hand : %d\n", game.player[0].nbCards);
    printf("Number of objectives : %d\n", game.player[0].nbObjective);
    for (int i=0; i<game.player[0].nbObjective; i++){
        displayObjective(game.player[0].objectives,i);
    }
   
    for (int i=0; i<4; i++){
        printf("Card n°%d is %d\n",i,game.player[0].startCards[i]);
    }
}

void displayPlayersParam(t_game * game){
    for (int i=0; i<2; i++){
        printf("\n===Player %d===\n",i);
        /*printf("Nb wagons : %d\n",game->player[i].nbWagon);
        printf("Nb cards : %d\n",game->player[i].nbCards);
       */ printf("Nb objectives : %d\n",game->player[i].nbObjective);
        if (i==0){
            /*for (int j=0; j<10; j++){
                printf("Card n°%d = %d\n",j+1,game->player[0].cards[j]);
                
            }*/
            for (int j=0; j<game->player[0].nbObjective;j++){
                displayObjective(game->player[0].objectives,j);
            }
        }
    }
}