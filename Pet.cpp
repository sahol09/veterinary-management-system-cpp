#include "Pet.h"
#include <sstream>
#include <vector>
using namespace std;

Pet::Pet() : id(0), name(""), breed(""), age(0), medicalNotes(""), vaccinated(false), ownerId(0) {}

Pet::Pet(int id, const string& name, const string& breed, int age, bool vaccinated, int ownerId, const string& notes)
    : id(id), name(name), breed(breed), age(age), vaccinated(vaccinated), ownerId(ownerId), medicalNotes(notes) {
}

int Pet::getId() const { return id; }
string Pet::getName() const { return name; }
string Pet::getBreed() const { return breed; }
int Pet::getAge() const { return age; }
bool Pet::isVaccinated() const { return vaccinated; }
int Pet::getOwnerId() const { return ownerId; }
string Pet::getMedicalNotes() const { return medicalNotes; }

void Pet::setName(const string& n) { name = n; }
void Pet::setBreed(const string& b) { breed = b; }
void Pet::setAge(int a) { age = a >= 0 ? a : 0; }
void Pet::setVaccinated(bool v) { vaccinated = v; }
void Pet::setOwnerId(int id) { ownerId = id; }
void Pet::setMedicalNotes(const string& notes) { medicalNotes = notes; }

string Pet::toString() const {
    ostringstream oss;
    oss << id << "|" << name << "|" << breed << "|" << age << "|" << (vaccinated ? "1" : "0") << "|" << ownerId << "|" << medicalNotes;
    return oss.str();
}

Pet Pet::fromString(const string& line) {
    stringstream ss(line);
    string item;
    vector<string> parts;
    while (getline(ss, item, '|')) parts.push_back(item);

    int id = stoi(parts[0]);
    string name = parts[1];
    string breed = parts[2];
    int age = stoi(parts[3]);
    bool vaccinated = parts[4] == "1";
    int ownerId = stoi(parts[5]);
    string notes = parts.size() > 6 ? parts[6] : "";

    return Pet(id, name, breed, age, vaccinated, ownerId, notes);
}

void Pet::display() const {
    cout << "ID: " << id << ", Name: " << name << ", Breed: " << breed
        << ", Age: " << age << ", Vaccinated: " << (vaccinated ? "Yes" : "No")
        << ", OwnerID: " << ownerId << ", Notes: " << medicalNotes << endl;
}
