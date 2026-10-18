# Veterinary Management System (C++)

A console-based **Veterinary Management System** written in C++ using object-oriented programming.
Vets, admins and staff can manage **pets, owners and appointments**, with role-based access,
input validation and records saved to text files between sessions.

**Module:** CMPC104 Software Development 2, Year 1 (Spring 2025), University of Roehampton. **Coursework mark: 83% (module 85%)**  
**Team:** Farhan Ahmed Sahol, Md Tamim Sarker, Fuad Hasan Noyon, Bany Amin

## Features
- Login with three roles: **Admin**, **Veterinarian**, **Staff** (three attempts), with role checks on every restricted action
- Pets: add, list, edit (vets/admin), delete (admin), per-pet appointment history
- Owners: add, list, edit, delete (admin); phone and email validation on add *and* edit
- Appointments: schedule, list, **modify**, cancel (record kept with status `Cancelled`); must be in the future and within 90 days
- Robust input: every number, y/n, date and time prompt re-asks on invalid input (`std::stoi` exceptions are caught)
- Persistence: `|`-separated `.txt` files loaded at start-up; corrupt lines are skipped with a warning instead of crashing

## Design
| Class | Responsibility |
|---|---|
| `Pet`, `Owner`, `Appointment` | Encapsulated data entities with getters/setters, `toString`/`serialize` and `fromString`/`parse` for file I/O |
| `VMSController` | Holds `std::vector`s of each entity; CRUD operations, ID generation, validation, load/save |
| `LoginSystem` | Credential lookup in a `std::map`, returns a `Role` (`enum class`) |
| `main.cpp` | Login, then the menu loop |

The system design flowchart (diagrams.net) is in [`Flowchart.html`](Flowchart.html).

## Build
**Visual Studio:** open `veterinary management system.vcxproj`, build (Ctrl+Shift+B) and run (Ctrl+F5).  
**g++:**
```bash
g++ -std=c++17 -Wall *.cpp -o vms && ./vms
```
Keep `pets.txt`, `owners.txt` and `appointments.txt` in the working directory.

## Demo logins
| Username | Password | Role |
|---|---|---|
| admin | admin123 | Admin |
| vet | vet123 | Veterinarian |
| staff | staff123 | Staff |

*Hard-coded for demonstration only.*

## Revisions after submission (Oct 2026)
The first commit in this repo is the code **as submitted** in April 2025 (sample data replaced with fictional records,
student IDs removed). The second commit fixes the problems found when reviewing it:

| Problem in the submitted version | Fix |
|---|---|
| **Did not compile:** in `isWithinNext24Hours` the parameter `time` hid the C function `time()`, so `time(nullptr)` failed. This is why the 24-hour rule "didn't work" in the demo | Parameter renamed; rule changed to *future and within 90 days* |
| No exception handling: typing letters at a number prompt broke `std::cin`, and a bad line in a data file crashed `std::stoi` | `readInt` / `readYesNo` / `readLine` helpers with `try/catch`; file lines parsed inside `try/catch`; length checks before indexing fields |
| Cancelling deleted the appointment | Status set to `Cancelled` so history is kept |
| No way to modify an appointment; no per-pet appointment history | `modifyAppointment()` and `viewPetAppointmentHistory()` added |
| Pets and appointments could point to owners/pets that did not exist | IDs checked on create; deleting an owner unlinks their pets; deleting a pet cancels its appointments |
| Owner edits skipped phone/email validation; `|` in text would corrupt the files | Same validation on edit; `|` rejected |
| Roles only enforced on "Edit Pet" | Deletes are admin-only; medical edits vet/admin-only |

Tested with g++ 13 (`-Wall -Wextra`, no warnings) using a scripted session covering invalid input, role denial, past dates and corrupt data lines.

---
*Shared as a portfolio piece after marking. Please do not reuse this work in your own assessed coursework.*
