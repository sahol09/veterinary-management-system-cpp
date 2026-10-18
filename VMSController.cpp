#include "VMSController.h"
#include <fstream>
#include <sstream>
#include <limits>
#include <chrono>
#include <ctime>
bool isValidEmail(const string& email) {
    return email.find('@') != string::npos && email.find('.') != string::npos;
}

bool isValidPhone(const string& phone) {
    for (char c : phone) {
        if (!isdigit(c) && c != '+' && c != '-' && c != ' ') return false;
    }
    return phone.length() >= 7;
}

bool isValidDate(const string& date) {
    if (date.length() != 10) return false;
    return isdigit(date[0]) && isdigit(date[1]) && isdigit(date[2]) && isdigit(date[3])
        && date[4] == '-' && isdigit(date[5]) && isdigit(date[6])
        && date[7] == '-' && isdigit(date[8]) && isdigit(date[9]);
}

bool isValidTime(const string& time) {
    if (time.length() != 5) return false;
    return isdigit(time[0]) && isdigit(time[1]) && time[2] == ':' &&
        isdigit(time[3]) && isdigit(time[4]);
}
bool isWithinNext24Hours(const string& date, const string& time) {
    tm tm_input = {};
    if (sscanf((date + " " + time).c_str(), "%d-%d-%d %d:%d",
        &tm_input.tm_year, &tm_input.tm_mon, &tm_input.tm_mday,
        &tm_input.tm_hour, &tm_input.tm_min) != 5) {
        return false;
    }

    tm_input.tm_year -= 1900;
    tm_input.tm_mon -= 1;
    tm_input.tm_sec = 0;

    time_t input_time = mktime(&tm_input);
    time_t now = time(nullptr);

    if (input_time == -1 || difftime(input_time, now) <= 0) return false;

    double hoursAhead = difftime(input_time, now) / 3600.0;
    return hoursAhead <= 24.0;
}

int VMSController::generatePetId() const {
    int maxId = 0;
    for (auto& p : pets)
        if (p.getId() > maxId) maxId = p.getId();
    return maxId + 1;
}

int VMSController::generateOwnerId() const {
    int maxId = 0;
    for (auto& o : owners)
        if (o.getId() > maxId) maxId = o.getId();
    return maxId + 1;
}

int VMSController::generateAppointmentId() const {
    int maxId = 0;
    for (auto& a : appointments)
        if (a.getId() > maxId) maxId = a.getId();
    return maxId + 1;
}

VMSController::VMSController() {
    loadData();
}

void VMSController::addPet() {
    string name, breed, notes;
    int age, ownerId;
    char vacc;

    cout << "Enter name: ";
    getline(cin, name);
    cout << "Enter breed: ";
    getline(cin, breed);
    cout << "Enter age: ";
    cin >> age;
    cin.ignore();
    if (age < 0) age = 0;

    cout << "Vaccinated? (y/n): ";
    cin >> vacc;
    cin.ignore();
    bool vaccinated = (vacc == 'y' || vacc == 'Y');

    cout << "Enter owner ID (0 if none): ";
    cin >> ownerId;
    cin.ignore();

    cout << "Enter medical notes: ";
    getline(cin, notes);

    Pet newPet(generatePetId(), name, breed, age, vaccinated, ownerId, notes);
    pets.push_back(newPet);
    cout << "Pet added!\n";
}

void VMSController::listPets() const {
    for (const auto& pet : pets) pet.display();
}

void VMSController::deletePet() {
    int id;
    cout << "Enter Pet ID to delete: ";
    cin >> id;
    cin.ignore();

    for (auto it = pets.begin(); it != pets.end(); ++it) {
        if (it->getId() == id) {
            pets.erase(it);
            cout << "Pet deleted.\n";
            return;
        }
    }
    cout << "Pet not found.\n";
}

void VMSController::editPet() {
    int id;
    cout << "Enter Pet ID to update: ";
    cin >> id;
    cin.ignore();

    for (auto& pet : pets) {
        if (pet.getId() == id) {
            string name, breed, notes;
            int age;
            char vacc;

            cout << "New name (" << pet.getName() << "): ";
            getline(cin, name);
            cout << "New breed (" << pet.getBreed() << "): ";
            getline(cin, breed);
            cout << "New age (" << pet.getAge() << "): ";
            cin >> age;
            cin.ignore();
            if (age < 0) age = 0;
            cout << "Vaccinated? (y/n): ";
            cin >> vacc;
            cin.ignore();
            cout << "Medical notes: ";
            getline(cin, notes);

            pet.setName(name);
            pet.setBreed(breed);
            pet.setAge(age);
            pet.setVaccinated(vacc == 'y' || vacc == 'Y');
            pet.setMedicalNotes(notes);
            cout << "Pet updated.\n";
            return;
        }
    }
    cout << "Pet not found.\n";
}

void VMSController::addOwner() {
    string name, addr, phone, email;

    cout << "Owner name: ";
    getline(cin, name);
    cout << "Address: ";
    getline(cin, addr);

    do {
        cout << "Phone: ";
        getline(cin, phone);
        if (!isValidPhone(phone))
            cout << "Invalid phone number. Please use digits, +, -, or spaces.\n";
    } while (!isValidPhone(phone));

    do {
        cout << "Email: ";
        getline(cin, email);
        if (!isValidEmail(email))
            cout << "Invalid email format. Try again.\n";
    } while (!isValidEmail(email));

    Owner o(generateOwnerId(), name, addr, phone, email);
    owners.push_back(o);
    cout << "Owner added.\n";
}


void VMSController::listOwners() const {
    for (const auto& o : owners) o.showInfo();
}

void VMSController::deleteOwner() {
    int id;
    cout << "Enter Owner ID to delete: ";
    cin >> id;
    cin.ignore();

    for (auto it = owners.begin(); it != owners.end(); ++it) {
        if (it->getId() == id) {
            owners.erase(it);
            cout << "Owner deleted.\n";
            return;
        }
    }
    cout << "Owner not found.\n";
}

void VMSController::editOwner() {
    int id;
    cout << "Enter Owner ID to edit: ";
    cin >> id;
    cin.ignore();

    for (auto& o : owners) {
        if (o.getId() == id) {
            string name, addr, phone, email;
            cout << "New name: "; getline(cin, name);
            cout << "New address: "; getline(cin, addr);
            cout << "New phone: "; getline(cin, phone);
            cout << "New email: "; getline(cin, email);

            o.setName(name);
            o.setAddress(addr);
            o.setPhone(phone);
            o.setEmail(email);
            cout << "Owner updated.\n";
            return;
        }
    }
    cout << "Owner not found.\n";
}

void VMSController::scheduleAppointment() {
    int petId;
    string date, time;

    cout << "Pet ID: ";
    cin >> petId;
    cin.ignore();

    while (true) {
        cout << "Date (YYYY-MM-DD): ";
        getline(cin, date);
        cout << "Time (HH:MM): ";
        getline(cin, time);

        if (!isValidDate(date)) {
            cout << "Invalid date format. Use YYYY-MM-DD.\n";
            continue;
        }
        if (!isValidTime(time)) {
            cout << "Invalid time format. Use HH:MM.\n";
            continue;
        }
        if (!isWithinNext24Hours(date, time)) {
            cout << "Appointment must be within the next 24 hours and not in the past.\n";
            continue;
        }

        break;  // Valid input
    }

    Appointment a(generateAppointmentId(), petId, date, time, "Scheduled");
    appointments.push_back(a);
    cout << "Appointment scheduled.\n";
}
void VMSController::listAppointments() const {
    for (const auto& a : appointments) a.print();
}

void VMSController::cancelAppointment() {
    int id;
    cout << "Appointment ID to cancel: ";
    cin >> id; cin.ignore();

    for (auto it = appointments.begin(); it != appointments.end(); ++it) {
        if (it->getId() == id) {
            appointments.erase(it);
            cout << "Appointment canceled.\n";
            return;
        }
    }
    cout << "Not found.\n";
}

void VMSController::loadData() {
    ifstream pf("pets.txt"), of("owners.txt"), af("appointments.txt");
    string line;
    while (getline(pf, line)) if (!line.empty()) pets.push_back(Pet::fromString(line));
    while (getline(of, line)) if (!line.empty()) owners.push_back(Owner::fromString(line));
    while (getline(af, line)) if (!line.empty()) appointments.push_back(Appointment::parse(line));
}

void VMSController::saveData() {
    ofstream pf("pets.txt"), of("owners.txt"), af("appointments.txt");
    for (const auto& p : pets) pf << p.toString() << "\n";
    for (const auto& o : owners) of << o.toString() << "\n";
    for (const auto& a : appointments) af << a.serialize() << "\n";
}
