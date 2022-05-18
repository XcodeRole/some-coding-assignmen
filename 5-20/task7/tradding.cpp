#include "tradding.hpp"
using namespace std;

//Overload the standart input of the Gender enumeration type
istream& operator>>(istream& is, Gender& gender){
    int n;
    is >> n;
    gender = Gender(n);
    return is;
}

//display root menu
void MainProcess::root_menu(){
    this->menu=Menu::root_m;
    cout<<"*****************************\n\
* 1: manager                *\n\
* 2: customer               *\n\
* 3: quit the program       *\n\
* ***************************\n\
Please input a number to set your role:";
    Menu select;
    int i;
    cin>>i;
    switch (i)
    {
    case 1:
        this->manager_menu();
        break;
    case 2:
        this->customer_menu();
    case 3:
        return;
    default:
        break;
    }
}

//display manager menu
void MainProcess::manager_menu(){
    this->menu=Menu::manager_m;
    cout<<"*****************************\n\
* 1: add products           *\n\
* 2: query products         *\n\
* 3: add customers          *\n\
* 4. query customers        *\n\
* 5: move to parent menu    *\n\
* ***************************\n\
Please input a number to run the program:";
    int i;
    cin>>i;
    switch (i)
    {
    case 1:
        this->add_product();
        break;
    case 2:
        this->query_product();
        break;
    case 3:
        this->add_customers();
        break;
    case 4:
        this->query_customers();
        break;
    case 5:
        this->root_menu();
        break;
    default:
        break;
    }
}

//manager add product 
void MainProcess::add_product(){
    int p_id;
    string name;
    int amount;
    double price;
    bool another;
    cout<<"Product id:";
    cin>>p_id;
    cout<<"Product name:";
    //to consider space in the input name string
    getline(cin,name);
    // cin>>name;
    cout<<"Product price:";
    cin>>price;
    Product product(p_id,name,amount,price);
    this->product_list.push_back(product);
    cout<<"Add another product( 0 : no , 1 : yes )?";
    cin>>another;
    if(another)
        this->add_product();
    else 
        this->manager_menu();
}

//manager query product
void MainProcess::query_product(){
    cout<<"Input a product Id (Input \"*\" to list all the products):";
    string p_id;
    cin>>p_id;
    istringstream in(p_id);
    if (p_id=="*"){
        cout<<"Product ID\t\tName\t\tAmount\t\tPrice"<<endl;
        for (auto product:this->product_list){
            cout<<product.p_id<<"\t\t"<<product.name<<"\t\t"<<product.amount<<"\t\t"<<product.price<<endl;
        }
    }else{
        int p_id;
        in>>p_id;
        bool found=false;
        for (auto product:this->product_list){
            if(product.p_id==p_id){
                cout<<product.p_id<<"\t\t"<<product.name<<"\t\t"<<product.amount<<"\t\t"<<product.price<<endl;
                found=true;
            }
        }
        if(!found){
            cout<<"No Product Found"<<endl;
        }
    }
    this->manager_menu();
}

//manager add customers
void MainProcess::add_customers(){
    string name;
    Gender gender;
    string mobile_num;
    string addr;
    int c_id;
    cout<<"Cutomer id: ";
    cin>>c_id;
    cout<<"Cutomer name :";
    cin>>name;
    cout<<"Cutomer gender(1:Male 2:Female): ";
    cin>>gender;
    cout<<"Mobile number: ";
    cin>>mobile_num;
    cout<<"Address: ";

    //to include space in the input addr
    getline(cin,addr);
    Customer customer(name,gender,mobile_num,addr,c_id);
    this->customer_list.push_back(customer);
    cout<<"Add another customer (0:No 1:Yes)?";
    bool another;
    cin>>another;
    if(another){
        this->add_customers();
    }else{
        this->manager_menu();
    }
}

//manager query customers
void MainProcess::query_customers(){
    cout<<"Input a customer Id (Input \"*\" to list all customers): ";
    string c_id;
    cin>>c_id;
    istringstream in(c_id);
    if(c_id=="*"){
        cout<<"Customer ID\t\tName\t\tGender\t\tMobile number\t\tCapital\t\taddress"<<endl;
        for (auto customer:this->customer_list){
            cout<<customer.c_id<<"\t\t"<<customer.name<<"\t\t"<<int(customer.gender)<<"\t\t"<<customer.mobile_num<<"\t\t"<<customer.capital<<"\t\t"<<customer.addr;
        }
    }else{
        int c_id;
        in>>c_id;
        bool found=false;
        for (auto customer:this->customer_list){
            if(customer.c_id==c_id){
                found=true;
                cout<<customer.c_id<<"\t\t"<<customer.name<<"\t\t"<<int(customer.gender)<<"\t\t"<<customer.mobile_num<<"\t\t"<<customer.capital<<"\t\t"<<customer.addr;
            }
        }
        if(!found)
            cout<<"No customer found"<<endl;
    }
    this->customer_menu();
}



//display customer menu
void MainProcess::customer_menu(){
    //If you are already in the customer's menu bar, you don't need to enter the customer's id.
    if(this->menu==Menu::customer_m){
        cout<<"*****************************\n\
* 1: add produets into cart *\n\
* 2: query products in cart *\n\
* 3: check up               *\n\
* 4: move to parent menu    *\n\
* ***************************\n\
Please input a number to run the program:"<<endl;
        int selector;
        cin>>selector;
        switch (selector)
        {
        case 1 :
            this->add_product_to_cart();
            break;
        case 2:
            this->query_product_in_cart();
            break;
        case 3:
            this->check_up();
            break;
        case 4:
            this->root_menu();
        default:
            break;
        }
    //If you are not currently in the customer menu bar, you need to enter the customer's id to select
    }else{
        this->menu=Menu::customer_m;
        cout<<"Please input your customer Id: ";
        string c_id_str;
        cin>>c_id_str;
        //Indicates whether we found this customer
        bool found=false;
        do {
            //to build string
            istringstream iss(c_id_str);
            int c_id;
            iss>>c_id;
            for (auto &customer:this->customer_list){
                if (customer.c_id==c_id){
                    found=true;
                    //the customer who will purchase product
                    this->purchase_cus=&customer;
                    break;
                }
            }
            if(!found){
                cout<<"Failed to find your customer Id.Please input a new customer Id or input quit to move to parent menu:";
                cin>>c_id_str;
            }
        //if we found the customer id or user enter "quit",then stop while and go to the next step
        }while(!found&&c_id_str !="quit");
        if(found){
            cout<<"*****************************\n\
    * 1: add produets into cart *\n\
    * 2: query products in cart *\n\
    * 3: check up               *\n\
    * 4: move to parent menu    *\n\
    * ***************************\n\
    Please input a number to run the program:"<<endl;
            int selector;
            cin>>selector;
            switch (selector)
            {
            case 1 :
                this->add_product_to_cart();
                break;
            case 2:
                this->query_product_in_cart();
                break;
            case 3:
                this->check_up();
                break;
            case 4:
                this->root_menu();
            default:
                break;
            }
        }else if(c_id_str=="quit")
            this->root_menu();
    }
}

void MainProcess::add_product_to_cart(){
    int p_id;
    int amount;
    double price;
    cout<<"Please input product id: ";
    cin>>p_id;
    cout<<"Input the amount of the product you want to buy: ";
    cin>>amount;
    Product *product_ptr;
    bool found=false;
    for(auto &product:this->product_list){
        if(product.p_id==p_id){
            product_ptr=&product;
            price=product.price;
            found=true;
            break;
        }
    }
    if(!found){
        cout<<"Product not found"<<endl;
    }else{
        double use_price=amount*price;
        if(use_price<this->purchase_cus->capital){
            cout<<"insufficient funds "<<endl;
        }
        else if(amount>product_ptr->amount){
            cout<<"Insufficient quantity of goods "<<endl;
        }
        else{
            Product product(product_ptr->p_id,product_ptr->name,amount,product_ptr->price);
            this->purchase_cus->purch_prod.push_back(product);
            //There is no need to subtract funds in this step, and it will be subtracted in subsequent accounts.
            // this->purchase_cus->capital-=use_price;
            cout<<"Add product int cart sucessfully"<<endl;
        }
    }
    this->customer_menu();
}

void MainProcess::query_product_in_cart(){
    cout<<"Input a product Id for query (input \"*\" to list all product in your cart): ";
    string p_id;
    cin>>p_id;
    istringstream in(p_id);
    if(p_id=="*"){
        cout<<"Product ID\t\tName\t\tAmount\t\tPrice"<<endl;
        for(auto product:this->purchase_cus->purch_prod){
            cout<<product.p_id<<"\t\t"<<product.name<<"\t\t"<<product.amount<<"\t\t"<<product.price<<endl;
        }
    }else{
        int p_id;
        in>>p_id;
        bool found=false;
        cout<<"Product ID\t\tName\t\tAmount\t\tPrice"<<endl;
        for(auto product:this->purchase_cus->purch_prod){
            if(product.p_id==p_id){
                found=true;
                cout<<product.p_id<<"\t\t"<<product.name<<"\t\t"<<product.amount<<"\t\t"<<product.price<<endl;
                break;
            }
        }
        if(!found){
            cout<<"Not found"<<endl;
        }
    }
    this->customer_menu();
}

void MainProcess::check_up(){
    int total_price=0;
    cout<<"Product ID\t\tName\t\tAmount\t\tPrice\t\tSubtotal"<<endl;
    for(auto product:this->purchase_cus->purch_prod){
        total_price+=product.price*product.amount;
        cout<<product.p_id<<"\t\t"<<product.name<<"\t\t"<<product.amount<<"\t\t"<<product.price<<"\t\t"<<product.price*product.amount<<endl;
    }
    cout<<"Total price: "<<total_price<<endl;
    cout<<"Customer information:"<<endl;
    cout<<"Customer ID: "<<this->purchase_cus->c_id<<"\tcustomer name: "<<this->purchase_cus->name<<"\tCapital: "<<this->purchase_cus->capital<<endl;
    cout<<"Please confirm to check up(0:no 1:yes) ";
    bool confirm;
    cin>>confirm;
    if(confirm){
        this->purchase_cus->capital-=total_price;
        cout<<"Checked up sucessfully!"<<endl;
    }else{
        cout<<"Checked up falied!"<<endl;
    }
    this->customer_menu();
}