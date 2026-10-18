#pragma once
#include <iostream>
#include <string>
using namespace std;

class Owner {
private:
    int id;
    string name;
    string address;
    string phone;
    string email;

public:
    Owner();
    Owner(int, const string&, const string&, const string&, const string&);

    int getId() const;
    string getName() const;
    string getAddress() const;
    string getPhone() const;
    string getEmail() const;

    void setName(const string&);
    void setAddress(const string&);
    void setPhone(const string&);
    void setEmail(const string&);

    string toString() const;
    static Owner fromString(const string&);
    void showInfo() const;
};
