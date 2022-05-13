TARGET=task2

SRC=$(TARGET).cpp

OBJECTS = $(SRC:.cpp=.o)

CXX?=g++

$(TARGET):$(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)


.PHONY: clean
clean:	
	rm -rf $(OBJECTS) $(TARGET)