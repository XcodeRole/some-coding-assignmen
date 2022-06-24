PROGRAM = game.so
cpp_FILES = iModel.cpp minimaxLog.cpp Mala.cpp expertLearner.cpp a.cpp Exp3.cpp eee.cpp ucb.cpp Rmax.cpp MySocket.cpp Littman.cpp CompStuff.cpp Rndm.cpp strategyPair.cpp REexpert.cpp jefe.cpp jefe++.cpp Godfather.cpp Bully.cpp WoLF.cpp QLearner.cpp playStrat.cpp filePlayer.cpp MQubed.cpp Manipulator.cpp CJAL.cpp fp.cpp Exp3_act.cpp gigawolf.cpp HedgeWMA.cpp SAlg.cpp sfp.cpp DeepQ.cpp Pavlov.cpp Memory1.cpp Memory2.cpp
c_FILES =
CC = g++
CXXFLAGS = -O -g -std=c++11 -DMYSOCKET_H

OBJECTS = $(c_FILES:.c=.o) $(cpp_FILES:.cpp=.o)

%.o:%.cpp
	$(CC) $(CXXFLAGS) -fPIC  -c  $< -o $@

$(PROGRAM): $(OBJECTS)
	$(CC) -shared -o $@ $(INCLUDES) $(OBJECTS) $(LIBS)

clean:
	rm -rf *.o $(PROGRAM)

rebuild: clean
	make
