#include <iostream>
#include <string>
#include <stdexcept>
#include "VMSController.h"
#include "LoginSystem.h"
using namespace std;

// Role checks in one place so every menu option is enforced consistently.
//  - Admin: everything
//  - Veterinarian: pets incl. medical records, owners, appointments (no deletes)
//  - Staff: basic pet/owner data and appointments (no medical edits, no deletes)
static bool canEditMedical(Role r) { return r == Role::ADMIN || r == Role::VET; }
static bool canDelete(Role r) { return r == Role::ADMIN; }

static string roleName(Role r) {
    switch (r) {
    case Role::ADMIN: return "Admin";
    case Role::VET:   return "Veterinarian";
    case Role::STAFF: return "Staff";
    default:          return "Unknown";
    }
}

int main() {
    try {
        VMSController vms;
        LoginSystem login;

        cout << "=== Veterinary Management System ===" << endl;

        // Up to three login attempts.
        Role currentRole = Role::INVALID;
        for (int attempt = 1; attempt <= 3 && currentRole == Role::INVALID; ++attempt) {
            string username = readLine("Username: ");
            string password = readLine("Password: ");
            currentRole = login.authenticate(username, password);
            if (currentRole == Role::INVALID)
                cout << "Incorrect username or password (" << attempt << "/3)." << endl;
        }
        if (currentRole == Role::INVALID) {
            cout << "Too many failed attempts. Exiting." << endl;
            return 1;
        }
        cout << "Logged in as " << roleName(currentRole) << "." << endl;

        while (true) {
            cout << "\n---- MAIN MENU ----\n"
                 << " 1. Add Pet\n"
                 << " 2. List Pets\n"
                 << " 3. Edit Pet (vet/admin)\n"
                 << " 4. Delete Pet (admin)\n"
                 << " 5. View Pet Appointment History\n"
                 << " 6. Add Owner\n"
                 << " 7. List Owners\n"
                 << " 8. Edit Owner\n"
                 << " 9. Delete Owner (admin)\n"
                 << "10. Schedule Appointment\n"
                 << "11. List Appointments\n"
                 << "12. Modify Appointment\n"
                 << "13. Cancel Appointment\n"
                 << " 0. Save & Exit\n";

            int choice = readInt("Select option: ", 0, 13);
            switch (choice) {
            case 1:  vms.addPet(); break;
            case 2:  vms.listPets(); break;
            case 3:
                if (canEditMedical(currentRole)) vms.editPet();
                else cout << "Access denied: only vets/admin can edit pet records." << endl;
                break;
            case 4:
                if (canDelete(currentRole)) vms.deletePet();
                else cout << "Access denied: only admin can delete records." << endl;
                break;
            case 5:  vms.viewPetAppointmentHistory(); break;
            case 6:  vms.addOwner(); break;
            case 7:  vms.listOwners(); break;
            case 8:  vms.editOwner(); break;
            case 9:
                if (canDelete(currentRole)) vms.deleteOwner();
                else cout << "Access denied: only admin can delete records." << endl;
                break;
            case 10: vms.scheduleAppointment(); break;
            case 11: vms.listAppointments(); break;
            case 12: vms.modifyAppointment(); break;
            case 13: vms.cancelAppointment(); break;
            case 0:
                vms.saveData();
                cout << "Saved successfully. Goodbye!" << endl;
                return 0;
            }
        }
    }
    catch (const exception& e) {
        // Last line of defence, e.g. input stream closed (Ctrl+D / Ctrl+Z).
        cerr << "\nFatal error: " << e.what() << endl;
        return 1;
    }
}
