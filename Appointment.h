#pragma once
#include <iostream>
#include <string>
using namespace std;

class Appointment {
private:
    int id;
    int petId;
    string date;
    string time;
    string status;

public:
    Appointment();
    Appointment(int, int, const string&, const string&, const string&);

    int getId() const;
    int getPetId() const;
    string getDate() const;
    string getTime() const;
    string getStatus() const;

    void setDate(const string&);
    void setTime(const string&);
    void setStatus(const string&);

    string serialize() const;
    static Appointment parse(const string&);
    void print() const;
};
