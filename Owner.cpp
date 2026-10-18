#include "Owner.h"
#include <sstream>
#include <vector>
#include <stdexcept>
using namespace std;

Owner::Owner() : id(0), name(""), address(""), phone(""), email("") {}

Owner::Owner(int id, const string& n, const string& a, const string& p, const string& e)
    : id(id), name(n), address(a), phone(p), email(e) {
}

int Owner::getId() const { return id; }
string Owner::getName() const { return name; }
string Owner::getAddress() const { return address; }
string Owner::getPhone() const { return phone; }
string Owner::getEmail() const { return email; }

void Owner::setName(const string& n) { name = n; }
void Owner::setAddress(const string& a) { address = a; }
void Owner::setPhone(const string& p) { phone = p; }
void Owner::setEmail(const string& e) { email = e; }

string Owner::toString() const {
    return to_string(id) + "|" + name + "|" + address + "|" + phone + "|" + email;
}

Owner Owner::fromString(const string& line) {
    stringstream ss(line);
    string item;
    vector<string> data;
    while (getline(ss, item, '|')) data.push_back(item);
    if (data.size() < 5) throw runtime_error("Invalid owner record");

    int id = stoi(data[0]);
    return Owner(id, data[1], data[2], data[3], data[4]);
}

void Owner::showInfo() const {
    cout << "Owner ID: " << id << ", Name: " << name
        << ", Phone: " << phone << ", Email: " << email << endl;
}
