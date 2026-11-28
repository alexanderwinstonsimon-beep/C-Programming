#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "manager.h"

#define MAX_ADMIN 100
#define MAX_STAFF 200
#define MAX_REG 1000
#define MAX_ID 999999999
#define ADMIN_FILE "admin.txt"
#define INSTRUCTOR_FILE "instructor.txt"
#define REG_FILE "registration.txt"

// ...everything after main(), from "input helpers" down...

int managerMenu(){
int choice;
int loggedIn = 0;        // 1 after a successful login

do{
    printf("\n=============================\n");
    printf("|     MANAGER MAIN MENU     |\n");
    printf("| 1. manager login          |\n");
    printf("| 2. add admin              |\n");
    printf("| 3. view admins            |\n");
    printf("| 4. search admin           |\n");
    printf("| 5. update admin status    |\n");
    printf("| 6. staff report           |\n");
    printf("| 7. revenue report         |\n");
    printf("| 8. logout                 |\n");
    printf("| 0. exit                   |\n");
    printf("=============================\n");
    choice = readInt(" Enter your choice (0-8): ", 0, 8);

    // options 2-7 need a login first
    if (choice >= 2 && choice <= 7 && !loggedIn) {
        printf("Please login first (option 1)\n");
        continue;
    }

    switch (choice){
        case 1:
            if (loggedIn) {
                printf("Already logged in\n");
            } else {
                loggedIn = managerLogin();
            }
            break;
        case 2:
            addAdmin();
            break;
        case 3:
            viewAdmins();
            break;
        case 4:
            searchAdmin();
            break;
        case 5:
            updateAdminStatus();
            break;
        case 6:
            staffReportMenu();
            break;
        case 7:
            revenueReportMenu();
            break;
        case 8:
            if (loggedIn) {
                loggedIn = 0;
                managerLogout();
            } else {
                printf("You are not logged in\n");
            }
            break;
        case 0:
            printf("Goodbye\n");
            break;
    }
}while (choice != 0);
return 0;

}

//input helpers// 

// Reads a whole line, accepts only an int between min and max.
int readInt(const char *prompt, int min, int max) {
char buffer[100];
int value;

while (1) {
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return min;
    }
    if (buffer[0] == '\n') {
        continue;                    // skip blank line
    }
    if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
        return value;
    }
    printf("Invalid input. Enter a number from %d to %d.\n", min, max);
}

}

// Reads a whole line (spaces allowed) into dest, repeats until it is not empty.
void readString(const char *prompt, char *dest, int size) {
int c;

while (1) {
    printf("%s", prompt);
    if (fgets(dest, size, stdin) == NULL) {
        dest[0] = '\0';
        return;
    }
    if (strchr(dest, '\n') == NULL) {            // line was longer than size
        while ((c = getchar()) != '\n' && c != EOF);
    }
    dest[strcspn(dest, "\n")] = '\0';
    if (strlen(dest) > 0) {
        return;
    }
}

}

//login & logout// 

int managerLogin(void) {
int id, fileId;
char password[50], filePass[50];
char buffer[100];
int found = 0;

while (!found) {
    id = readInt("Enter ID (0 to cancel): ", 0, MAX_ID);
    if (id == 0) {
        printf("Login cancelled\n");
        return 0;
    }
    readString("Enter password: ", password, sizeof(password));

    FILE *fp = fopen("manager.txt", "r");
    if (fp == NULL) {
        printf("File error\n");
        return 0;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (sscanf(buffer, "%d %49s", &fileId, filePass) == 2
            && id == fileId && strcmp(password, filePass) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found)
        printf("Login Successful\n");
    else
        printf("Login Failed, check password or ID\n");
}
return 1;

}

void managerLogout(void) {
printf("Logout Successful\n");
}

// administrators//

void addAdmin(void) {
Admin list[MAX_ADMIN];
int count = loadAdmins(list);
Admin newAdmin;
int i, duplicate;

if (count >= MAX_ADMIN) {
    printf("Admin list is full\n");
    return;
}

/* ID: repeat until unique */
do {
    duplicate = 0;
    newAdmin.id = readInt("Enter new admin ID: ", 1, MAX_ID);
    for (i = 0; i < count; i++) {
        if (list[i].id == newAdmin.id) {
            printf("ID already exists\n");
            duplicate = 1;
            break;
        }
    }
} while (duplicate);

/* Name: spaces allowed, '|' is the file delimiter so it is rejected */
do {
    readString("Enter name: ", newAdmin.name, sizeof(newAdmin.name));
    if (strchr(newAdmin.name, '|') != NULL) {
        printf("Name cannot contain '|'\n");
    }
} while (strchr(newAdmin.name, '|') != NULL);

/* Password */
do {
    readString("Enter password: ", newAdmin.password, sizeof(newAdmin.password));
    if (strchr(newAdmin.password, '|') != NULL) {
        printf("Password cannot contain '|'\n");
    }
} while (strchr(newAdmin.password, '|') != NULL);

newAdmin.status = 1;                      /* new admins start active */

list[count] = newAdmin;
count++;
saveAdmins(list, count);

printf("Admin added successfully\n");

}

int loadAdmins(Admin list[]) {
FILE *fp = fopen(ADMIN_FILE, "r");
char buffer[200];
int count = 0;

if (fp == NULL) {
    return 0;                // no file yet = no records
}

while (count < MAX_ADMIN && fgets(buffer, sizeof(buffer), fp) != NULL) {
    if (sscanf(buffer, "%d|%49[^|]|%49[^|]|%d",
               &list[count].id,
               list[count].name,
               list[count].password,
               &list[count].status) == 4) {
        count++;
    }
}
fclose(fp);
return count;                // number of records loaded

}

void saveAdmins(Admin list[], int count) {
FILE *fp = fopen(ADMIN_FILE, "w");
int i;

if (fp == NULL) {
    printf("File error\n");
    return;
}

for (i = 0; i < count; i++) {
    fprintf(fp, "%d|%s|%s|%d\n",
            list[i].id, list[i].name, list[i].password, list[i].status);
}
fclose(fp);

}

// Prints a table of administrators (passwords are not shown).
void printAdminTable(Admin list[], int count) {
int i;

if (count == 0) {
    printf("\nNo administrators found.\n");
    return;
}

printf("\n%-8s %-25s %s\n", "ID", "Name", "Status");
printf("----------------------------------------\n");

for (i = 0; i < count; i++) {
    printf("%-8d %-25s %s\n",
           list[i].id, list[i].name,
           list[i].status ? "Active" : "Not active");
}

printf("----------------------------------------\n");
printf("Total: %d\n", count);

}

void viewAdmins(void) {
Admin list[MAX_ADMIN];
int count = loadAdmins(list);

printf("\n===== ADMINISTRATORS =====");
printAdminTable(list, count);

}

void searchAdmin(void) {
Admin list[MAX_ADMIN], result[MAX_ADMIN];
int count = loadAdmins(list);
int choice, id, i, found = 0;
char key[50];

printf("\n1. Search by ID\n2. Search by name\n");
choice = readInt("Enter choice: ", 1, 2);

if (choice == 1) {
    id = readInt("Enter admin ID: ", 1, MAX_ID);
    for (i = 0; i < count; i++) {
        if (list[i].id == id) {
            result[found] = list[i];
            found++;
        }
    }
} else {
    readString("Enter name to search: ", key, sizeof(key));
    for (i = 0; i < count; i++) {
        if (containsIgnoreCase(list[i].name, key)) {
            result[found] = list[i];
            found++;
        }
    }
}

printf("\n===== SEARCH RESULTS =====");
printAdminTable(result, found);

}

void updateAdminStatus(void) {
Admin list[MAX_ADMIN];
int count = loadAdmins(list);
int id, i, found = 0;

if (count == 0) {
    printf("No administrators found.\n");
    return;
}

id = readInt("Enter admin ID to update: ", 1, MAX_ID);

for (i = 0; i < count; i++) {
    if (list[i].id == id) {
        list[i].status = !list[i].status;
        found = 1;
        break;
    }
}

if (found) {
    saveAdmins(list, count);
    printf("Admin %d is now %s\n", id, list[i].status ? "Active" : "Not active");
} else {
    printf("Admin not found\n");
}

}

//staff report//

// File format: id|name|password|status
// Appends records to list starting at index count, returns new count.
int loadStaffFile(const char *filename, const char *role, Staff list[], int count) {
FILE *fp = fopen(filename, "r");
char buffer[200];

if (fp == NULL) {
    printf("Cannot open %s\n", filename);
    return count;
}

while (count < MAX_STAFF && fgets(buffer, sizeof(buffer), fp) != NULL) {
    if (sscanf(buffer, "%d|%49[^|]|%*[^|]|%d",
               &list[count].id,
               list[count].name,
               &list[count].status) == 3) {
        strcpy(list[count].role, role);
        count++;
    }
}
fclose(fp);
return count;

}

int loadStaff(Staff list[]) {
int count = 0;
count = loadStaffFile(ADMIN_FILE, "Administrator", list, count);
count = loadStaffFile(INSTRUCTOR_FILE, "Instructor", list, count);
return count;
}

void printStaffList(Staff list[], int count) {
int i, admins = 0, instructors = 0;

if (count == 0) {
    printf("No records found.\n");
    return;
}

printf("\n%-8s %-25s %-15s %s\n", "ID", "Name", "Role", "Status");
printf("------------------------------------------------------------\n");

for (i = 0; i < count; i++) {
    printf("%-8d %-25s %-15s %s\n",
           list[i].id, list[i].name, list[i].role,
           list[i].status ? "Active" : "Not active");

    if (strcmp(list[i].role, "Administrator") == 0) {
        admins++;
    } else {
        instructors++;
    }
}

printf("------------------------------------------------------------\n");
printf("Total administrators: %d\n", admins);
printf("Total instructors:    %d\n", instructors);
printf("Total staff:          %d\n", count);

}

// Case-insensitive check
int containsIgnoreCase(const char *text, const char *key) {
char t[100], k[100];
int i;

for (i = 0; text[i] != '\0' && i < 99; i++) {
    t[i] = tolower((unsigned char)text[i]);
}
t[i] = '\0';

for (i = 0; key[i] != '\0' && i < 99; i++) {
    k[i] = tolower((unsigned char)key[i]);
}
k[i] = '\0';

return strstr(t, k) != NULL;

}

void viewStaffReport(void) {
Staff list[MAX_STAFF];
int count = loadStaff(list);

printf("\n===== STAFF REPORT =====");
printStaffList(list, count);

}

void filterStaffByRole(void) {
Staff list[MAX_STAFF], result[MAX_STAFF];
int count = loadStaff(list);
int choice, i, found = 0;
const char *role;

printf("\n1. Administrators\n2. Instructors\n");
choice = readInt("Enter choice: ", 1, 2);
role = (choice == 1) ? "Administrator" : "Instructor";

for (i = 0; i < count; i++) {
    if (strcmp(list[i].role, role) == 0) {
        result[found] = list[i];
        found++;
    }
}

printf("\n===== %s LIST =====", role);
printStaffList(result, found);

}

void searchStaffByName(void) {
Staff list[MAX_STAFF], result[MAX_STAFF];
int count = loadStaff(list);
int i, found = 0;
char key[50];

readString("Enter name to search: ", key, sizeof(key));

for (i = 0; i < count; i++) {
    if (containsIgnoreCase(list[i].name, key)) {
        result[found] = list[i];
        found++;
    }
}

printf("\n===== SEARCH RESULTS FOR \"%s\" =====", key);
printStaffList(result, found);

}

void staffReportMenu(void) {
int choice;

do {
    printf("\n--- Staff Report ---\n");
    printf("1. View all staff\n");
    printf("2. Filter by role\n");
    printf("3. Search by name\n");
    printf("4. Back\n");
    choice = readInt("Enter choice: ", 1, 4);

    switch (choice) {
        case 1: viewStaffReport();
        break;
        case 2: filterStaffByRole();
        break;
        case 3: searchStaffByName();
        break;
        case 4:
        break;
    }
} while (choice != 4);

}

/* ---------- revenue report ---------- */

// File format per line: regId|classId|day|month|year|fee
// Example: 3001|12|15|3|2026|80.00
int loadRegistrations(Registration list[]) {
FILE *fp = fopen(REG_FILE, "r");
char buffer[200];
int count = 0;

if (fp == NULL) {
    printf("Cannot open %s\n", REG_FILE);
    return 0;
}

while (count < MAX_REG && fgets(buffer, sizeof(buffer), fp) != NULL) {
    if (sscanf(buffer, "%d|%d|%d|%d|%d|%lf",
               &list[count].regId,
               &list[count].classId,
               &list[count].day,
               &list[count].month,
               &list[count].year,
               &list[count].fee) == 6) {
        count++;
    }
}
fclose(fp);
return count;

}

void viewMonthlyRevenue(void) {
Registration list[MAX_REG];
int count = loadRegistrations(list);
int month, year, i, registrations = 0;
double total = 0;

month = readInt("Enter month (1-12): ", 1, 12);
year = readInt("Enter year: ", 2000, 2100);

for (i = 0; i < count; i++) {
    if (list[i].month == month && list[i].year == year) {
        registrations++;
        total += list[i].fee;
    }
}

printf("\n===== MONTHLY REVENUE REPORT (%02d/%d) =====\n", month, year);
printf("Class registrations: %d\n", registrations);
printf("Total revenue:       RM %.2f\n", total);

}

void viewYearlyRevenue(void) {
Registration list[MAX_REG];
int count = loadRegistrations(list);
int year, i, registrations = 0;
double total = 0;

year = readInt("Enter year: ", 2000, 2100);

for (i = 0; i < count; i++) {
    if (list[i].year == year) {
        registrations++;
        total += list[i].fee;
    }
}

printf("\n===== YEARLY REVENUE REPORT (%d) =====\n", year);
printf("Class registrations: %d\n", registrations);
printf("Total revenue:       RM %.2f\n", total);

}

void revenueReportMenu(void) {
int choice;

do {
    printf("\n--- Revenue Report ---\n");
    printf("1. Monthly revenue\n");
    printf("2. Yearly revenue\n");
    printf("3. Back\n");
    choice = readInt("Enter choice: ", 1, 3);

    switch (choice) {
        case 1: viewMonthlyRevenue();
        break;
        case 2: viewYearlyRevenue();
        break;
        case 3:
        break;
    }
} while (choice != 3);

}