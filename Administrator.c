#include "main.h"

#define ADMIN_MAX 100

char instId[ADMIN_MAX][20], instName[ADMIN_MAX][50], instUser[ADMIN_MAX][30];
char instPass[ADMIN_MAX][30], instRole[ADMIN_MAX][30], instContact[ADMIN_MAX][30];
char instStatus[ADMIN_MAX][20];
int instCount;

char stuIdAdmin[ADMIN_MAX][20], stuNameAdmin[ADMIN_MAX][50], stuUserAdmin[ADMIN_MAX][30];
char stuPassAdmin[ADMIN_MAX][30], stuStatusAdmin[ADMIN_MAX][20], stuContactAdmin[ADMIN_MAX][30];
int stuCountAdmin;

char payStudent[ADMIN_MAX][20], payDateAdmin[ADMIN_MAX][20];
int payClassAdmin[ADMIN_MAX], payCountAdmin;
float payAmountAdmin[ADMIN_MAX];

static void trim(char *text) {
    size_t n = strlen(text);
    while (n && (text[n-1] == '\n' || text[n-1] == '\r'))
        text[--n] = '\0';
}

static void inputLine(const char *prompt, char *text, size_t size) {
    printf("%s", prompt);
    if (fgets(text, (int)size, stdin) == NULL)
        text[0] = '\0';
    trim(text);
}

static int fields(char *line, char out[][100], int limit) {
    int count = 0;
    char *start = line, *pipe;
    while (count < limit) {
        pipe = strchr(start, '|');
        if (pipe) *pipe = '\0';
        strcpy(out[count++], start);
        if (!pipe) break;
        start = pipe + 1;
    }
    return count;
}

static int contains(const char *text, const char *needle) {
    char a[150], b[150];
    int i;
    if (needle[0] == '\0') return 1;
    for (i = 0; text[i] && i < 149; i++) a[i] = (char)tolower((unsigned char)text[i]);
    a[i] = '\0';
    for (i = 0; needle[i] && i < 149; i++) b[i] = (char)tolower((unsigned char)needle[i]);
    b[i] = '\0';
    return strstr(a, b) != NULL;
}

static int loadInstructors(void) {
    FILE *fp = fopen("instructors.txt", "r");
    char line[300], f[7][100];
    instCount = 0;
    if (!fp) return 0;
    while (fgets(line, sizeof(line), fp) && instCount < ADMIN_MAX) {
        trim(line);
        int n = fields(line, f, 7);
        if (n < 2) continue;
        strcpy(instId[instCount], f[0]);
        strcpy(instName[instCount], f[1]);
        if (n >= 7) {
            strcpy(instUser[instCount], f[2]); strcpy(instPass[instCount], f[3]);
            strcpy(instRole[instCount], f[4]); strcpy(instContact[instCount], f[5]);
            strcpy(instStatus[instCount], f[6][0] ? f[6] : "active");
        } else {
            strcpy(instUser[instCount], n >= 3 ? f[2] : "");
            strcpy(instPass[instCount], n >= 4 ? f[3] : "");
            strcpy(instRole[instCount], n >= 5 ? f[4] : "");
            strcpy(instContact[instCount], n >= 6 ? f[5] : "");
            strcpy(instStatus[instCount], "active");
        }
        instCount++;
    }
    fclose(fp);
    return 1;
}

static int saveInstructors(void) {
    FILE *fp = fopen("instructors.txt", "w");
    if (!fp) return 0;
    for (int i = 0; i < instCount; i++)
        fprintf(fp, "%s|%s|%s|%s|%s|%s|%s\n", instId[i], instName[i],
                instUser[i], instPass[i], instRole[i], instContact[i], instStatus[i]);
    fclose(fp);
    return 1;
}

static int loadAdminStudents(void) {
    FILE *fp = fopen("students.txt", "r");
    char line[300], f[6][100];
    stuCountAdmin = 0;
    if (!fp) return 0;
    while (fgets(line, sizeof(line), fp) && stuCountAdmin < ADMIN_MAX) {
        trim(line);
        int n = fields(line, f, 6);
        if (!n) continue;
        strcpy(stuIdAdmin[stuCountAdmin], f[0]);
        if (n == 3) {
            strcpy(stuNameAdmin[stuCountAdmin], f[1]);
            strcpy(stuUserAdmin[stuCountAdmin], f[1]);
            strcpy(stuPassAdmin[stuCountAdmin], f[2]);
            strcpy(stuStatusAdmin[stuCountAdmin], "active");
            strcpy(stuContactAdmin[stuCountAdmin], "");
        } else {
            strcpy(stuNameAdmin[stuCountAdmin], n >= 2 ? f[1] : "");
            strcpy(stuUserAdmin[stuCountAdmin], n >= 3 ? f[2] : "");
            strcpy(stuPassAdmin[stuCountAdmin], n >= 4 ? f[3] : "");
            strcpy(stuStatusAdmin[stuCountAdmin], n >= 5 && f[4][0] ? f[4] : "active");
            strcpy(stuContactAdmin[stuCountAdmin], n >= 6 ? f[5] : "");
        }
        stuCountAdmin++;
    }
    fclose(fp);
    return 1;
}

static int saveStudents(void) {
    FILE *fp = fopen("students.txt", "w");
    if (!fp) return 0;
    for (int i = 0; i < stuCountAdmin; i++)
        fprintf(fp, "%s|%s|%s|%s|%s|%s\n", stuIdAdmin[i], stuNameAdmin[i],
                stuUserAdmin[i], stuPassAdmin[i], stuStatusAdmin[i], stuContactAdmin[i]);
    fclose(fp);
    return 1;
}

static int loadAdminPayments(void) {
    FILE *fp = fopen("payments.txt", "r");
    char line[200], f[4][100];
    payCountAdmin = 0;
    if (!fp) return 0;
    while (fgets(line, sizeof(line), fp) && payCountAdmin < ADMIN_MAX) {
        trim(line);
        if (fields(line, f, 4) == 4) {
            strcpy(payStudent[payCountAdmin], f[0]);
            payClassAdmin[payCountAdmin] = atoi(f[1]);
            payAmountAdmin[payCountAdmin] = (float)atof(f[2]);
            strcpy(payDateAdmin[payCountAdmin], f[3]);
            payCountAdmin++;
        }
    }
    fclose(fp);
    return 1;
}

int administratorLogin(void) {
    char user[30], pass[30];
    int attempts = 3;
    while (attempts > 0) {
        inputLine("\n===== Administrator Login =====\nUsername: ", user, sizeof(user));
        inputLine("Password: ", pass, sizeof(pass));
        if (strcmp(user, "Alexander") == 0 && !strcmp(pass, "admin123")) {
            printf("\nLogin successful. Welcome, Administrator!\n");
            return 1;
        }
        printf("\nInvalid username or password. Attempts left: %d\n", --attempts);
    }
    printf("\nToo many failed attempts. Returning to main menu.\n");
    return 0;
}

static int findInstructor(const char *id) {
    for (int i = 0; i < instCount; i++) if (!strcmp(instId[i], id)) return i;
    return -1;
}

int manageInstructors(void) {
    int choice;
    loadInstructors();
    do {
        printf("\nManage Instructors\n1. Add\n2. View\n3. Search\n4. Update Status\n5. Back\nEnter choice: ");
        if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); choice = 0; }
        else while (getchar() != '\n');
        switch (choice) {
            case 1: {
                char id[20], name[50], user[30], pass[30], role[30], contact[30];
                inputLine("\nInstructor ID: ", id, sizeof(id));
                if (!id[0] || findInstructor(id) >= 0) { printf("\nInvalid or duplicate instructor ID.\n"); break; }
                inputLine("Name: ", name, sizeof(name)); inputLine("Username: ", user, sizeof(user));
                inputLine("Password: ", pass, sizeof(pass)); inputLine("Role/Discipline: ", role, sizeof(role));
                inputLine("Contact: ", contact, sizeof(contact));
                if (!name[0] || !user[0] || !pass[0]) { printf("\nName, username and password are required.\n"); break; }
                if (instCount >= ADMIN_MAX) { printf("\nInstructor storage full.\n"); break; }
                strcpy(instId[instCount], id); strcpy(instName[instCount], name);
                strcpy(instUser[instCount], user); strcpy(instPass[instCount], pass);
                strcpy(instRole[instCount], role); strcpy(instContact[instCount], contact);
                strcpy(instStatus[instCount], "active"); instCount++;
                printf(saveInstructors() ? "\nInstructor added successfully.\n" : "\nError: could not save instructor data.\n");
                break;
            }
            case 2:
                printf("\n%-12s %-22s %-18s %-16s %-16s\n", "ID", "Name", "Role", "Contact", "Status");
                for (int i = 0; i < instCount; i++)
                    printf("%-12s %-22s %-18s %-16s %-16s\n", instId[i], instName[i], instRole[i], instContact[i], instStatus[i]);
                if (!instCount) printf("(No instructor records.)\n");
                break;
            case 3: {
                int filter; char needle[100];
                printf("\nSearch by:\n1. ID\n2. Name\n3. Role\n4. Contact\nEnter choice: ");
                scanf("%d", &filter); while (getchar() != '\n'); inputLine("Search text: ", needle, sizeof(needle));
                int found = 0;
                for (int i = 0; i < instCount; i++) {
                    int match = (filter == 1) ? contains(instId[i], needle) :
                                (filter == 2) ? contains(instName[i], needle) :
                                (filter == 3) ? contains(instRole[i], needle) :
                                contains(instContact[i], needle);
                    if (match) {
                        printf("%-12s %-22s %-18s %-16s %-16s\n", instId[i], instName[i], instRole[i], instContact[i], instStatus[i]);
                        found++;
                    }
                }
                if (!found) printf("(No matching instructors.)\n");
                break;
            }
            case 4: {
                char id[20]; inputLine("\nInstructor ID to deactivate: ", id, sizeof(id));
                int i = findInstructor(id);
                if (i < 0) printf("\nInstructor ID not found.\n");
                else { strcpy(instStatus[i], "not active"); printf(saveInstructors() ? "\nStatus updated to 'not active'.\n" : "\nError: could not save data.\n"); }
                break;
            }
            case 5: break;
            default: printf("\nInvalid choice.\n");
        }
    } while (choice != 5);
    return 0;
}

static int findStudent(const char *id) {
    for (int i = 0; i < stuCountAdmin; i++) if (!strcmp(stuIdAdmin[i], id)) return i;
    return -1;
}

int manageStudents(void) {
    int choice;
    loadAdminStudents();
    do {
        printf("\nManage Students\n1. Add\n2. View\n3. Search\n4. Update Status\n5. Back\nEnter choice: ");
        if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); choice = 0; }
        else while (getchar() != '\n');
        switch (choice) {
            case 1: {
                char id[20], name[50], user[30], pass[30], contact[30];
                inputLine("\nStudent ID: ", id, sizeof(id));
                if (!id[0] || findStudent(id) >= 0) { printf("\nInvalid or duplicate student ID.\n"); break; }
                inputLine("Name: ", name, sizeof(name)); inputLine("Username: ", user, sizeof(user));
                inputLine("Password: ", pass, sizeof(pass)); inputLine("Contact: ", contact, sizeof(contact));
                if (!name[0] || !user[0] || !pass[0]) { printf("\nName, username and password are required.\n"); break; }
                if (stuCountAdmin >= ADMIN_MAX) { printf("\nStudent storage full.\n"); break; }
                strcpy(stuIdAdmin[stuCountAdmin], id); strcpy(stuNameAdmin[stuCountAdmin], name);
                strcpy(stuUserAdmin[stuCountAdmin], user); strcpy(stuPassAdmin[stuCountAdmin], pass);
                strcpy(stuStatusAdmin[stuCountAdmin], "active"); strcpy(stuContactAdmin[stuCountAdmin], contact);
                stuCountAdmin++;
                printf(saveStudents() ? "\nStudent added successfully.\n" : "\nError: could not save student data.\n");
                break;
            }
            case 2:
                printf("\n%-12s %-22s %-18s %-16s %-16s\n", "ID", "Name", "Username", "Status", "Contact");
                for (int i = 0; i < stuCountAdmin; i++)
                    printf("%-12s %-22s %-18s %-16s %-16s\n", stuIdAdmin[i], stuNameAdmin[i],
                           stuUserAdmin[i], stuStatusAdmin[i], stuContactAdmin[i]);
                if (!stuCountAdmin) printf("(No student records.)\n");
                break;
            case 3: {
                int filter; char needle[100];
                printf("\nSearch by:\n1. ID\n2. Name\n3. Username\n4. Contact\nEnter choice: ");
                scanf("%d", &filter); while (getchar() != '\n'); inputLine("Search text: ", needle, sizeof(needle));
                int found = 0;
                for (int i = 0; i < stuCountAdmin; i++) {
                    int match = (filter == 1) ? contains(stuIdAdmin[i], needle) :
                                (filter == 2) ? contains(stuNameAdmin[i], needle) :
                                (filter == 3) ? contains(stuUserAdmin[i], needle) :
                                contains(stuContactAdmin[i], needle);
                    if (match) {
                        printf("%-12s %-22s %-18s %-16s %-16s\n", stuIdAdmin[i], stuNameAdmin[i],
                               stuUserAdmin[i], stuStatusAdmin[i], stuContactAdmin[i]);
                        found++;
                    }
                }
                if (!found) printf("(No matching students.)\n");
                break;
            }
            case 4: {
                char id[20]; inputLine("\nStudent ID to deactivate: ", id, sizeof(id));
                int i = findStudent(id);
                if (i < 0) printf("\nStudent ID not found.\n");
                else { strcpy(stuStatusAdmin[i], "not active"); printf(saveStudents() ? "\nStatus updated to 'not active'.\n" : "\nError: could not save data.\n"); }
                break;
            }
            case 5: break;
            default: printf("\nInvalid choice.\n");
        }
    } while (choice != 5);
    return 0;
}

int viewPaymentRecords(void) {
    int choice;
    loadAdminPayments();
    do {
        printf("\nPayment Records\n1. View All\n2. Filter by Student ID\n3. Filter by Class ID\n4. Filter by Date\n5. Back\nEnter choice: ");
        if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); choice = 0; }
        else while (getchar() != '\n');
        if (choice >= 1 && choice <= 4) {
            char text[30] = ""; int classFilter = -1;
            if (choice == 2) inputLine("Student ID: ", text, sizeof(text));
            else if (choice == 3) { printf("Class ID: "); scanf("%d", &classFilter); while (getchar() != '\n'); }
            else if (choice == 4) inputLine("Date (DD/MM/YYYY): ", text, sizeof(text));
            printf("\n%-12s %-12s %-12s %-15s\n", "Student ID", "Class ID", "Amount", "Date");
            int found = 0;
            for (int i = 0; i < payCountAdmin; i++) {
                int match = choice == 1 || (choice == 2 && contains(payStudent[i], text)) ||
                            (choice == 3 && payClassAdmin[i] == classFilter) ||
                            (choice == 4 && contains(payDateAdmin[i], text));
                if (match) {
                    printf("%-12s %-12d RM%-9.2f %-15s\n", payStudent[i], payClassAdmin[i], payAmountAdmin[i], payDateAdmin[i]);
                    found++;
                }
            }
            if (!found) printf("(No matching payment records.)\n");
        } else if (choice != 5) printf("\nInvalid choice.\n");
    } while (choice != 5);
    return 0;
}

int administratorMenu(void) {
    int choice;
    if (!administratorLogin()) return 0;
    do {
        printf("\nAdministrator Menu\n1. Manage Instructors\n2. Manage Students\n3. Payment Records\n4. Logout\nEnter choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = 0;
        }
        else while (getchar() != '\n');
        switch (choice) {
            case 1: manageInstructors(); break;
            case 2: manageStudents(); break;
            case 3: viewPaymentRecords(); break;
            case 4: printf("\nLogged out successfully.\n"); break;
            default: printf("\nInvalid choice.\n");
        }
    } while (choice != 4);
    return 0;
}
