#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include "UserLogin.h"
#include "Booking.h"
#include "Room.h"
#include "Payment.h"
#include "RoomData.h"

using namespace std;

int main()
{
    int loginChoice,choice;
    string username, password;
    RoomData roomData;
    Payment payment;
    string roomFile= "RoomType.csv";
    string userFile= "User.csv";
    string bookingFile= "BookingHistory.csv";
    string paymentFile= "PaymentHistory.csv";

    while(true)
    {
        cout<<"---------------------Accommodation Booking Travel Application---------------------"<<endl;
        cout<<"Welcome to the Accommodation Booking Travel Application!!"<<endl;
        cout<<"We provide travel booking to 5 countries which include: ";
        cout<<"Paris, New York, Tokyo, London, Sydney."<<endl<<endl;
        cout<<"1. Login..."<<endl;
        cout<<"2. Register..."<<endl;
        cout<<"3. Exit"<<endl;
        cout<<"Enter your choice: ";
        cin>>loginChoice;

        switch(loginChoice)
        {
        case 1:
            {
                while(true)
                {
                    cout<<"Enter your username: ";
                    cin>>username;
                    cout<<"Enter your password: ";
                    cin>>password;

                    if(userLogin::isUserValid(username,password,userFile))
                    {
                        cout<<"Login successful! Welcome "<<username<<" ."<<endl;

                        while(true)
                        {
                            cout<<"\n---------------------Main Menu---------------------"<<endl;
                            cout<<"1. Display Room Types"<<endl;
                            cout<<"2. Book Room"<<endl;
                            cout<<"3. View All Room Booking History"<<endl;
                            cout<<"4. View Booking History by Booking ID"<<endl;
                            cout<<"5. Book Same Room for a Different Date"<<endl;
                            cout<<"6. View Payment History"<<endl;
                            cout<<"7. Exit"<<endl;
                            cout<<"Enter your choice: ";
                            cin>>choice;

                            switch(choice)
                            {
                            case 1:
                                {
                                    string destination;
                                    roomData.loadRoomsFromFile(roomFile);

                                    cout<<"\nEnter destination to search for rooms: ";
                                    cin.ignore();
                                    getline(cin,destination);
                                    cout<<"\n";

                                    roomData.displayRoomsByDestination(destination);
                                    break;
                                }

                            case 2:
                                {
                                    string dest;
                                    roomData.loadRoomsFromFile(roomFile);

                                    cout<<"Enter destination to book a room: ";
                                    cin.ignore();
                                    getline(cin,dest);
                                    roomData.bookRoom(dest,username);

                                    break;
                                }

                            case 3:
                                {
                                    try
                                    {
                                        Booking::viewHistory(username);
                                    }
                                    catch(string exceptionString)
                                    {
                                        cout<<exceptionString<<endl;
                                    }
                                    break;
                                }

                            case 4:
                                {
                                    string bookingID;
                                    cout<<"Enter your Booking ID: ";
                                    cin.ignore();
                                    getline(cin,bookingID);

                                    roomData.findBookingByID(bookingID,bookingFile);
                                    break;
                                }

                            case 5:
                                {
                                    try
                                    {
                                        cout << "\n---------------------Rebook Room---------------------" << endl;
                                        Booking::viewHistory(username);

                                        cout << "\nEnter the Booking ID of the room you want to book again: ";
                                        string bookingID;
                                        cin >> bookingID;

                                        ifstream bookingFileStream(bookingFile);
                                        if (!bookingFileStream.is_open())
                                        {
                                            cerr << "Error: Unable to open file" << endl;
                                            break;
                                        }

                                        string line, fileUser, id, dest, hotel, rid, type, checkIn, checkOut, price;
                                        bool bookingFound = false;
                                        while (getline(bookingFileStream, line))
                                        {
                                            stringstream ss(line);
                                            getline(ss, fileUser, ',');
                                            getline(ss, id, ',');
                                            getline(ss, dest, ',');
                                            getline(ss, hotel, ',');
                                            getline(ss, rid,',');
                                            getline(ss, type, ',');
                                            getline(ss, checkIn, ',');
                                            getline(ss, checkOut, ',');
                                            getline(ss, price, ',');

                                            if (fileUser == username && id == bookingID)
                                            {
                                                bookingFound = true;

                                                cout << "\nEnter new check-in date (dd/mm/yyyy): ";
                                                string newCheckIn;
                                                cin >> newCheckIn;

                                                cout << "Enter new check-out date (dd/mm/yyyy): ";
                                                string newCheckOut;
                                                cin >> newCheckOut;

                                                if (!Booking::isDateOverlap(checkIn, checkOut, newCheckIn, newCheckOut))
                                                {
                                                    Booking newBooking(username, Booking::generateBookingID(), dest, hotel, type, newCheckIn, newCheckOut, stod(price));
                                                    newBooking.saveBookingToHistory(bookingFile, newBooking);

                                                    cout << "Room successfully rebooked for new dates!" << endl;
                                                }
                                                else
                                                {
                                                    cout << "Error: The selected room has a date conflict for the new dates." << endl;
                                                }
                                                break;
                                            }
                                        }

                                        if (!bookingFound)
                                        {
                                            cout << "Error: Booking ID not found!" << endl;
                                        }

                                        bookingFileStream.close();
                                    }
                                    catch (string exceptionString)
                                    {
                                        cout << exceptionString << endl;
                                    }

                                    break;
                                }


                            case 6:
                                {
                                    cout<<"---------------------Payment History---------------------"<<endl;
                                    cout<<"Username"<<setw(15)<<"Payment ID"<<setw(20)<<"Payment Method"<<setw(25)<<"Card/Account Number"<<setw(15)<<"Total Amount"<<endl;
                                    payment.loadPaymentHistory(paymentFile, username);
                                    break;
                                }

                            case 7:
                                cout<<"Thank You for using the Travel Booking Application. Bye!\n"<<endl;
                                return 0;

                            default:
                                cout<<"Invalid Choice! Please try again."<<endl;
                                break;

                            }
                        }
                        return false;
                    }
                    else
                    {
                        cout<<"Invalid username or password. Please try again.\n"<<endl;
                    }

                }

                break;
            }

        case 2:
            {
                cout<<"Enter your username: ";
                cin>>username;
                cout<<"Enter your password: ";
                cin>>password;
                cout<<"Enter your fullname: ";
                string fullname;
                cin.ignore();
                getline(cin,fullname);
                cout<<"Enter your email: ";
                string email;
                cin>>email;

                try
                {
                    userLogin::registerUser(username, password, fullname, email,userFile);
                }
                catch(string exceptionString)
                {
                    cout<<exceptionString<<endl;
                }

                break;
            }

        case 3:
            cout<<"Exiting...\n"<<endl;
            return false;

        default:
            cout<<"Invalid Choice."<<endl;
            break;
        }
    }
}
