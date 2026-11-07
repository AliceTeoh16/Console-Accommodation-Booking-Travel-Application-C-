#ifndef USERLOGIN_H_INCLUDED
#define USERLOGIN_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class userLogin
{
private:
    string username;
    string password;
    string fullname;
    string email;

public:
    userLogin(string user,string pass, string fname, string e)
    {
        username= user;
        password= pass;
        fullname= fname;
        email= e;
    }

    void setUsername(string user)
    {
        username= user;
    }

    string getUsername()
    {
        return username;
    }

    void setPassword(string pass)
    {
        password= pass;
    }

    string getPassword()
    {
        return password;
    }

    void setFullName(string fname)
    {
        fullname= fname;
    }

    string getFullName()
    {
        return fullname;
    }

    void setEmail(string e)
    {
        email= e;
    }

    string getEmail()
    {
        return email;
    }

    static bool isValidEmail(string email)
    {
        size_t atPos= email.find('@');
        size_t dotPos= email.find('.',atPos);

        return atPos != string::npos && dotPos != string::npos && dotPos > atPos;
    }

    static void registerUser(string username,string password,string fullname,string email,string filename)
    {
        if(!isValidEmail(email))
        {
            throw string("Invalid email format.");
        }

        if (isUsernameTaken(username, filename))
        {
           throw string("Username already exists. Please choose a different username.");

        }

        ofstream file(filename, ios::app);
        if(!file.is_open())
        {
            cerr<<"Debug: Unable to open "<<filename<<endl;
            throw string("Error: Unable to open file for writing.");
        }
        file<<username<<","<<password<<","<<fullname<<","<<email<<endl;
        file.close();
        cout<<"Registered successfully!"<<endl;
    }

    static bool isUserValid(string username, string password, string filename)
    {
        ifstream file(filename);
        if(!file.is_open())
        {
            cerr<<"Error: Unable to open"<<filename<<endl;
            return false;
        }

        string line,storedUsername,storedPassword,storedFullName,storedEmail;
        getline(file,line);

        while(getline(file,line))
        {
            stringstream ss(line);
            getline(ss,storedUsername,',');
            getline(ss,storedPassword,',');
            getline(ss,storedFullName,',');
            getline(ss,storedEmail,',');

            if(storedUsername == username && storedPassword == password)
            {
                file.close();
                return true;
            }

        }

        file.close();
        return false;
    }

    static bool isUsernameTaken(const string& username, const string& filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            return false;
        }

        string line, storedUsername, storedPassword, storedFullName, storedEmail;
        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, storedUsername, ',');
            if (storedUsername == username)
            {
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }

};


#endif // USERLOGIN_H_INCLUDED
