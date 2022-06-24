PROGRAM = game
cpp_FILES = main.cpp iModel.cpp minimaxLog.cpp Mala.cpp expertLearner.cpp a.cpp Exp3.cpp eee.cpp ucb.cpp Rmax.cpp MySocket.cpp Littman.cpp CompStuff.cpp Rndm.cpp strategyPair.cpp REexpert.cpp jefe.cpp jefe++.cpp Godfather.cpp Bully.cpp WoLF.cpp QLearner.cpp playStrat.cpp filePlayer.cpp MQubed.cpp Manipulator.cpp CJAL.cpp fp.cpp Exp3_act.cpp gigawolf.cpp HedgeWMA.cpp SAlg.cpp sfp.cpp DeepQ.cpp Pavlov.cpp Memory1.cpp Memory2.cpp
c_FILES =
CC = g++

CFLAGS = -O -g -std=c++11 -DMYSOCKET_H
LIBs = 

OBJECTS = $(c_FILES:.c=.o) $(cpp_FILES:.cpp=.o)

.SUFFIXES: .o .cpp .c

.cpp.o:
	$(CC) $(FLAGS) $(CFLAGS) $< -c $(INCLUDES)

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) $< -c $(INCLUDES)

$(PROGRAM): $(OBJECTS)
	$(CC) $(FLAGS) $(CFLAGS) -o $@ $(INCLUDES) $(OBJECTS) $(LIBS)

clean:
	rm -rf *.o game

rebuild: clean
	make
