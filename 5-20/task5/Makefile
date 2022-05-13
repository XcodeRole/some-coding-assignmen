TARGET=task5

SRC=$(TARGET).cpp

OBJECTS = $(SRC:.cpp=.o)

# CXXFLAGS?=-fpermissive

CXX?=g++

# %.cpp:%.o
# 	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(TARGET):$(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)


.PHONY: clean
clean:	
	rm -rf $(OBJECTS) $(TARGET)