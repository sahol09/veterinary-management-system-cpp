-----------------------------------------
Veterinary Management System (VMS)
-----------------------------------------

👤 Student Name: Farhan Ahmed Sahol , Md Tamim Sarker, Fuad Hasan Noyon, Bany Amin
📚 Course: Software Development 2 (CMP020C105S)
🏫 University: University of Roehampton
📅 Semester: Spring 2025

-----------------------------------------
🧾 Overview:
-----------------------------------------
This project is a C++ console-based Veterinary Management System (VMS) built using object-oriented programming principles. It allows management of pets, owners, and appointments with file persistence, user role control, and input validation.

The project meets all coursework functional requirements and includes additional features like:
- Login system with role-based access
- Robust input validation (email, phone, date, time)
- Restriction on appointment dates (must be within the next 24 hours), that's how we wrote it but for some reason it didn't work but the teacher saw this during demonstration and marked accordingly.
- Persistent storage via text files

-----------------------------------------
📁 Files Included:
-----------------------------------------
- main.cpp
- VMSController.h / VMSController.cpp
- Pet.h / Pet.cpp
- Owner.h / Owner.cpp
- Appointment.h / Appointment.cpp
- LoginSystem.h / LoginSystem.cpp
- pets.txt (pet records)
- owners.txt (owner records)
- appointments.txt (appointment records)
- README.txt (this file)

-----------------------------------------
🔑 Login Credentials:
-----------------------------------------
These are hardcoded for testing purposes:

| Username | Password  | Role        |
|----------|-----------|-------------|
| admin    | admin123  | Admin       |
| vet      | vet123    | Veterinarian|
| staff    | staff123  | Staff       |

Admin has full access. Veterinarians can edit pet medical records. Staff can view/add/manage basic records and appointments.

-----------------------------------------
📦 How to Compile & Run (Visual Studio):
-----------------------------------------
1. Create a new Console Application Project in Visual Studio.
2. Add all `.cpp` and `.h` files to the project.
3. Place `pets.txt`, `owners.txt`, and `appointments.txt` in the **same folder as the .exe**.
4. Press Ctrl+Shift+B to Build, then Ctrl+F5 to Run.

-----------------------------------------
📌 Key Functionalities:
-----------------------------------------
✔ Add/Edit/Delete Pets & Owners  
✔ Schedule/Cancel Appointments  
✔ File persistence across sessions  
✔ Role-based access control  
✔ Validations:
   - Phone & email for owners
   - Date & time format for appointments
   - Appointment must be in the future and within 24 hours
   - No negative ages for pets

-----------------------------------------
📐 System Design Overview (for Demo):
-----------------------------------------
- Classes: Pet, Owner, Appointment, VMSController
- Uses OOP: Encapsulation, class separation, object persistence
- Data stored in `.txt` files using `|`-separated values
- Simple menu interface with condition-based logic

-----------------------------------------
📝 Notes:
-----------------------------------------
- The program is tested for robustness against invalid inputs
- All user actions are stored across sessions via `.txt` files
- ERD and flowchart was shown during the in-person demo


