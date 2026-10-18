#pragma once
#include <iostream>
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

public:
    VMSController();

    // CRUD
    void addPet();
    void listPets() const;
    void deletePet();
    void editPet();

    void addOwner();
    void listOwners() const;
    void deleteOwner();
    void editOwner();

    void scheduleAppointment();
    void listAppointments() const;
    void cancelAppointment();

    // File I/O
    void loadData();
    void saveData();
};
