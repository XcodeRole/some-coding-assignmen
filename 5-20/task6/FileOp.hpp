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
Filename is received from parameter instead of using in-class attribute
Double-precision coordinate points are used here, but in fact, 
when implementing the conversion of string and double, 
I don't know if it will affect the correctness
But I think the function logic is correct
*/
class FileOp{
    public:
    FileOp();
    ~FileOp();

    static void board2file(int num,string filename);
    static vector<coordinate> file2attr(string filename);
    static void del_point(string filename,coordinate cor);
    static bool isInfile(string filename,coordinate cor);
};
