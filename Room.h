#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include<iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <ctime>

using namespace std;


class Room
{
public:
    string destination;
    string hotel;
    string roomType;
    double price;
    int capacity;
    string facilities;
    int quantity;

    Room(string dest, string h, string type, double p, int cap, string faci, int q)
    {
        destination= dest;
        hotel= h;
        roomType= type;
        price= p;
        capacity= cap;
        facilities= faci;
        quantity= q;
    }

    string getDestination()
    {
        return destination;
    }

    int getQuantity()
    {
        return quantity;
    }

    virtual void display() const
    {
        cout<<"Destination: "<<destination<<endl;
        cout<<"Hotel: "<<hotel<<endl;
        cout<<"Room Type: "<<roomType<<endl;
        cout<<"Price per Night: $ "<<price<<endl;
        cout<<"Capacity: "<<capacity<<" guests"<<endl;
        cout<<"Facilities: "<<facilities<<endl;
        cout<<"Number of rooms available: "<<quantity<<endl;
        cout << "---------------------------------" << endl;
    }

    virtual ~Room() {}
};

class DeluxeRoom : public Room
{
public:
    DeluxeRoom(string dest, string h, double p, int cap, string faci, int q)
        : Room(dest, h, "Deluxe", p, cap, faci, q) {}


    void display() const override
    {
        cout << "[Deluxe Room] ";
        Room::display();
    }
};

class SuiteRoom : public Room
{
public:
    SuiteRoom(string dest, string h, double p, int cap, string faci, int q)
        : Room(dest, h, "Suite", p, cap, faci, q) {}

    void display() const override
    {
        cout << "[Suite Room] ";
        Room::display();
    }
};

class StandardRoom : public Room
{
public:
    StandardRoom( string dest,  string h, double p, int cap, string fac, int qty)
        : Room(dest, h, "Standard", p, cap, fac, qty) {}

    void display() const override
    {
        cout << "[Standard Room] ";
        Room::display();
    }
};


#endif // ROOM_H_INCLUDED





