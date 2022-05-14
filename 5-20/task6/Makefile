TARGET=task6

SRC=$(shell find . -name \*.cpp) #| sed 's/.cpp/.o/')

OBJECTS = $(SRC:.cpp=.o)


CXX?=g++


$(TARGET):$(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)


.PHONY: clean
clean:	
	rm -rf $(OBJECTS) $(TARGET)