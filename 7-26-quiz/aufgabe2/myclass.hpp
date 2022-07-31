#include <string>
#include <vector>
#include <iostream>
#include "distance.h"
using namespace std;
class Myexcept{
private:
    string message;
public:
    Myexcept(string str = "name latlon std::runtime_error lat[-90, 90] lon[-180, 180]") : message{str} {}
    string what() const { return message; }
};

class Station{
private:
    string name;
    double lat;
    double lon;
public:
    Station();
    Station(string name,double lat,double lon);
    ~Station();
    string get_name() const{
        return name;
    }
    void set_name(const string new_name){
        name=new_name;
    }
    double get_lat() const{
        return lat;
    }
    double get_lon() const{
        return lon;
    }
    double calc_distance(const Station& other) const;
};
Station::Station(string name,double lat,double lon){
    try{
        if(lat < -90 | lat > 90 | lon < -180 | lon > 180)
            throw Myexcept();
    }catch(Myexcept e){
        cout<<e.what();
    }
}
Station::Station(){
    name="Null Island";
    lat=0;
    lon=0;
}
double Station::calc_distance(const Station& other) const{
    return geo_distance(this->get_lat(),this->get_lon(),other.get_lat(),other.get_lon());
}
class Geocache{
private:
    unsigned int terrain;
    unsigned int size;
public:
    vector<Station> stations;
    Geocache(unsigned int terrain,unsigned int size):terrain(terrain),size(size){}
    double calc_difficulty() const{
        return size*4-terrain;
    }
    bool is_multi() const{
        if (stations.size()>1)
            return true;
        else
            return false;
    }
    double calc_length() const;
};