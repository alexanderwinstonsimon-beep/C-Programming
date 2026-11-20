#ifndef STUDENT_H
#define STUDENT_H

// Function declarations for the Student Management System
int login();
int bookClass();
int payment();
int rating();
int showBookedClasses();

// File loading and saving functions (optional to declare here, but good practice if used in other files)
int loadStudents();
int loadClasses();
int saveClasses();
int loadPayments();
int savePayments();
int loadRatings();
int saveRatings();

// The student menu function (which was previously int main() in std.c)
int studentMenu();

#endif