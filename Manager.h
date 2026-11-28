#ifndef MANAGER_H
#define MANAGER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
int id;
char name[50];
char password[50];
int status;          
} Admin;

typedef struct {
int id;
char name[50];
char role[20];       
int status;          
} Staff;

typedef struct {
int regId;
int classId;
int day;
int month;
int year;
double fee;
} Registration;

int readInt(const char *prompt, int min, int max);
void readString(const char *prompt, char *dest, int size);

int managerLogin(void);
void managerLogout(void);

int loadAdmins(Admin list[]);
void saveAdmins(Admin list[], int count);
void addAdmin(void);
void printAdminTable(Admin list[], int count);
void viewAdmins(void);
void searchAdmin(void);
void updateAdminStatus(void);

int loadStaffFile(const char *filename, const char *role, Staff list[], int count);
int loadStaff(Staff list[]);
void printStaffList(Staff list[], int count);
int containsIgnoreCase(const char *text, const char *key);
void viewStaffReport(void);
void filterStaffByRole(void);
void searchStaffByName(void);
void staffReportMenu(void);

int loadRegistrations(Registration list[]);
void viewMonthlyRevenue(void);
void viewYearlyRevenue(void);
void revenueReportMenu(void);

int managerMenu();

#endif