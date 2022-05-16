#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <sstream>

using namespace std;

typedef struct coordinate{
    double x;
    double y;
}coordinate;

/*
There using static functions
Filename is received from parameter instead of using in-class attribute.

Double-precision coordinate points are used here, but in fact, 
when implementing the conversion of string and double, I have to consider precesion
(The conversion precision has been set in the code).
*/

class FileOp{
    public:
    FileOp(){};
    ~FileOp(){};

    static void board2file(int num,string filename);
    static vector<coordinate> file2attr(string filename);
    static bool del_point(string filename,coordinate cor);
    static bool isInfile(string filename,coordinate cor);
};
