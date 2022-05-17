#include "tradding.hpp"

void MainProcess::root_menu(){
    this->menu=Menu::root_m;
    cout<<"*****************************\n\
* 1: manager                *\n\
* 2: customer               *\n\
* 3: quit the program       *\n\
* ***************************\n\
Please input a number to set your role:"<<endl;
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

void MainProcess::manager_menu(){
    this->menu=Menu::manager_m;
    cout<<"*****************************\n\
* 1: add products           *\n\
* 2: query products         *\n\
* 3: add customers          *\n\
* 4. query customers        *\n\
* 5. move to parent menu    *\n\
* ***************************\n\
Please input a number to run the program:"<<endl;
}

void MainProcess::customer_menu(){
    this->menu=Menu::customer_m;
    cout<<"Please input"<<endl;
    cout<<"*****************************\n\
* 1: add products           *\n\
* 2: query products         *\n\
* 3: add customers          *\n\
* 4. query customers        *\n\
* 5. move to parent menu    *\n\
* ***************************\n\
Please input a number to run the program:"<<endl;
}