#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Pet.h"
#include "Owner.h"
#include "Appointment.h"
using namespace std;

class VMSController {
private:
    vector<Pet> pets;
    vector<Owner> owners;
    vector<Appointment> appointments;

    int generatePetId() const;
    int generateOwnerId() const;
    int generateAppointmentId() const;

    // Lookups used to keep links between records valid
    Pet* findPet(int id);
    Owner* findOwner(int id);
    Appointment* findAppointment(int id);
    bool ownerExists(int id) const;
    bool petExists(int id) const;

    // Shared prompt for a valid future date and time
    void readAppointmentDateTime(string& date, string& time) const;

public:
    VMSController();

    // Pets
    void addPet();
    void listPets() const;
    void deletePet();
    void editPet();
    void viewPetAppointmentHistory() const;

    // Owners
    void addOwner();
    void listOwners() const;
    void deleteOwner();
    void editOwner();

    // Appointments
    void scheduleAppointment();
    void listAppointments() const;
    void modifyAppointment();
    void cancelAppointment();

    // File I/O
    void loadData();
    void saveData() const;
};

// ---- Input helpers (defined in VMSController.cpp, also used by main.cpp) ----
// Each helper re-prompts until the input is valid, so invalid input never
// leaves std::cin in a failed state or crashes the program.
string readLine(const string& prompt, bool allowEmpty = false);
int readInt(const string& prompt, int minValue, int maxValue);
bool readYesNo(const string& prompt);
