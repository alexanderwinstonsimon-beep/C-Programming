# 🥋 Self Defence Academy Management System (SDAMS)

## Assignment Description

The **Self Defence Academy (SDA)** is a martial arts institution that offers various martial arts disciplines, including **Karate, Judo, Taekwondo, MMA**, and other styles. 

Currently, most of SDA's daily operations are managed manually, with information recorded and tracked using different methods. As there is no centralized management system, it is difficult for the academy to efficiently manage and track important information such as student records, classes, training schedules, and other daily operations.

To address these challenges, SDA has appointed you to develop a **Self Defence Academy Management System (SDAMS)**. This project challenges you to design and develop a SDAMS using your **C programming skills**. The system aims to centralize and simplify the management of the academy's daily operations.

> ⚠️ **Mandatory Rule:** All features listed below under each user role are mandatory requirements and represent the minimum expected functionality. Additional features beyond these requirements are encouraged and will earn marks under the *“Additional Features”* component of the assessment.

---

## User Roles & Functional Requirements

### 🧑‍💼 Manager
* **Authentication:** Login and logout from the system.
* **Manage Administrator (Add, View, Search, Update):** 
  * Add new administrator accounts.
  * Search for existing administrators by name or ID.
  * View administrator details.
  * Update administrator status from `'active'` to `'not active'`.
* **Staff Report (View, Search):** View details for administrators and instructors, with filtering capabilities by role and name, alongside total counts for each role.
* **Revenue Report (View, Search):** View and filter monthly or yearly revenue reports, including the total number of class registrations and overall revenue.

### 💻 Administrator
* **Authentication:** Login and logout from the system.
* **Manage Instructors (Add, View, Search, Update):** Create, view, and search existing instructor information. Filter records by instructor ID, name, role, or contact information. Update instructor status from `'active'` to `'not active'`.
* **Manage Students (Add, View, Search, Update):** Add, view, and filter the list of registered students by name, ID, or other relevant information. Update student status from `'active'` to `'not active'`.
* **Payment Records (View, Search):** View and filter past payment records made by students.

### 🧑‍🎓 Student
* **Authentication:** Login and logout from the system.
* **Book Class Management (Book, View, Reschedule, Cancel):** Book classes set up by the instructor. View, reschedule, and cancel bookings.
* **Payment Management (Add, View):** Make payments for registered classes and view personal past payment records.
* **Instructor Rating (Add, View):** Give and view feedback scoring rates based on the instructor's overall classes.

### 🥋 Instructor
* **Authentication:** Login and logout from the system.
* **Class Schedule Management (Add, View, Search, Update):** Manage training schedules and class availability, allowing students to book available classes conveniently.
* **Issue Facility Problem (Add, View):** Log facility issues such as damaged mats, broken equipment, lighting problems, air-conditioning issues, or damaged rooms.
* **View Instructor Rating (View, Search):** Monitor overall rating scores given by students.

### 🔧 Facilities Officer
* **Authentication:** Login and logout from the system.
* **Equipment Inventory Management (Add, View, Search, Update):** Manage the inventory by adding, viewing, searching, and updating new and existing martial arts equipment (e.g., mats, punching bags, gloves, uniforms).
* **Facility Problem Management (View, Search, Update):** View, search, and update resolution statuses for facility issues reported by instructors.
* **Equipment Inventory Stock Reports (View, Search):** View and search summary details on equipment stock levels. Filter reports by equipment ID or name.