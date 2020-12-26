/*------------------------
*
*     Ticket to ride
*
* ------------------------
* File : mainP1.c
* Author : Clément Chamayou
* Date : 30 november 2020
*/

#include "TicketToRideAPI.h"
#include <stdlib.h>
#include <string.h>


int main(void){

    /* variables describing the server */
    char * serverName = "li1417-56.members.linode.com";
    unsigned int port[2] = {1234,5678};
    char * botName = "DO_NOTHING";

    /* variables describing the map */
    char * gameType = "TRAINING DO_NOTHING start=0 map=USA";
    char * gameName = (char *) malloc(50*sizeof(char)); 
    int nbCities;
    int nbTracks;
    int * arrayTracks;
    
    /* variables related to the game */
    t_color cards[4];
    t_color faceUp[5];
    int player;
    

    connectToServer(serverName,port[1],botName);

    waitForT2RGame(gameType,gameName,&nbCities,&nbTracks);
    arrayTracks = malloc(5*nbTracks*sizeof(int));
    
    player = getMap(arrayTracks,faceUp,cards);

    closeConnection();


    return EXIT_SUCCESS;
}