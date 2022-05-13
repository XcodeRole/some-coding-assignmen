CC?=gcc
CXX?=g++
CFLAGS?=
CXXFLAGS?=
INC?=-IOpenCL-CLHPP/include -I/usr/local/cuda-11.3/targets/x86_64-linux/include/ -I/usr/include/opencv4

LDFLAGS?=-L/usr/local/cuda-11.3/targets/x86_64-linux/lib -lOpenCL -lopencv_core -pthread -lX11 -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

target=demo

cpp2o=$(shell find . -name *.cpp | sed "s/.cpp/.o/")

all:$(target)

$(cpp2o):%.o:%.cpp
	$(CXX) $(INC) -c $< -o $@ $(CXXFLAGS)


$(target):$(cpp2o)
	$(CXX) $< -o $@ $(LDFLAGS)

.PHONY:clean

clean:
	rm -rf demo demo.o


