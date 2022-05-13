#include<iostream>

using namespace std;
#define N 300


//KMP algorithm caculate the next array
void caculate_next(const char * str, int * next, int len) 
{  
    int i, j;    
    next[0] = -1;  
    for( i = 1; i < len; i++ )  
    {  
        j = next[ i - 1 ];  
        while( str[ j + 1 ] != str[ i ] && ( j >= 0 ) )  
            j = next[ j ];  
        if( str[ i ] == str[ j + 1 ] )  
            next[ i ] = j + 1;  
        else  
            next[ i ] = -1;  
    }  
}
/* 
I think the string length needs to be passed in here to prevent out of bounds,
so I modified the function a little bit 
*/
int findC (char const *source,int s_len,char const *obj,int o_len){
    int next[N];
    caculate_next(obj,next,o_len);
    int i = 0, j = 0;  
    while( i < s_len && j < o_len )  
    {  
        if( source[ i ] == obj[ j ] )  
        {  
            i++;  
            j++;  
        }  
        else  
        {  
            if( j == 0 )  
                i++;  
            else  
                j = next[ j - 1 ] + 1;  
        }  
    }  
    if(j==o_len)
        return (i - o_len);
    else return -1;
}

int main(){
    int s_len,o_len;
    cout<<"Please enter the length of source string as well as the string itself:"<<endl;
    cin>>s_len;
    char *source=(char *)malloc(s_len*sizeof(char));
    cin>>source;
    cout<<"Please enter the length of obj string as well as the string itself:"<<endl;
    cin>>o_len;
    char *obj=(char *)malloc(o_len*sizeof(char));
    cin>>obj;
    int loc=findC(source,s_len,obj,o_len);
    cout<<"The location :"<<loc<<endl;
}