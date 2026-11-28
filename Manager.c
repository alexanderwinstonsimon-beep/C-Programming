#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "manager.h"

#define MAX_ADMIN 100
#define MAX_STAFF 200
#define MAX_REG 1000
#define MAX_ID 999999999
#define MANAGER_FILE "Text_Files/manager.txt"
#define ADMIN_FILE "Text_Files/admin.txt"
#define INSTRUCTOR_FILE "Text_Files/instructors.txt"
#define PAYMENT_FILE "Text_Files/payments.txt"

int managerMenu(){
int choice;
int loggedIn = 0;

do{
    printf("\nManager Management System");
    printf("\nLogged in: %s", loggedIn ? "Yes" : "No");
    printf("\n---------------------------------\n");
    printf("1. Manager Login\n");
    printf("2. Add Administrator\n");
    printf("3. View Administrators\n");
    printf("4. Search Administrators\n");
    printf("5. Update Administrator Status\n");
    printf("6. Staff Report\n");
    printf("7. Revenue Report\n");
    printf("8. Logout\n");
    printf("0. Exit\n");
    choice = readInt("\nEnter choice (0-8): ", 0, 8);

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

int readInt(const char *prompt, int min, int max) {
char buffer[100];
int value;

while (1) {
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return min;
    }
    if (buffer[0] == '\n') {
        continue;
    }
    if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
        return value;
    }
    printf("Invalid input. Enter a number from %d to %d.\n", min, max);
}

}

void readString(const char *prompt, char *dest, int size) {
int c;

while (1) {
    printf("%s", prompt);
    if (fgets(dest, size, stdin) == NULL) {
        dest[0] = '\0';
        return;
    }
    if (strchr(dest, '\n') == NULL) {
        while ((c = getchar()) != '\n' && c != EOF);
    }
    dest[strcspn(dest, "\n")] = '\0';
    if (strlen(dest) > 0) {
        return;
    }
}

}

static int parseManagerCredential(const char *line, int *id, char password[50]) {
    return sscanf(line, "%d|%49[^\r\n]", id, password) == 2 ||
           sscanf(line, "%d %49s", id, password) == 2;
}

static int createManagerAccount(void) {
int id;
char password[50];

printf("\nNo Manager account found. Set up the first account.\n");
id = readInt("Choose Manager ID: ", 1, MAX_ID);
do {
    readString("Create password: ", password, sizeof(password));
    if (strchr(password, '|') != NULL) {
        printf("Password cannot contain '|'\n");
    }
} while (strchr(password, '|') != NULL);

FILE *fp = fopen(MANAGER_FILE, "w");
if (fp == NULL) {
    printf("Unable to create %s\n", MANAGER_FILE);
    return 0;
}
int writeFailed = fprintf(fp, "%d|%s\n", id, password) < 0;
int closeFailed = fclose(fp) != 0;
if (writeFailed || closeFailed) {
    printf("Unable to save Manager credentials.\n");
    return 0;
}
printf("Manager account created. Login successful.\n");
return 1;
}

int managerLogin(void) {
int id, fileId;
char password[50], filePass[50];
char buffer[100];
int found = 0, hasCredential = 0;
FILE *fp = fopen(MANAGER_FILE, "r");

if (fp == NULL) {
    return createManagerAccount();
}

while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    if (parseManagerCredential(buffer, &fileId, filePass)) {
        hasCredential = 1;
        break;
    }
}
if (ferror(fp)) {
    fclose(fp);
    printf("Unable to read %s\n", MANAGER_FILE);
    return 0;
}
if (!hasCredential) {
    fclose(fp);
    return createManagerAccount();
}

while (!found) {
    printf("\nManager Login\n");
    printf("---------------------------------\n");
    id = readInt("Enter ID (0 to cancel): ", 0, MAX_ID);
    if (id == 0) {
        fclose(fp);
        printf("Login cancelled\n");
        return 0;
    }
    readString("Enter password: ", password, sizeof(password));

    rewind(fp);
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (parseManagerCredential(buffer, &fileId, filePass)
            && id == fileId && strcmp(password, filePass) == 0) {
            found = 1;
            break;
        }
    }

    if (found)
        printf("Login Successful\n");
    else
        printf("Login Failed, check password or ID\n");
}
fclose(fp);
return 1;
}

void managerLogout(void) {
printf("Logout Successful\n");
}

void addAdmin(void) {
Admin list[MAX_ADMIN];
int count = loadAdmins(list);
Admin newAdmin;
int i, duplicate;

if (count >= MAX_ADMIN) {
    printf("Admin list is full\n");
    return;
}

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

do {
    readString("Enter name: ", newAdmin.name, sizeof(newAdmin.name));
    if (strchr(newAdmin.name, '|') != NULL) {
        printf("Name cannot contain '|'\n");
    }
} while (strchr(newAdmin.name, '|') != NULL);

do {
    readString("Enter password: ", newAdmin.password, sizeof(newAdmin.password));
    if (strchr(newAdmin.password, '|') != NULL) {
        printf("Password cannot contain '|'\n");
    }
} while (strchr(newAdmin.password, '|') != NULL);

newAdmin.status = 1;

list[count] = newAdmin;
count++;
if (saveAdmins(list, count)) {
    printf("Admin added successfully\n");
} else {
    printf("Admin was not saved.\n");
}

}

int loadAdmins(Admin list[]) {
FILE *fp = fopen(ADMIN_FILE, "r");
char buffer[200];
int count = 0;

if (fp == NULL) {
    return 0;
}

while (count < MAX_ADMIN && fgets(buffer, sizeof(buffer), fp) != NULL) {
    char *firstSeparator;
    char *secondSeparator;

    buffer[strcspn(buffer, "\r\n")] = '\0';
    firstSeparator = strchr(buffer, '|');
    secondSeparator = firstSeparator ? strchr(firstSeparator + 1, '|') : NULL;

    if (secondSeparator == NULL &&
        sscanf(buffer, "%49[^|]|%29[^\r\n]",
               list[count].name, list[count].password) == 2) {
        int candidate = 1;
        int duplicate;
        do {
            duplicate = 0;
            for (int i = 0; i < count; i++) {
                if (list[i].id == candidate) {
                    duplicate = 1;
                    candidate++;
                    break;
                }
            }
        } while (duplicate);
        list[count].id = candidate;
        list[count].status = 1;
        count++;
    } else if (secondSeparator != NULL &&
               sscanf(buffer, "%d|%49[^|]|%29[^|]|%d",
                      &list[count].id,
                      list[count].name,
                      list[count].password,
                      &list[count].status) == 4) {
        count++;
    }
}
fclose(fp);
return count;

}

int saveAdmins(Admin list[], int count) {
FILE *fp = fopen(ADMIN_FILE, "w");
int i;

if (fp == NULL) {
    printf("File error\n");
    return 0;
}

for (i = 0; i < count; i++) {
    if (fprintf(fp, "%d|%s|%s|%d\n",
                list[i].id, list[i].name, list[i].password, list[i].status) < 0) {
        fclose(fp);
        printf("File error\n");
        return 0;
    }
}
if (fclose(fp) != 0) {
    printf("File error\n");
    return 0;
}
return 1;
}

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
        if (!list[i].status) {
            printf("Admin %d is already not active.\n", id);
            return;
        }
        list[i].status = 0;
        found = 1;
        break;
    }
}

if (found) {
    if (saveAdmins(list, count)) {
        printf("Admin %d is now Not active\n", id);
    } else {
        printf("Admin status was not saved.\n");
    }
} else {
    printf("Admin not found\n");
}

}

int loadStaffFile(const char *filename, const char *role, Staff list[], int count) {
if (strcmp(role, "Administrator") == 0) {
    Admin admins[MAX_ADMIN];
    int adminCount = loadAdmins(admins);

    for (int i = 0; i < adminCount && count < MAX_STAFF; i++) {
        snprintf(list[count].id, sizeof(list[count].id), "%d", admins[i].id);
        strcpy(list[count].name, admins[i].name);
        strcpy(list[count].role, role);
        strcpy(list[count].status, admins[i].status ? "active" : "not active");
        count++;
    }
    return count;
}

FILE *fp = fopen(filename, "r");
char buffer[300];
char password[30], contact[30];

if (fp == NULL) {
    printf("Cannot open %s\n", filename);
    return count;
}

while (count < MAX_STAFF && fgets(buffer, sizeof(buffer), fp) != NULL) {
    buffer[strcspn(buffer, "\r\n")] = '\0';
    if (sscanf(buffer, "%19[^|]|%49[^|]|%29[^|]|%29[^|]|%19[^\r\n]",
               list[count].id,
               list[count].name,
               password,
               contact,
               list[count].status) == 5) {
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

printf("\n%-12s %-25s %-15s %s\n", "ID", "Name", "Role", "Status");
printf("------------------------------------------------------------\n");

for (i = 0; i < count; i++) {
    printf("%-12s %-25s %-15s %s\n",
           list[i].id, list[i].name, list[i].role,
           list[i].status);

    if (strcmp(list[i].role, "Administrator") == 0) {
        admins++;
    } else if (strcmp(list[i].role, "Instructor") == 0) {
        instructors++;
    }
}

if (count == 0) {
    printf("No records found.\n");
}

printf("------------------------------------------------------------\n");
printf("Total administrators: %d\n", admins);
printf("Total instructors:    %d\n", instructors);
printf("Total staff:          %d\n", count);

}

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
    printf("\nStaff Report");
    printf("\n---------------------------------\n");
    printf("1. View all staff\n");
    printf("2. Filter by role\n");
    printf("3. Search by name\n");
    printf("4. Back to Manager Menu\n");
    choice = readInt("\nEnter choice (1-4): ", 1, 4);

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

int loadRegistrations(Registration list[]) {
FILE *fp = fopen(PAYMENT_FILE, "r");
char buffer[200];
int count = 0;

if (fp == NULL) {
    printf("Cannot open %s\n", PAYMENT_FILE);
    return -1;
}

while (count < MAX_REG && fgets(buffer, sizeof(buffer), fp) != NULL) {
    if (sscanf(buffer, "%19[^|]|%d|%lf|%d/%d/%d",
               list[count].studentId,
               &list[count].classId,
               &list[count].fee,
               &list[count].day,
               &list[count].month,
               &list[count].year) == 6) {
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

if (count < 0) {
    return;
}

month = readInt("Enter month (1-12): ", 1, 12);
year = readInt("Enter year: ", 2000, 2100);

for (i = 0; i < count; i++) {
    if (list[i].month == month && list[i].year == year) {
        registrations++;
        total += list[i].fee;
    }
}

printf("\n===== MONTHLY REVENUE REPORT (%02d/%d) =====\n", month, year);
printf("Paid class registrations: %d\n", registrations);
printf("Total revenue:            RM %.2f\n", total);

}

void viewYearlyRevenue(void) {
Registration list[MAX_REG];
int count = loadRegistrations(list);
int year, i, registrations = 0;
double total = 0;

if (count < 0) {
    return;
}

year = readInt("Enter year: ", 2000, 2100);

for (i = 0; i < count; i++) {
    if (list[i].year == year) {
        registrations++;
        total += list[i].fee;
    }
}

printf("\n===== YEARLY REVENUE REPORT (%d) =====\n", year);
printf("Paid class registrations: %d\n", registrations);
printf("Total revenue:            RM %.2f\n", total);

}

void revenueReportMenu(void) {
int choice;

do {
    printf("\nRevenue Report");
    printf("\n---------------------------------\n");
    printf("1. Monthly revenue\n");
    printf("2. Yearly revenue\n");
    printf("3. Back to Manager Menu\n");
    choice = readInt("\nEnter choice (1-3): ", 1, 3);

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