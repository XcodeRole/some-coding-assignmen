#include <iostream>
#include <vector>
#include <sstream>



using namespace std;

enum class Gender{
    Male=1,
    Female=2
};

class Product{
    private:
    int p_id;
    string name;
    int amount;
    double price;

    public:
    Product(){};
    Product(int p_id,string name,int amount,double price):p_id(p_id),name(name),amount(amount),price(price){};
    
    friend class MainProcess;
};

class Persion{
    private:
    string name;
    Gender gender;
    string mobile_num;
    string addr;

    public:
    Persion(){};
    Persion(string name,Gender gender,string mobile_num,string addr):name(name),gender(gender),mobile_num(mobile_num),addr(addr){};
    friend class MainProcess;
};

class Customer:public Persion{
    private:
    int c_id;
    int capital=10000;
    //a vector of products the customer is purchasing
    vector <Product> purch_prod;

    public:
    Customer(){};
    Customer(string name,Gender gender,string mobile_num,string addr,int c_id):Persion(name,gender,mobile_num,addr),c_id(c_id){};
    friend class MainProcess;
};


class Manager : public Persion{
    private:
    int age;
    string title;

    public:
    Manager(){};
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
    /*
    At first I was going to use a reference to represent a customer in the 
    customer list and be able to modify it,However, the reference needs to be assigned at the time of initialization, 
    which does not conform to the dynamic assignment in this application.
    */
    //pointer to represent a specific customer in above customer_list
    Customer *purchase_cus;

    //private function on manager menu
    void add_product();
    void query_product();
    void add_customers();
    void query_customers();
    
    //private fucntion on customer menu
    void add_product_to_cart();
    void query_product_in_cart();
    void check_up();

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