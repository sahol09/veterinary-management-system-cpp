#include <iostream>
#include <string>
#include "VMSController.h"
#include "LoginSystem.h"  // If using Role-based system
using namespace std;

int main() {
    VMSController vms;
    LoginSystem login;

    string username, password;
    Role currentRole = Role::INVALID;

    cout << "=== Veterinary Management System ===" << endl;
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);

    currentRole = login.authenticate(username, password);

    if (currentRole == Role::INVALID) {
        cout << "Authentication failed. Exiting." << endl;
        return 0;
    }

    bool running = true;
    while (running) {
        cout << "\n---- MAIN MENU ----" << endl;
        cout << "1. Add Pet" << endl;
        cout << "2. List Pets" << endl;
        cout << "3. Edit Pet" << endl;
        cout << "4. Delete Pet" << endl;

        cout << "5. Add Owner" << endl;
        cout << "6. List Owners" << endl;
        cout << "7. Edit Owner" << endl;
        cout << "8. Delete Owner" << endl;

        cout << "9. Schedule Appointment" << endl;
        cout << "10. List Appointments" << endl;
        cout << "11. Cancel Appointment" << endl;

        cout << "12. Save & Exit" << endl;

        cout << "Select option: ";
        string choice;
        getline(cin, choice);

        if (choice == "1") vms.addPet();
        else if (choice == "2") vms.listPets();
        else if (choice == "3") {
            if (currentRole == Role::VET || currentRole == Role::ADMIN)
                vms.editPet();
            else
                cout << "Access denied: only vets/admin can edit pets." << endl;
        }
        else if (choice == "4") vms.deletePet();

        else if (choice == "5") vms.addOwner();
        else if (choice == "6") vms.listOwners();
        else if (choice == "7") vms.editOwner();
        else if (choice == "8") vms.deleteOwner();

        else if (choice == "9") vms.scheduleAppointment();
        else if (choice == "10") vms.listAppointments();
        else if (choice == "11") vms.cancelAppointment();

        else if (choice == "12") {
            vms.saveData();
            cout << "Saved successfully. Goodbye!" << endl;
            running = false;
        }
        else {
            cout << "Invalid option. Try again." << endl;
        }
    }

    return 0;
}
