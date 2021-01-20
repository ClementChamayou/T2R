#options de compilations
CC = gcc
CCFLAGS = -Wall -g
LIBS = 

#fichiers du projet
SRC = mainP1.c TicketToRideAPI.c clientAPI.c move.c game.c
OBJ = $(SRC:.c=.o)
EXEC = P1 

.PHONY: clean all mproper

#règle initiale 
all : $(EXEC)

#dépendance des .h
mainP1.o : TicketToRideAPI.h move.h game.h
TicketToRideAPI.o : TicketToRideAPI.h clientAPI.h
clientAPI.o : clientAPI.h
move.o : move.h game.h
game.o : game.h

#règle de compilation
%.o : %.c 
	$(CC) $(CCFLAGS) -o $@ -c $<

#édition de lien
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

#règles suplémentaires
clean :
	rm -f *.o 

mproper : 
	rm -f $(EXEC) *.o
