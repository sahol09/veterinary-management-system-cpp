#pragma once
#include <iostream>
#include <string>
using namespace std;

class Pet {
private:
    int id;
    string name;
    string breed;
    int age;
    string medicalNotes;
    bool vaccinated;
    int ownerId;

public:
    Pet();
    Pet(int, const string&, const string&, int, bool, int, const string&);

    int getId() const;
    string getName() const;
    string getBreed() const;
    int getAge() const;
    bool isVaccinated() const;
    int getOwnerId() const;
    string getMedicalNotes() const;

    void setName(const string&);
    void setBreed(const string&);
    void setAge(int);
    void setVaccinated(bool);
    void setOwnerId(int);
    void setMedicalNotes(const string&);

    string toString() const;
    static Pet fromString(const string&);
    void display() const;
};

