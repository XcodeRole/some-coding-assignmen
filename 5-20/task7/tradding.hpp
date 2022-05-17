#include <iostream>
#include <vector>



using namespace std;

enum class Gender{
    Male,
    Female
};

class Product{
    private:
    int p_id;
    string name;
    int amount;
    double price;
};

class Persion{
    private:
    string name;
    Gender gender;
    string mobile_num;
    string addr;
};

class Customer:public Persion{
    private:
    int c_id;
    int capital=10000;
    vector <int> purch_prod;
};


class Manager : public Persion{
    private:
    int age;
    string title;
};

enum class Menu{
    root_m,
    manager_m,
    customer_m
};

class MainProcess{
    private:
    vector<Product> product_list;
    vector<Customer> customer_list;
    Manager manager;
    Customer purchase_cus;

    //use to represent which menu we are on now
    Menu menu;


    public:
    MainProcess(){};
    ~MainProcess(){};
    void root_menu();
    void manager_menu();
    void logic_control();
    void customer_menu();
};