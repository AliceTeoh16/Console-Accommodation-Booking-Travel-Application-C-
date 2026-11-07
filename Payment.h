#ifndef PAYMENT_H_INCLUDED
#define PAYMENT_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

class Payment
{
private:
    string paymentID;
    string paymentMethod;
    string cardNumber;
    string accountNumber;
    double totalAmount;
    string paymentStatus;

public:
    Payment()
    {
        paymentID="";
        paymentMethod="";
        cardNumber="";
        accountNumber="";
        totalAmount=0.0;
        paymentStatus="Pending";
    }

    Payment(string method, double amount, string card= "",string account="")
    {
        paymentMethod= method;
        totalAmount= amount;
        cardNumber= card;
        accountNumber= account;
        paymentStatus= "Pending";
        paymentID= generatePaymentID();

    }

    static string generatePaymentID()
    {
        const int idLength = 7;
        const string chars = "0123456789";
        string paymentID = "PAY";

        srand(time(0));

        for (int i = 0; i < idLength; i++)
        {
            paymentID += chars[rand() % chars.size()];
        }

        return paymentID;
    }

    bool processPayment()
    {
        cout<<"\nProcessing payment of $" << totalAmount<< " via "<<paymentMethod;
        if(paymentMethod=="Credit Card")
        {
            cout<<"(Card Number: "<<cardNumber<<")";
        }
        else if (paymentMethod=="Online Banking")
        {
            cout<<"(Account Number: "<<accountNumber<<")";
        }
        cout<<"..."<<endl;

        if(paymentMethod=="Cash on Delivery")
        {
            cout<<"Payment will be collected upon arrival."<<endl;
        }
        else if(!cardNumber.empty()&& paymentMethod=="Credit Card")
        {
            cout<<"Payment successful!"<<endl;
        }
        else if(!accountNumber.empty()&& paymentMethod=="Online Banking")
        {
            cout<<"Payment completed via online banking."<<endl;
        }
        else if(paymentMethod=="E-Wallet")
        {
            cout<<"Payment confirmed from your E-Wallet."<<endl;
        }
        else
        {
            cerr<<"Payment failed. Please try again."<<endl;
            return false;
        }

        paymentStatus = "Completed";
        return true;
    }

    void savePaymentToHistory(string username, string fileName)
    {
        ofstream file(fileName, ios::app);

        if(!file.is_open())
        {
            cerr<<"Error: Unable to open "<<fileName<<endl;
            return;
        }

        file<<username<<","<<paymentID<<","<<paymentMethod<<",";

        if(!cardNumber.empty())
        {
            file<<cardNumber<<",";
        }
        else if(!accountNumber.empty())
        {
            file<<accountNumber<<",";
        }
        else
        {
            file<<"N/A,";
        }

        file<<totalAmount<<endl;
        file.close();
    }

    void displayPaymentDetails()
    {
        cout<<"Payment ID: "<<paymentID<<endl;
        cout<<"Payment Method: "<<paymentMethod<<endl;
        if(!cardNumber.empty())
        {
            cout<<"Card Number: "<<cardNumber<<endl;
        }
        else if(!accountNumber.empty())
        {
            cout<<"Account Number: "<<accountNumber<<endl;
        }
        cout<<"Total Amount: $ "<<totalAmount<<endl;
        cout<<"Payment Status: "<<paymentStatus<<endl;
    }

    void loadPaymentHistory(string fileName, string username)
    {
        ifstream file(fileName);
        if(!file.is_open())
        {
            cerr<<"Error: Unable to open "<<fileName<<endl;
            return;
        }

        string line;
        getline(file, line);

        bool found= false;

        while(getline(file, line))
        {
            stringstream ss(line);
            string uname,id,method,cardOrAccount, totalPrice;

            getline(ss,uname,',');
            getline(ss,id,',');
            getline(ss,method,',');
            getline(ss,cardOrAccount,',');
            getline(ss,totalPrice,',');

            if(uname==username)
            {
                cout<<uname<<setw(15)<<id<<setw(20)<<method<<setw(23)<<cardOrAccount<<setw(15)<<totalPrice<<endl;
                found = true;
            }
        }

        if(!found)
        {
            cout<<"No payment history found !"<<endl;
        }

        file.close();
    }

    string getPaymentStatus()
    {
        return paymentStatus;
    }

    string getPaymentID()
    {
        return paymentID;
    }
};

#endif // PAYMENT_H_INCLUDED
