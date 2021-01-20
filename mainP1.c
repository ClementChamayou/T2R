/*---------------------------
*
*     Ticket to ride game
*
* ---------------------------
* File : mainP1.c
* Author : Clément Chamayou
* Date : 30 november 2020
*/

#include "TicketToRideAPI.h"
#include <stdlib.h>
#include <string.h>
#include "move.h"
#include "game.h"


int main(void){

    t_server server = {"li1417-56.members.linode.com",{1234,3456,4567},"DO_NOTHING"}; /* description of the server */
    t_game game;                                                                   

    printf("Username : ");
	scanf("%s",game.player[0].name);

    connectToServer(server.serverName,server.port[1],game.player[0].name);

	createGame(&game);

	createPlayers(&game);

	initTracks(game.Tracks,game.board.arrayTracks,game.board.nbTracks,game.board.nbCities);

	//displayParam(game);

	//printMap();

    do {       

        /* display map 
		if (!game.replay)
			printMap();
		*/
		/* the opponent plays ??*/
		if (game.playing == 1){

			game.retCode = getMove(&game.move, &game.replay);
			
		}
		else {
			/* we play */
			game.nbTracksNeeded = (game.Prec,game.Tracks,game.player[0].objectives[0].city1,game.player[0].objectives[0].city2,game.board.nbCities);
			game.lenPath = givePath(game.player[0].objectives[0].city1,game.player[0].objectives[0].city2,game.nbTracksNeeded,game.path,
							game.Tracks,game.Prec);
			chooseMove(game.Tracks,game.path,game.lenPath,&game.player[0],&game.move,game.faceUp,game.player[0].doneObj,
							&game.player[0].nbObjective,game.lap, game.drawObjectives);
			game.replay = needReplay(&game.move, game.lastMove);
			game.retCode = playOurMove(&game.move, &game.lastMove,&game.drawObjectives);
			
		}
		updateGame(&game);
		//displayPlayersParam(&game); 		

		/* change player */
		if (game.retCode == NORMAL_MOVE && !game.replay)
			game.playing = !game.playing;		

    }while(game.retCode == NORMAL_MOVE);
	

    endComment(game);

    closeConnection();

	printf("See you soon racoon !\n");

    return EXIT_SUCCESS;
}

