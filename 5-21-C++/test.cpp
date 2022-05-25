#include <regex>
#include<iostream>
using namespace std;
int main(){
    //默认只搜索一次
    string seastr = "this her isres1is search! isres2is multi result test isres3is as.";
    smatch srm;//保存搜索的结果
    regex rx("this(.*?)as.", regex::icase);//搜索的正则式
    regex_match(seastr, srm, rx);
    cout << srm.str() << endl;
    for (unsigned i = 0; i < srm.size(); ++i) {
        cout << "[" << srm[i] << "] \n";
    }
}
