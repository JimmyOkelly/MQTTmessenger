#ifndef USER1_H
#define USER1_H
#include <string>
#include<iostream>

using namespace std;

extern string LogUser;

class user1
{
public:
    const string username1;
    user1(string Myfirstname , string emailaddress , string mobile); //constructor
private:
    string email;
    string mobile;

};

#endif // USER1_H
