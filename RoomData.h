#ifndef ROOMDATA_H_INCLUDED
#define ROOMDATA_H_INCLUDED

#include<iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <ctime>
#include <iomanip>
#include "Booking.h"
#include "Payment.h"
#include "Room.h"

class RoomData
{
private:
    vector<Room*> rooms;
    vector<Booking> bookings;

    string trim(const string& str) // Jackt (2019)
    {
        size_t start = str.find_first_not_of(" \t\n\r");
        size_t end = str.find_last_not_of(" \t\n\r");
        return (start == string::npos) ? "" : str.substr(start, end - start + 1);
    }

     bool isLeapYear(int year)
    {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    bool isValidDate(const string& date)
    {
        if (date.size() != 10 || date[2] != '/' || date[5] != '/')
            return false;

        struct tm tm = {0};

        if (sscanf(date.c_str(), "%2d/%2d/%4d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year) != 3)
            return false;

        if (tm.tm_mon < 1 || tm.tm_mon > 12 || tm.tm_mday < 1 || tm.tm_mday > 31)
            return false;

        if (tm.tm_mon == 2 && tm.tm_mday > (isLeapYear(tm.tm_year) ? 29 : 28))
            return false;

        if ((tm.tm_mon == 4 || tm.tm_mon == 6 || tm.tm_mon == 9 || tm.tm_mon == 11) && tm.tm_mday > 30)
            return false;

        return true;
    }

    bool hasDateConflict(const string& checkIn, const string& checkOut,const string& hotel, const string& roomType)
    {
        for (const auto& booking : bookings)
        {
            if (booking.hotel == hotel && booking.roomType == roomType && Booking::hasDateConflict(booking.username, hotel, roomType, checkIn, checkOut))
            {
                return true;
            }
        }
        return false;
    }

     int calculateDaysDifference(string checkInDate, string checkOutDate) //Hydlide (2012)
    {
        struct tm tm1 ={0}, tm2={0};

        sscanf(checkInDate.c_str(),"%d/%d/%d", &tm1.tm_mday, &tm1.tm_mon,&tm1.tm_year);
        tm1.tm_mon -= 1;
        tm1.tm_year -= 1900;

        sscanf(checkOutDate.c_str(), "%d/%d/%d", &tm2.tm_mday, &tm2.tm_mon, &tm2.tm_year);
        tm2.tm_mon -= 1;
        tm2.tm_year -= 1900;

         time_t time1 = mktime(&tm1);
        time_t time2 = mktime(&tm2);

        if (time1 == -1 || time2 == -1)
        {
            cerr << "Error: Invalid date conversion." << endl;
            return -1;
        }

        return static_cast<int>(difftime(time2,time1)/(60*60*24));
    }

public:
    ~RoomData()
    {
        for (Room* room : rooms)
        {
            delete room;
        }
    }

    void loadRoomsFromFile(const string& fileName)
    {
        ifstream file(fileName);
        if (!file.is_open())
        {
            cerr << "Error: Unable to open file " << fileName << endl;
            return;
        }

        string line;
        getline(file, line);

        while (getline(file, line))
        {
            stringstream ss(line);
            string destination, hotel, roomType, priceStr, capacityStr, facilities, quantityStr;

            getline(ss, destination, ',');
            getline(ss, hotel, ',');
            getline(ss, roomType, ',');
            getline(ss, priceStr, ',');
            getline(ss, capacityStr, ',');
            getline(ss, facilities, ',');
            getline(ss, quantityStr, ',');

            destination = trim(destination);
            hotel = trim(hotel);
            roomType = trim(roomType);
            facilities = trim(facilities);

            try
            {
                double price = stod(trim(priceStr));
                int capacity = stoi(trim(capacityStr));
                int quantity = stoi(trim(quantityStr));

                if (roomType == "Deluxe")
                {
                    rooms.push_back(new DeluxeRoom(destination, hotel, price, capacity, facilities, quantity));
                }
                else if (roomType == "Suite")
                {
                    rooms.push_back(new SuiteRoom(destination, hotel, price, capacity, facilities, quantity));
                }
                else
                {
                    rooms.push_back(new StandardRoom(destination, hotel, price, capacity, facilities, quantity));
                }
            }

            catch (const exception&)
            {
                cerr << "Error: Invalid data in line: " << line << endl;
            }
        }
        file.close();
    }

    void saveRoomsToFile(const string& fileName)
    {
        ofstream file(fileName);
        if (!file.is_open())
        {
            cerr << "Error: Unable to open " << fileName << endl;
            return;
        }

        file << "Destination,Hotel,Room Type,Price,Capacity,Amenities,Quantity\n";
        for (const auto& room : rooms)
        {
            file << room->destination << "," << room->hotel << "," << room->roomType << "," << room->price << ","
                 << room->capacity << "," << room->facilities << "," << room->quantity << "\n";
        }
        file.close();
    }

    void saveToHistory(const string& fileName, const Booking& booking)
    {
        ofstream file(fileName, ios::app);
        if (!file.is_open())
        {
            cerr << "Error: Unable to open " << fileName << endl;
            return;
        }

        file << booking.username << "," << booking.bookingID << "," << booking.destination << "," << booking.hotel << ","
            << booking.roomType << "," << booking.checkInDate << "," << booking.checkOutDate << "," << booking.totalPrice << endl;
        file.close();
        cout << "\nBooking saved to history." << endl;
    }

    void displayRoomsByDestination(const string& destination)
    {
        bool found = false;
        for (const Room* room : rooms)
        {
            if (equal(destination.begin(), destination.end(), room->destination.begin(),
                [](char a, char b) { return tolower(a) == tolower(b); }))
            {
                room->display();
                found = true;
            }
        }
        if (!found)
        {
            cout << "No rooms found for destination: " << destination << endl;
        }
    }

    bool caseInsensitiveCompare(const string& a, const string& b) //Josh Kelley (2012)
    {
        if (a.size() != b.size())
            return false;

        return equal(a.begin(), a.end(), b.begin(),
                 [](char a, char b) { return tolower(a) == tolower(b); });
    }

    void findBookingByID(const string& bookingID, const string& fileName)
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        cerr << "Error: Unable to open " << fileName << endl;
        return;
    }

    string line;
    bool found = false;

    // Skip the header line
    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);
        string username, id, destination, hotel, roomType, checkInDate, checkOutDate, totalPriceStr;

        getline(ss, username, ',');
        getline(ss, id, ',');
        getline(ss, destination, ',');
        getline(ss, hotel, ',');
        getline(ss, roomType, ',');
        getline(ss, checkInDate, ',');
        getline(ss, checkOutDate, ',');
        getline(ss, totalPriceStr, ',');

        if (id == bookingID)
        {
            found = true;
            double totalPrice = stod(totalPriceStr);

            cout << "\n------ Booking Details ------" << endl;
            cout << "Booking ID: " << id << endl;
            cout << "Username: " << username << endl;
            cout << "Destination: " << destination << endl;
            cout << "Hotel: " << hotel << endl;
            cout << "Room Type: " << roomType << endl;
            cout << "Check-in Date: " << checkInDate << endl;
            cout << "Check-out Date: " << checkOutDate << endl;
            cout << "Total Price: $" << fixed << setprecision(2) << totalPrice << endl;
            cout << "-----------------------------" << endl;
            break;
        }
    }

    if (!found)
    {
        cout << "No booking found with ID: " << bookingID << endl;
    }

    file.close();
}


    void bookRoom(const string& destination,string username)
    {
        vector<Room*> availableRooms;

        for (auto& room : rooms)
        {
            if (caseInsensitiveCompare(destination, room->getDestination()) && room->getQuantity() > 0)
            {
                availableRooms.push_back(room);
            }
        }

        if (availableRooms.empty())
        {
            cout << "No rooms available for destination: " << destination << endl;
            return;
        }

        for (const auto& room : availableRooms)
        {
            room->display();
        }

        cout << "\nEnter hotel name: ";
        string hotelName;
        getline(cin, hotelName);

        cout << "Enter room type: ";
        string roomType;
        getline(cin, roomType);

        Room* selectedRoom = nullptr;
        for (auto& room : availableRooms)
        {
            if (equal(hotelName.begin(), hotelName.end(), room->hotel.begin(),
                           [](char a, char b) { return tolower(a) == tolower(b); }) &&
                equal(roomType.begin(), roomType.end(), room->roomType.begin(),
                           [](char a, char b) { return tolower(a) == tolower(b); }))
            {
                selectedRoom = room;
                break;
            }
        }

        if (!selectedRoom)
        {
            cout << "No matching hotel and room type found." << endl;
            return;
        }

        cout << "Enter the number of rooms to book: ";
        int quantityToBook;
        cin >> quantityToBook;

        if (quantityToBook > selectedRoom->quantity)
        {
            cout << "Not enough rooms available. Only " << selectedRoom->quantity << " rooms left." << endl;
            return;
        }

        cout<<"Enter the total number of guests: ";
        int totalGuests;
        cin>>totalGuests;

        if (totalGuests > (selectedRoom->capacity * quantityToBook))
        {
            cout << "The selected number of rooms cannot accommodate " << totalGuests << " guests. "
                  << "Each room has a capacity of " << selectedRoom->capacity << " guests."<<endl;
            return;
        }

        cout << "Enter check-in date (dd/mm/yyyy): ";
        string checkInDate;
        cin >> checkInDate;

        cout << "Enter check-out date (dd/mm/yyyy): ";
        string checkOutDate;
        cin >> checkOutDate;

        if (!isValidDate(checkInDate) || !isValidDate(checkOutDate))
        {
            cerr << "Invalid date format. Please use dd/mm/yyyy." << endl;
            return;
        }

        int totalDays = calculateDaysDifference(checkInDate, checkOutDate);
        if (totalDays <= 0)
        {
            cerr << "Error: Check-out date must be after check-in date." << endl;
            return;
        }

        if (hasDateConflict(checkInDate, checkOutDate, selectedRoom->hotel, selectedRoom->roomType))
        {
            cout << "The selected room is not available during the specified dates due to an existing booking." << endl;
            return;
        }

        double subtotal = totalDays * quantityToBook * selectedRoom->price;
        double tax= subtotal* 0.10;
        double totalAmount= subtotal + tax;

        cout << "\n------ Booking Summary ------"<<endl;
        cout << "Hotel: " << selectedRoom->hotel << endl;
        cout << "Room Type: " << selectedRoom->roomType << endl;
        cout << "Price per Room per Night: $" << fixed << setprecision(2) << selectedRoom->price << endl;
        cout << "Number of Rooms: " << quantityToBook << endl;
        cout << "Number of Nights: " << totalDays << endl;
        cout << "Subtotal: $" << subtotal << endl;
        cout << "Taxes (10%): $" << tax << endl;
        cout << "Total Amount: $" << totalAmount << endl;

        //Payment
        cout<<"\nSelect payment method (Credit Card/ E-Wallet/ Cash on Delivery/ Online Banking): ";
        string paymentMethod;
        cin.ignore();
        getline(cin, paymentMethod);

        string cardNumber= "";
        string accountNumber= "";
        if(paymentMethod=="Credit Card")
        {
            cout<<"Enter card number: ";
            getline(cin,cardNumber);
        }
        else if(paymentMethod=="Online Banking")
        {
            cout<<"Enter account number: ";
            getline(cin, accountNumber);
        }

        //Ask for confirmation
        char confirm;
        cout << "Do you want to confirm the booking? (y/n): ";
        cin >> confirm;
        if (tolower(confirm) != 'y')
        {
            cout << "Booking canceled."<<endl;
            return;
        }

        cout << "Processing payment..."<<endl;
        cout << "Press Enter to continue..."<<endl;
        cin.ignore();
        cin.get();

        cout << "Payment successful!"<<endl;


        Payment payment(paymentMethod, totalAmount, cardNumber, accountNumber);
        if(!payment.processPayment())
        {
            cerr<<"Booking canceled."<<endl;
            return;
        }

        // Book the room
        selectedRoom->quantity -= quantityToBook;
        string bookingID = Booking::generateBookingID();
        Booking newBooking(username, bookingID, selectedRoom->destination, selectedRoom->hotel, selectedRoom->roomType,
                           checkInDate, checkOutDate,totalAmount);
        bookings.push_back(newBooking);

        saveRoomsToFile("RoomType.csv");
        saveToHistory("BookingHistory.csv",newBooking);
        payment.savePaymentToHistory(username, "PaymentHistory.csv");

        cout << "Booking successful! Your booking ID is " << bookingID << endl;
        payment.displayPaymentDetails();

        cout << "\n------ Booking Confirmation -----"<<endl;
        cout << "Booking ID: " << bookingID << endl;
        cout << "Hotel: " << selectedRoom->hotel << endl;
        cout << "Room Type: " << selectedRoom->roomType << endl;
        cout << "Check-in Date: " << checkInDate << endl;
        cout << "Check-out Date: " << checkOutDate << endl;
        cout << "Total Amount Paid: $" << totalAmount << endl;
        cout << "Payment Method: " << paymentMethod << endl;
        cout << "-----------------------------------"<<endl;
        cout << "Booking successful!" << endl;
    }

};

#endif // ROOMDATA_H_INCLUDED
