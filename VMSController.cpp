#include "VMSController.h"
#include <fstream>
#include <sstream>
#include <limits>
#include <ctime>
#include <cctype>
#include <cstdio>
#include <stdexcept>

// ============================================================================
// Input helpers
// ============================================================================

static const string INVALID_INPUT = "The input you entered was invalid. ";

// Reads a whole line. Rejects '|' because it is the field separator in the
// data files (a '|' inside a name would corrupt the record on save).
string readLine(const string& prompt, bool allowEmpty) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) {
            throw runtime_error("Input stream closed.");
        }
        if (line.find('|') != string::npos) {
            cout << INVALID_INPUT << "The '|' character is not allowed.\n";
            continue;
        }
        if (line.empty() && !allowEmpty) {
            cout << INVALID_INPUT << "This field cannot be empty.\n";
            continue;
        }
        return line;
    }
}

// Reads an integer in [minValue, maxValue]. std::stoi throws on non-numeric
// text; the exception is caught and the user is asked again.
int readInt(const string& prompt, int minValue, int maxValue) {
    while (true) {
        string text = readLine(prompt);
        try {
            size_t used = 0;
            int value = stoi(text, &used);
            if (used != text.size()) throw invalid_argument("trailing characters");
            if (value < minValue || value > maxValue) throw out_of_range("outside allowed range");
            return value;
        }
        catch (const exception&) {
            cout << INVALID_INPUT << "Enter a whole number from " << minValue << " to " << maxValue << ".\n";
        }
    }
}

bool readYesNo(const string& prompt) {
    while (true) {
        string text = readLine(prompt);
        if (text == "y" || text == "Y") return true;
        if (text == "n" || text == "N") return false;
        cout << INVALID_INPUT << "Enter y or n.\n";
    }
}

// ============================================================================
// Validation
// ============================================================================

static bool isValidEmail(const string& email) {
    size_t at = email.find('@');
    if (at == string::npos || at == 0) return false;          // something before '@'
    size_t dot = email.find('.', at + 2);                     // a '.' after the domain name
    return dot != string::npos && dot + 1 < email.size();
}

static bool isValidPhone(const string& phone) {
    int digits = 0;
    for (char c : phone) {
        if (isdigit(static_cast<unsigned char>(c))) digits++;
        else if (c != '+' && c != '-' && c != ' ') return false;
    }
    return digits >= 7;
}

static bool isValidDate(const string& date) {
    if (date.length() != 10 || date[4] != '-' || date[7] != '-') return false;
    for (int i : {0, 1, 2, 3, 5, 6, 8, 9})
        if (!isdigit(static_cast<unsigned char>(date[i]))) return false;
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));
    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

static bool isValidTime(const string& time) {
    if (time.length() != 5 || time[2] != ':') return false;
    if (!isdigit(static_cast<unsigned char>(time[0])) || !isdigit(static_cast<unsigned char>(time[1])) ||
        !isdigit(static_cast<unsigned char>(time[3])) || !isdigit(static_cast<unsigned char>(time[4])))
        return false;
    int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(3, 2));
    return hour <= 23 && minute <= 59;
}

// Appointments must be in the future and no more than this many days ahead.
static const int MAX_DAYS_AHEAD = 90;

// FIX: the parameter used to be called `time`, which hid the C library
// function time() and stopped the file compiling. Renamed to timeText.
static bool isFutureWithinLimit(const string& date, const string& timeText) {
    tm when = {};
    if (sscanf((date + " " + timeText).c_str(), "%d-%d-%d %d:%d",
        &when.tm_year, &when.tm_mon, &when.tm_mday, &when.tm_hour, &when.tm_min) != 5) {
        return false;
    }
    when.tm_year -= 1900;
    when.tm_mon -= 1;
    when.tm_sec = 0;
    when.tm_isdst = -1;

    time_t appointmentTime = mktime(&when);
    time_t now = time(nullptr);
    if (appointmentTime == -1) return false;

    double secondsAhead = difftime(appointmentTime, now);
    return secondsAhead > 0 && secondsAhead <= MAX_DAYS_AHEAD * 24.0 * 3600.0;
}

// ============================================================================
// IDs and lookups
// ============================================================================

int VMSController::generatePetId() const {
    int maxId = 0;
    for (const auto& p : pets) if (p.getId() > maxId) maxId = p.getId();
    return maxId + 1;
}

int VMSController::generateOwnerId() const {
    int maxId = 0;
    for (const auto& o : owners) if (o.getId() > maxId) maxId = o.getId();
    return maxId + 1;
}

int VMSController::generateAppointmentId() const {
    int maxId = 0;
    for (const auto& a : appointments) if (a.getId() > maxId) maxId = a.getId();
    return maxId + 1;
}

Pet* VMSController::findPet(int id) {
    for (auto& p : pets) if (p.getId() == id) return &p;
    return nullptr;
}

Owner* VMSController::findOwner(int id) {
    for (auto& o : owners) if (o.getId() == id) return &o;
    return nullptr;
}

Appointment* VMSController::findAppointment(int id) {
    for (auto& a : appointments) if (a.getId() == id) return &a;
    return nullptr;
}

bool VMSController::ownerExists(int id) const {
    for (const auto& o : owners) if (o.getId() == id) return true;
    return false;
}

bool VMSController::petExists(int id) const {
    for (const auto& p : pets) if (p.getId() == id) return true;
    return false;
}

VMSController::VMSController() {
    loadData();
}

// ============================================================================
// Pets
// ============================================================================

void VMSController::addPet() {
    string name = readLine("Enter name: ");
    string breed = readLine("Enter breed: ");
    int age = readInt("Enter age (years): ", 0, 50);
    bool vaccinated = readYesNo("Vaccinated? (y/n): ");

    // FIX: the owner ID is now checked so a pet cannot point at a missing owner.
    int ownerId;
    while (true) {
        ownerId = readInt("Enter owner ID (0 if none): ", 0, numeric_limits<int>::max());
        if (ownerId == 0 || ownerExists(ownerId)) break;
        cout << "No owner with ID " << ownerId << ". Add the owner first or enter 0.\n";
    }
    string notes = readLine("Enter medical notes (optional): ", true);

    pets.push_back(Pet(generatePetId(), name, breed, age, vaccinated, ownerId, notes));
    cout << "Pet added!\n";
}

void VMSController::listPets() const {
    if (pets.empty()) { cout << "No pets on record.\n"; return; }
    for (const auto& pet : pets) pet.display();
}

void VMSController::deletePet() {
    int id = readInt("Enter Pet ID to delete: ", 1, numeric_limits<int>::max());
    for (auto it = pets.begin(); it != pets.end(); ++it) {
        if (it->getId() == id) {
            pets.erase(it);
            // Any future appointments for this pet can no longer go ahead.
            for (auto& a : appointments)
                if (a.getPetId() == id && a.getStatus() == "Scheduled") a.setStatus("Cancelled");
            cout << "Pet deleted (its scheduled appointments were cancelled).\n";
            return;
        }
    }
    cout << "Pet not found.\n";
}

void VMSController::editPet() {
    int id = readInt("Enter Pet ID to update: ", 1, numeric_limits<int>::max());
    Pet* pet = findPet(id);
    if (!pet) { cout << "Pet not found.\n"; return; }

    cout << "Press Enter to keep the current value.\n";
    string name = readLine("New name (" + pet->getName() + "): ", true);
    string breed = readLine("New breed (" + pet->getBreed() + "): ", true);
    string ageText = readLine("New age (" + to_string(pet->getAge()) + "): ", true);
    string vacc = readLine(string("Vaccinated? y/n (") + (pet->isVaccinated() ? "y" : "n") + "): ", true);
    string notes = readLine("Medical notes (" + pet->getMedicalNotes() + "): ", true);

    if (!name.empty()) pet->setName(name);
    if (!breed.empty()) pet->setBreed(breed);
    if (!ageText.empty()) {
        try {
            int age = stoi(ageText);
            if (age < 0 || age > 50) throw out_of_range("age");
            pet->setAge(age);
        }
        catch (const exception&) {
            cout << INVALID_INPUT << "Age left unchanged.\n";
        }
    }
    if (vacc == "y" || vacc == "Y") pet->setVaccinated(true);
    else if (vacc == "n" || vacc == "N") pet->setVaccinated(false);
    if (!notes.empty()) pet->setMedicalNotes(notes);
    cout << "Pet updated.\n";
}

void VMSController::viewPetAppointmentHistory() const {
    int id = readInt("Enter Pet ID: ", 1, numeric_limits<int>::max());
    if (!petExists(id)) { cout << "Pet not found.\n"; return; }
    bool any = false;
    for (const auto& a : appointments) {
        if (a.getPetId() == id) { a.print(); any = true; }
    }
    if (!any) cout << "No appointments for this pet.\n";
}

// ============================================================================
// Owners
// ============================================================================

void VMSController::addOwner() {
    string name = readLine("Owner name: ");
    string addr = readLine("Address: ");

    string phone;
    while (true) {
        phone = readLine("Phone: ");
        if (isValidPhone(phone)) break;
        cout << INVALID_INPUT << "Use at least 7 digits; +, - and spaces are allowed.\n";
    }

    string email;
    while (true) {
        email = readLine("Email: ");
        if (isValidEmail(email)) break;
        cout << INVALID_INPUT << "Use the format name@example.com.\n";
    }

    owners.push_back(Owner(generateOwnerId(), name, addr, phone, email));
    cout << "Owner added.\n";
}

void VMSController::listOwners() const {
    if (owners.empty()) { cout << "No owners on record.\n"; return; }
    for (const auto& o : owners) o.showInfo();
}

void VMSController::deleteOwner() {
    int id = readInt("Enter Owner ID to delete: ", 1, numeric_limits<int>::max());
    for (auto it = owners.begin(); it != owners.end(); ++it) {
        if (it->getId() == id) {
            owners.erase(it);
            // Unlink the owner's pets rather than leaving a dangling owner ID.
            for (auto& p : pets) if (p.getOwnerId() == id) p.setOwnerId(0);
            cout << "Owner deleted (their pets are now unlinked).\n";
            return;
        }
    }
    cout << "Owner not found.\n";
}

// FIX: editing an owner now applies the same phone/email validation as adding one.
void VMSController::editOwner() {
    int id = readInt("Enter Owner ID to edit: ", 1, numeric_limits<int>::max());
    Owner* o = findOwner(id);
    if (!o) { cout << "Owner not found.\n"; return; }

    cout << "Press Enter to keep the current value.\n";
    string name = readLine("New name (" + o->getName() + "): ", true);
    string addr = readLine("New address (" + o->getAddress() + "): ", true);

    string phone;
    while (true) {
        phone = readLine("New phone (" + o->getPhone() + "): ", true);
        if (phone.empty() || isValidPhone(phone)) break;
        cout << INVALID_INPUT << "Use at least 7 digits; +, - and spaces are allowed.\n";
    }
    string email;
    while (true) {
        email = readLine("New email (" + o->getEmail() + "): ", true);
        if (email.empty() || isValidEmail(email)) break;
        cout << INVALID_INPUT << "Use the format name@example.com.\n";
    }

    if (!name.empty()) o->setName(name);
    if (!addr.empty()) o->setAddress(addr);
    if (!phone.empty()) o->setPhone(phone);
    if (!email.empty()) o->setEmail(email);
    cout << "Owner updated.\n";
}

// ============================================================================
// Appointments
// ============================================================================

void VMSController::readAppointmentDateTime(string& date, string& time) const {
    while (true) {
        date = readLine("Date (YYYY-MM-DD): ");
        time = readLine("Time (HH:MM, 24-hour): ");
        if (!isValidDate(date)) { cout << INVALID_INPUT << "Use YYYY-MM-DD.\n"; continue; }
        if (!isValidTime(time)) { cout << INVALID_INPUT << "Use HH:MM between 00:00 and 23:59.\n"; continue; }
        if (!isFutureWithinLimit(date, time)) {
            cout << INVALID_INPUT << "The appointment must be in the future and within "
                 << MAX_DAYS_AHEAD << " days.\n";
            continue;
        }
        return;
    }
}

void VMSController::scheduleAppointment() {
    // FIX: the pet must exist before an appointment is booked for it.
    int petId = readInt("Pet ID: ", 1, numeric_limits<int>::max());
    if (!petExists(petId)) { cout << "Pet not found. Add the pet first.\n"; return; }

    string date, time;
    readAppointmentDateTime(date, time);

    appointments.push_back(Appointment(generateAppointmentId(), petId, date, time, "Scheduled"));
    cout << "Appointment scheduled.\n";
}

void VMSController::listAppointments() const {
    if (appointments.empty()) { cout << "No appointments on record.\n"; return; }
    for (const auto& a : appointments) a.print();
}

// NEW: the brief asked for appointments to be modifiable as well as cancelled.
void VMSController::modifyAppointment() {
    int id = readInt("Appointment ID to modify: ", 1, numeric_limits<int>::max());
    Appointment* a = findAppointment(id);
    if (!a) { cout << "Appointment not found.\n"; return; }
    if (a->getStatus() != "Scheduled") { cout << "Only scheduled appointments can be changed.\n"; return; }

    string date, time;
    readAppointmentDateTime(date, time);
    a->setDate(date);
    a->setTime(time);
    cout << "Appointment updated.\n";
}

// FIX: cancelling now keeps the record with status "Cancelled" (as the brief
// requires) instead of deleting it, so appointment history is preserved.
void VMSController::cancelAppointment() {
    int id = readInt("Appointment ID to cancel: ", 1, numeric_limits<int>::max());
    Appointment* a = findAppointment(id);
    if (!a) { cout << "Appointment not found.\n"; return; }
    if (a->getStatus() == "Cancelled") { cout << "Already cancelled.\n"; return; }
    a->setStatus("Cancelled");
    cout << "Appointment cancelled.\n";
}

// ============================================================================
// File I/O
// ============================================================================

// FIX: each line is parsed inside try/catch, so one corrupt line is reported
// and skipped instead of std::stoi throwing and crashing the program.
template <typename T, typename Parser>
static void loadFile(const string& filename, vector<T>& out, Parser parse) {
    ifstream in(filename);
    if (!in) return;   // first run: file does not exist yet
    string line;
    int lineNo = 0;
    while (getline(in, line)) {
        lineNo++;
        if (!line.empty() && line.back() == '\r') line.pop_back();   // files saved on Windows
        if (line.empty()) continue;
        try {
            out.push_back(parse(line));
        }
        catch (const exception&) {
            cerr << "Warning: skipped invalid record on line " << lineNo << " of " << filename << "\n";
        }
    }
}

void VMSController::loadData() {
    loadFile("pets.txt", pets, Pet::fromString);
    loadFile("owners.txt", owners, Owner::fromString);
    loadFile("appointments.txt", appointments, Appointment::parse);
}

void VMSController::saveData() const {
    ofstream pf("pets.txt"), of("owners.txt"), af("appointments.txt");
    if (!pf || !of || !af) {
        cerr << "Error: could not open data files for writing. Changes were not saved.\n";
        return;
    }
    for (const auto& p : pets) pf << p.toString() << "\n";
    for (const auto& o : owners) of << o.toString() << "\n";
    for (const auto& a : appointments) af << a.serialize() << "\n";
}
