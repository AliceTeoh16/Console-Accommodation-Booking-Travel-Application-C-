#ifndef BOOKING_H_INCLUDED
#define BOOKING_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include "Base.h"

using namespace std;

class Booking
{
public:
    string username;
    string bookingID;
    string destination;
    string hotel;
    string roomType;
    string checkInDate;
    string checkOutDate;
    double totalPrice;

    Booking(string uname,string id,string des,string h,string type,string checkIn,string checkOut, double price)
    {
        username= uname;
        bookingID= id;
        destination= des;
        hotel= h;
        roomType= type;
        checkInDate= checkIn;
        checkOutDate= checkOut;
        totalPrice= price;
    }

    static bool isDateOverlap(string checkIn1,string checkOut1,string checkIn2, string checkOut2)
    {
        return !(checkOut1<=checkIn2||checkIn1>=checkOut2);
    }

    static string generateBookingID()
    {
        const int idLength=6;
        const string chars= "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        string bookingId="B";

        srand(time(0));

        for(int i=0;i<idLength;i++)
        {
            bookingId +=chars[rand()%chars.size()];
        }

       return bookingId;
    }

    bool isNumber(const string& str)
    {
        if (str.empty())
            return false;

        int i = 0;

        if (str[0] == '-')
            i++;
        for (; i < str.length(); ++i)
        {
            if (!isdigit(str[i]) && str[i] != '.')
                return false;
        }
        return true;
    }


    bool isValidDate(const string& date)
    {
        if (date.length() != 10 || date[2] != '/' || date[5] != '/')
            return false;

        string day = date.substr(0, 2);
        string month = date.substr(3, 2);
        string year = date.substr(6, 4);

        return isNumber(day) && isNumber(month) && isNumber(year) && stoi(day) >= 1 && stoi(day) <= 31 && stoi(month) >= 1 && stoi(month) <= 12;
    }


    static bool hasDateConflict(string uname, string hotel, string type,string checkIn, string checkOut)
    {
        ifstream bookingFile("BookingHistory.csv");
        if(!bookingFile.is_open())
        {
             cerr<<"Error: Unable to open file"<<endl;
             return false;
        }

        string line;
        while(getline(bookingFile,line))
        {
            stringstream ss(line);
            string fileUser,fileBookingID,fileDestination,fileHotel,fileRoomType,fileCheckIn,fileCheckOut,filePrice;
            getline(ss,fileUser,',');
            getline(ss,fileBookingID,',');
            getline(ss,fileDestination,',');
            getline(ss,fileHotel,',');
            getline(ss,fileRoomType,',');
            getline(ss,fileCheckIn,',');
            getline(ss,fileCheckOut,',');
            getline(ss,filePrice,',');

            if(fileUser==uname && fileHotel==hotel && fileRoomType==type && isDateOverlap(fileCheckIn,fileCheckOut,checkIn,checkOut))
            {
                return true;
            }
        }
        return false;

    }

    void saveBookingToHistory(string fileName,Booking& booking)
    {
        ofstream bookingFile(fileName,ios::app);
        if(!bookingFile.is_open())
        {
            string Error= "Unable to open file";
            throw Error;
        }

        bookingFile<<username<<","<<bookingID<<","<<destination<<","<<hotel<<","
                   <<roomType<<","<<checkInDate<<","<<checkOutDate<<","<<totalPrice<<endl;

        bookingFile.close();
    }

    static void viewHistory(string username)
    {
        ifstream bookingFile("BookingHistory.csv");
        if(!bookingFile.is_open())
        {
            string Error= "Unable to open file";
            throw Error;
        }

        string line;
        bool hasBookings= false;

        cout<<"\n---------------------Booking History---------------------"<<endl;

        while(getline(bookingFile,line))
        {
            stringstream ss(line);
            string fileUser, id, dest, hotel, type, checkIn, checkOut, price;
            getline(ss,fileUser,',');
            if(fileUser==username)
            {
                hasBookings= true;
                getline(ss,id,',');
                getline(ss,dest,',');
                getline(ss,hotel,',');
                getline(ss,type,',');
                getline(ss,checkIn,',');
                getline(ss,checkOut,',');
                getline(ss,price,',');

                cout<<"Username: "<<fileUser<<endl;
                cout<<"Booking ID: "<<id<<endl;
                cout<<"Destination: "<<dest<<endl;
                cout<<"Hotel: "<<hotel<<endl;
                cout<<"Room Type: "<<type<<endl;
                cout<<"Check-In: "<<checkIn<<endl;
                cout<<"Check-out: "<<checkOut<<endl;
                cout<<"Total Amount: $"<<price<<"\n"<<endl;
            }

        }
        if(!hasBookings)
        {
            cout<<"No bookings found!"<<endl;
        }

        bookingFile.close();
    }

};

#endif // BOOKING_H_INCLUDED
