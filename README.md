# Veterinary Management System (C++)

A console-based **Veterinary Management System** written in C++ using object-oriented programming.
Vets, admins and staff can manage **pets, owners and appointments**, with role-based access,
input validation and records saved to text files between sessions.

**Module:** CMPC104 Software Development 2, Year 1 (Spring 2025), University of Roehampton. **Coursework mark: 83% (module 85%)**  
**Team:** Farhan Ahmed Sahol, Md Tamim Sarker, Fuad Hasan Noyon, Bany Amin

## Features
- Login with three roles: **Admin**, **Veterinarian**, **Staff**
- Pets: add, list, edit (vets/admin only), delete; vaccination status and medical notes
- Owners: add, list, edit, delete; phone and email validation
- Appointments: schedule, list, cancel; date (`YYYY-MM-DD`) and time (`HH:MM`) validation
- Persistence: pets, owners and appointments stored in `|`-separated `.txt` files and loaded at start-up

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

> **Note:** this commit is the code as submitted in April 2025. The sample data files were replaced with fictional records,
> and student ID numbers were removed from `Readme.txt`.
