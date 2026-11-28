#include "main.h"

#define ADMIN_MAX 100
#define INSTRUCTOR_FILE "Text_Files/instructors.txt"
#define STUDENT_FILE "Text_Files/students.txt"
#define PAYMENT_FILE "Text_Files/payments.txt"

typedef struct {
    char id[20];
    char name[50];
    char password[30];
    char contact[30];
    char status[20];
} InstructorRecord;

typedef struct {
    char id[20];
    char name[50];
    char password[30];
    char status[20];
} StudentRecord;

typedef struct {
    char studentId[20];
    int classId;
    float amount;
    char date[20];
} PaymentRecord;

InstructorRecord instructors[ADMIN_MAX];
int instCount;

StudentRecord students[ADMIN_MAX];
int stuCountAdmin;

PaymentRecord payments[ADMIN_MAX];
int payCountAdmin;

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


//Manage Instructors
static int saveInstructors(void) {
    FILE *fp = fopen(INSTRUCTOR_FILE, "w");
    if (!fp) return 0;
    for (int i = 0; i < instCount; i++)
        fprintf(fp, "%s|%s|%s|%s|%s\n", instructors[i].id, instructors[i].name,
                instructors[i].password, instructors[i].contact, instructors[i].status);
    fclose(fp);
    return 1;
}

static int findInstructor(const char *id) {
    for (int i = 0; i < instCount; i++)
        if (!strcmp(instructors[i].id, id)) return i;
    return -1;
}

static int loadInstructors(void) {
    FILE *fp = fopen(INSTRUCTOR_FILE, "r");
    char line[300], f[7][100];
    instCount = 0;
    if (!fp) return 0;
    while (fgets(line, sizeof(line), fp) && instCount < ADMIN_MAX) {
        trim(line);
        int n = fields(line, f, 7);
        if (n != 5) continue;

        strcpy(instructors[instCount].id, f[0]);
        strcpy(instructors[instCount].name, f[1]);
        strcpy(instructors[instCount].password, f[2]);
        strcpy(instructors[instCount].contact, f[3]);
        strcpy(instructors[instCount].status, f[4]);

        instCount++;
    }
    fclose(fp);
    return 1;
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
                char id[20], name[50], pass[30], contact[30];
                if (instCount >= ADMIN_MAX) {
                    printf("\nMaximum instructor limit reached.\n");
                    break;
                }
                inputLine("\nInstructor ID: ", id, sizeof(id));
                if (!id[0] || findInstructor(id) >= 0) {
                    printf("\nInvalid or duplicate instructor ID.\n");
                    break;
                }
                inputLine("Name: ", name, sizeof(name));
                inputLine("Password: ", pass, sizeof(pass));
                inputLine("Contact: ", contact, sizeof(contact));
                if (!name[0] || !pass[0]) {
                    printf("\nName and password are required.\n");
                    break;
                }
                strcpy(instructors[instCount].id, id);
                strcpy(instructors[instCount].name, name);
                strcpy(instructors[instCount].password, pass);
                strcpy(instructors[instCount].contact, contact);
                strcpy(instructors[instCount].status, "active");
                instCount++;

                if (saveInstructors())
                    printf("\nInstructor added successfully.\n");
                else
                    printf("\nError: could not save instructor data.\n");
                break;
            }
            case 2:
                printf("\n%-12s %-22s %-16s %-16s\n", "ID", "Name", "Contact", "Status");
                for (int i = 0; i < instCount; i++)
                    printf("%-12s %-22s %-16s %-16s\n", instructors[i].id, instructors[i].name,
                           instructors[i].contact, instructors[i].status);
                if (!instCount) printf("(No instructor records.)\n");
                break;
            case 3: {
                int filter; char needle[100];
                printf("\nSearch by:\n1. ID\n2. Name\n3. Contact\nEnter choice: ");
                scanf("%d", &filter); while (getchar() != '\n'); inputLine("Search text: ", needle, sizeof(needle));
                int found = 0;
                for (int i = 0; i < instCount; i++) {
                    int match = (filter == 1) ? contains(instructors[i].id, needle) :
                                (filter == 2) ? contains(instructors[i].name, needle) :
                                (filter == 3) ? contains(instructors[i].contact, needle) :
                                0;
                    if (match) {
                        printf("%-12s %-22s %-16s %-16s\n", instructors[i].id, instructors[i].name,
                               instructors[i].contact, instructors[i].status);
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
                else { strcpy(instructors[i].status, "not active"); printf(saveInstructors() ? "\nStatus updated to 'not active'.\n" : "\nError: could not save data.\n"); }
                break;
            }
            case 5: break;
            default: printf("\nInvalid choice.\n");
        }
    } while (choice != 5);
    return 0;
}


//Manage Students
static int findStudent(const char *id) {
    for (int i = 0; i < stuCountAdmin; i++)
        if (!strcmp(students[i].id, id)) return i;
    return -1;
}

static int saveStudents(void) {
    FILE *fp = fopen(STUDENT_FILE, "w");
    if (!fp) return 0;
    for (int i = 0; i < stuCountAdmin; i++)
        fprintf(fp, "%s|%s|%s|%s\n", students[i].id, students[i].name,
                students[i].password, students[i].status);
    fclose(fp);
    return 1;
}

static int loadAdminStudents(void) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    char line[300], f[4][100];
    stuCountAdmin = 0;
    if (!fp) return 0;
    while (fgets(line, sizeof(line), fp) && stuCountAdmin < ADMIN_MAX) {
        trim(line);
        int n = fields(line, f, 4);
        if (n != 4) continue;
        strcpy(students[stuCountAdmin].id, f[0]);
        strcpy(students[stuCountAdmin].name, f[1]);
        strcpy(students[stuCountAdmin].password, f[2]);
        strcpy(students[stuCountAdmin].status, f[3]);
        stuCountAdmin++;
    }
    fclose(fp);
    return 1;
}

int manageStudents(void) {
    int choice;
    loadAdminStudents();
    do {
        printf("\nManage Students\n");
        printf("1. Add\n2. View\n3. Search\n4. Update Status\n5. Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = 0;
        }
        else {
            while (getchar() != '\n');
        }
        switch (choice) {
            case 1: {
                char id[20], name[50], pass[30];
                if (stuCountAdmin >= ADMIN_MAX) {
                    printf("\nStudent storage full.\n");
                    break;
                }
                inputLine("\nStudent ID: ", id, sizeof(id));
                if (!id[0] || findStudent(id) >= 0) {
                    printf("\nInvalid or duplicate student ID.\n");
                    break;
                }
                inputLine("Name: ", name, sizeof(name));
                inputLine("Password: ", pass, sizeof(pass));
                if (!name[0] || !pass[0]) {
                    printf("\nName and password are required.\n");
                    break;
                }
                strcpy(students[stuCountAdmin].id, id);
                strcpy(students[stuCountAdmin].name, name);
                strcpy(students[stuCountAdmin].password, pass);
                strcpy(students[stuCountAdmin].status, "active");
                stuCountAdmin++;
                if (saveStudents())
                    printf("\nStudent added successfully.\n");
                else
                    printf("\nError: could not save student data.\n");

                break;
            }
            case 2:
                printf("\n%-12s %-22s %-16s\n", "ID", "Name", "Status");
                for (int i = 0; i < stuCountAdmin; i++) {
                    printf("%-12s %-22s %-16s\n",
                           students[i].id,
                           students[i].name,
                           students[i].status);
                }
                if (!stuCountAdmin)
                    printf("(No student records.)\n");

                break;
            case 3: {
                int filter;
                char needle[100];
                printf("\nSearch by:\n");
                printf("1. ID\n");
                printf("2. Name\n");
                printf("Enter choice: ");
                if (scanf("%d", &filter) != 1) {
                    while (getchar() != '\n');
                    printf("\nInvalid search option.\n");
                    break;
                }
                while (getchar() != '\n');
                if (filter < 1 || filter > 2) {
                    printf("\nInvalid search option.\n");
                    break;
                }
                inputLine("Search text: ", needle, sizeof(needle));
                int found = 0;
                for (int i = 0; i < stuCountAdmin; i++) {
                    int match = (filter == 1) ?
                        contains(students[i].id, needle) :
                        contains(students[i].name, needle);
                    if (match) {
                        printf("%-12s %-22s %-16s\n",
                               students[i].id,
                               students[i].name,
                               students[i].status);
                        found++;
                    }
                }
                if (!found)
                    printf("(No matching students.)\n");
                break;
            }
            case 4: {
                char id[20];
                inputLine("\nStudent ID to deactivate: ", id, sizeof(id));
                int i = findStudent(id);
                if (i < 0) {
                    printf("\nStudent ID not found.\n");
                }
                else {
                    strcpy(students[i].status, "not active");
                    if (saveStudents())
                        printf("\nStatus updated to 'not active'.\n");
                    else
                        printf("\nError: could not save data.\n");
                }
                break;
            }
            case 5:
                break;
            default:
                printf("\nInvalid choice.\n");
        }
    } while (choice != 5);
    return 0;
}


//Manage Payments

static int loadAdminPayments(void) {
    FILE *fp = fopen(PAYMENT_FILE, "r");
    char line[200], f[4][100];
    payCountAdmin = 0;
    if (!fp) return 0;
    while (fgets(line, sizeof(line), fp) && payCountAdmin < ADMIN_MAX) {
        trim(line);
        if (fields(line, f, 4) == 4) {
            strcpy(payments[payCountAdmin].studentId, f[0]);
            payments[payCountAdmin].classId = atoi(f[1]);
            payments[payCountAdmin].amount = (float)atof(f[2]);
            strcpy(payments[payCountAdmin].date, f[3]);
            payCountAdmin++;
        }
    }
    fclose(fp);
    return 1;
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
                int match = choice == 1 || (choice == 2 && contains(payments[i].studentId, text)) ||
                            (choice == 3 && payments[i].classId == classFilter) ||
                            (choice == 4 && contains(payments[i].date, text));
                if (match) {
                    printf("%-12s %-12d RM%-9.2f %-15s\n", payments[i].studentId,
                           payments[i].classId, payments[i].amount, payments[i].date);
                    found++;
                }
            }
            if (!found) printf("(No matching payment records.)\n");
        } else if (choice != 5) printf("\nInvalid choice.\n");
    } while (choice != 5);
    return 0;
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
