#include "Appointment.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

Appointment::Appointment() : id(0), petId(0), date(""), time(""), status("Scheduled") {}

Appointment::Appointment(int i, int pId, const string& d, const string& t, const string& s)
    : id(i), petId(pId), date(d), time(t), status(s) {
}

int Appointment::getId() const {
    return id;
}

int Appointment::getPetId() const {
    return petId;
}

string Appointment::getDate() const {
    return date;
}

string Appointment::getTime() const {
    return time;
}

string Appointment::getStatus() const {
    return status;
}

void Appointment::setDate(const string& d) {
    date = d;
}

void Appointment::setTime(const string& t) {
    time = t;
}

void Appointment::setStatus(const string& s) {
    status = s;
}

string Appointment::serialize() const {
    return to_string(id) + "|" + to_string(petId) + "|" + date + "|" + time + "|" + status;
}

Appointment Appointment::parse(const string& line) {
    stringstream ss(line);
    string part;
    vector<string> tokens;
    while (getline(ss, part, '|')) {
        tokens.push_back(part);
    }
    // Guard against short/corrupt lines (operator[] would read out of bounds)
    if (tokens.size() < 5) throw runtime_error("Invalid appointment record");

    int id = stoi(tokens[0]);
    int petId = stoi(tokens[1]);
    string date = tokens[2];
    string time = tokens[3];
    string status = tokens[4];

    return Appointment(id, petId, date, time, status);
}

void Appointment::print() const {
    cout << "Appointment ID: " << id << ", Pet ID: " << petId
        << ", Date: " << date << ", Time: " << time
        << ", Status: " << status << endl;
}
