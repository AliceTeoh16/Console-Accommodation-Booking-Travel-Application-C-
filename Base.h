#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <iostream>

using namespace std;

class Username
{
protected:
    string username;
public:
    Username()
    {
        username= "";
    }
    Username(string uname)
    {
        username= uname;
    }

    virtual void display()= 0;
};

#endif // BASE_H_INCLUDED
