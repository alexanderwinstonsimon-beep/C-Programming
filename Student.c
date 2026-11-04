#include <stdio.h>
#include <string.h>

#define MAX 50

#define STUDENT_FILE "students.txt"
#define CLASS_FILE   "classes.txt"
#define PAYMENT_FILE "payments.txt"
#define RATING_FILE  "ratings.txt"

char stuId[MAX][20];
char stuUser[MAX][30];
char stuPass[MAX][30];
int  stuCount = 0;

int  classId[MAX];
char classSubject[MAX][50];
char classInstructor[MAX][50];
char classTime[MAX][50];
char classBookedBy[MAX][20];
int  classCount = 0;

char payStuId[MAX][20];
int  payClassId[MAX];
float payAmount[MAX];
char payDate[MAX][20];
int  payCount = 0;

char rateStuId[MAX][20];
int  rateClassId[MAX];
int  rateScore[MAX];
char rateComment[MAX][100];
int  rateCount = 0;

char currentStudentId[20] = "";

int loadStudents(){
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) return 0;
    stuCount = 0;
    while (fscanf(fp, "%19[^|]|%29[^|]|%29[^\n]\n", stuId[stuCount], stuUser[stuCount], stuPass[stuCount]) == 3) {
        stuCount++;
        if (stuCount >= MAX) 
            break;
    }
    fclose(fp);
    return 1;
}

int loadClasses(){
    FILE *fp = fopen(CLASS_FILE, "r");
    if (!fp) return 0;
    classCount = 0;
    while (fscanf(fp, "%d|%49[^|]|%49[^|]|%49[^|]|%19[^\n]\n", &classId[classCount], classSubject[classCount], classInstructor[classCount], classTime[classCount], classBookedBy[classCount]) == 5) {
        classCount++;
        if (classCount >= MAX) 
            break;
    }
    fclose(fp);
    return 1;
}

int saveClasses(){
    FILE *fp = fopen(CLASS_FILE, "w");
    if (!fp) return 0;
    for (int i = 0; i < classCount; i++)
        fprintf(fp, "%d|%s|%s|%s|%s\n", classId[i], classSubject[i], classInstructor[i], classTime[i], classBookedBy[i]);
    fclose(fp);
    return 1;
}

int loadPayments(){
    FILE *fp = fopen(PAYMENT_FILE, "r");
    if (!fp) return 0;
    payCount = 0;
    while (fscanf(fp, "%19[^|]|%d|%f|%19[^\n]\n", payStuId[payCount], &payClassId[payCount], &payAmount[payCount], payDate[payCount]) == 4) {
        payCount++;
        if (payCount >= MAX) 
            break;
    }
    fclose(fp);
    return 1;
}

int savePayments(){
    FILE *fp = fopen(PAYMENT_FILE, "w");
    if (!fp) return 0;
    for (int i = 0; i < payCount; i++)
        fprintf(fp, "%s|%d|%.2f|%s\n", payStuId[i], payClassId[i], payAmount[i], payDate[i]);
    fclose(fp);
    return 1;
}

int loadRatings(){
    FILE *fp = fopen(RATING_FILE, "r");
    if (!fp) return 0;
    rateCount = 0;
    while (fscanf(fp, "%19[^|]|%d|%d|%99[^\n]\n", rateStuId[rateCount], &rateClassId[rateCount], &rateScore[rateCount], rateComment[rateCount]) == 4) {
        rateCount++;
        if (rateCount >= MAX) 
            break;
    }
    fclose(fp);
    return 1;
}

int saveRatings(){
    FILE *fp = fopen(RATING_FILE, "w");
    if (!fp) return 0;
    for (int i = 0; i < rateCount; i++)
        fprintf(fp, "%s|%d|%d|%s\n", rateStuId[i], rateClassId[i], rateScore[i], rateComment[i]);
    fclose(fp);
    return 1;
}

int login(){
    char user[30], pass[30];
    int attempts = 3;

    while (attempts > 0){
        printf("\n===== Student Login =====\n");
        printf("Username: ");
        scanf("%29s", user);
        printf("Password: ");
        scanf("%29s", pass);

        for (int i = 0; i < stuCount; i++){
            if (strcmp(stuUser[i], user) == 0 && strcmp(stuPass[i], pass) == 0){
                strcpy(currentStudentId, stuId[i]);
                printf("\nLogin successful. Welcome, %s (ID: %s)!\n", user, currentStudentId);
                return 1;
            }
        }
        attempts--;
        printf("\nInvalid username or password. Attempts left: %d\n", attempts);
    }
    printf("\nToo many failed attempts. Exiting.\n");
    return 0;
}

int bookClass();
int payment();
int rating();

int showBookedClasses(){
    int shown = 0;

    printf("\nYour Booked Classes (%s)\n", currentStudentId);
    printf("%-5s %-15s %-15s %-12s\n", "ID", "Subject", "Instructor", "Time");
    printf("--------------------------------------------------------------\n");

    for (int i = 0; i < classCount; i++){
        if (strcmp(classBookedBy[i], currentStudentId) == 0){
            printf("%-5d %-15s %-15s %-12s\n", classId[i], classSubject[i], classInstructor[i], classTime[i]);
            shown++;
        }
    }

    if (shown == 0)
        printf("(You have not booked any classes yet.)\n");

    return shown;
}

//main menu
int main(){
    int choice;

    if (!loadStudents()){
        printf("\n[Error] %s not found.\n", STUDENT_FILE);
        printf("Ask the administrator to register students first.\n");
        return 0;
    }
    if (!login()) return 0;

    loadClasses();
    loadPayments();
    loadRatings();

    do {
        printf("\nStudent Management System");
        printf("\nLogged in as: %s", currentStudentId);
        printf("\n---------------------------------");
        printf("\n1. Book Class Management\n");
        printf("2. Payment Management\n");
        printf("3. Instructor Rating\n");
        printf("4. Exit\n");

        printf("\nEnter choice: ");
        scanf("%d", &choice);
        switch(choice){
            case 1: 
                bookClass(); 
                break;
            case 2: 
                payment();   
                break;
            case 3:
                rating();    
                break;
            case 4: 
                printf("\nExiting the program.\n"); 
                break;
            default: 
                printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice != 4);
    return 0;
}

int bookClass(){
    int choice;

    do {
        printf("\nBook Class Management");
        printf("\n---------------------------------");
        printf("\n1. Book\n2. View\n3. Reschedule\n4. Cancel\n5. Back\n");

        printf("\nEnter choice: ");
        scanf("%d", &choice);
        switch(choice){
            case 1: {
                int id, found = -1;

                if (classCount == 0){
                    printf("\nNo classes available yet.\n");
                    break;
                }

                printf("\n%-5s %-15s %-15s %-12s %-22s\n", "ID", "Subject", "Instructor", "Time", "Status");
                printf("------------------------------------------------------------------------\n");
                for (int i = 0; i < classCount; i++){
                    char status[25];
                    if (strcmp(classBookedBy[i], "-") == 0) 
                        strcpy(status, "Available");
                    else if (strcmp(classBookedBy[i], currentStudentId) == 0) 
                        strcpy(status, "Booked by you");
                    else 
                        strcpy(status, "Booked");
                    printf("%-5d %-15s %-15s %-12s %-22s\n", classId[i], classSubject[i], classInstructor[i], classTime[i], status);
                }

                printf("\nEnter class ID to book: ");
                scanf("%d", &id);

                for (int i = 0; i < classCount; i++)
                    if (classId[i] == id){ 
                        found = i; break; 
                    }

                if (found == -1) 
                    printf("\nClass ID not found.\n");
                else if (strcmp(classBookedBy[found], "-") != 0) 
                    printf("\nClass already booked!\n");
                else
                    strcpy(classBookedBy[found], currentStudentId);
                    saveClasses();
                    printf("\nClass '%s' booked successfully!\n", classSubject[found]);
                
                break;
            }
            case 2: {
                // if (classCount == 0){
                //     printf("\nNo classes available yet.\n");
                //     break;
                // }

                // printf("\n%-5s %-15s %-15s %-12s %-22s\n", "ID", "Subject", "Instructor", "Time", "Status");
                // printf("------------------------------------------------------------------------\n");
                // for (int i = 0; i < classCount; i++){
                //     char status[25];
                //     if (strcmp(classBookedBy[i], "-") == 0) 
                //         strcpy(status, "Available");
                //     else if (strcmp(classBookedBy[i], currentStudentId) == 0) 
                //         strcpy(status, "Booked by you");
                //     else 
                //         strcpy(status, "Booked");
                //     printf("%-5d %-15s %-15s %-12s %-22s\n", classId[i], classSubject[i], classInstructor[i], classTime[i], status);
                // }
                showBookedClasses();
                break;
            }
            case 3: {
                int oldId, newId, oldIdx = -1, newIdx = -1;

                if (classCount == 0){ 
                    printf("\nNo classes available yet.\n"); 
                    break; 
                }

                if (showBookedClasses() == 0) 
                    break;

                printf("\nEnter class ID to reschedule: ");
                scanf("%d", &oldId);
                for (int i = 0; i < classCount; i++)
                    if (classId[i] == oldId && strcmp(classBookedBy[i], currentStudentId) == 0){
                        oldIdx = i; 
                        break;
                    }

                if (oldIdx == -1){ 
                    printf("\nYou have not booked this class.\n"); 
                    break; 
                }

                printf("\nAvailable classes to switch to:\n");
                printf("%-5s %-15s %-15s %-12s\n", "ID", "Subject", "Instructor", "Time");
                printf("--------------------------------------------------------------\n");
                int avail = 0;
                for (int i = 0; i < classCount; i++){
                    if (strcmp(classBookedBy[i], "-") == 0){
                        printf("%-5d %-15s %-15s %-12s\n", classId[i], classSubject[i], classInstructor[i], classTime[i]);
                        avail++;
                    }
                }
                if (avail == 0){ 
                    printf("(No available classes to switch to.)\n"); 
                    break; 
                }

                printf("\nEnter new class ID: ");
                scanf("%d", &newId);
                for (int i = 0; i < classCount; i++)
                    if (classId[i] == newId){ 
                        newIdx = i; 
                        break; 
                    }

                if (newIdx == -1){
                    printf("\nNew class ID not found.\n");
                }
                else if (strcmp(classBookedBy[newIdx], "-") != 0){
                    printf("\nNew class is already booked.\n");
                }
                else {
                    strcpy(classBookedBy[oldIdx], "-");
                    strcpy(classBookedBy[newIdx], currentStudentId);
                    saveClasses();
                    printf("\nRescheduled from '%s' to '%s'.\n", classSubject[oldIdx], classSubject[newIdx]);
                }
                break;
            }
            case 4: {
                int id, idx = -1;

                if (classCount == 0){ 
                    printf("\nNo classes available yet.\n"); 
                    break; 
                }

                if (showBookedClasses() == 0){
                    break;
                }

                printf("\nEnter class ID to cancel: ");
                scanf("%d", &id);
                for (int i = 0; i < classCount; i++)
                    if (classId[i] == id) { 
                        idx = i; 
                        break; 
                    }

                if (idx == -1) 
                    printf("\nClass ID not found.\n");
                else if (strcmp(classBookedBy[idx], currentStudentId) != 0)
                    printf("\nThis class is not booked by you.\n");
                else
                    strcpy(classBookedBy[idx], "-");
                    saveClasses();
                    printf("\nClass '%s' cancelled.\n", classSubject[idx]);
                break;
            }
            case 5:
                printf("\nReturning to main menu.\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice != 5);
    return 0;
}

int payment(){
    int choice;

    do {
        printf("\nPayment Management");
        printf("\n---------------------------------");
        printf("\n1. Make Payment\n2. View Payment History\n3. Back\n");

        printf("\nEnter choice: ");
        scanf("%d", &choice);
        switch(choice){
            case 1: {
                int id, found = 0;
                float currentPrice = 50.00;

                if (classCount == 0){
                    printf("\nNo classes available. You can only pay for booked classes.\n");
                    break;
                }

                printf("\nYour Booked Classes (%s)\n", currentStudentId);
                printf("%-5s %-15s %-15s %-12s %-10s\n", "ID", "Subject", "Instructor", "Time", "Price");
                printf("---------------------------------------------------------------------\n");
    
                int shown = 0;
                for (int i = 0; i < classCount; i++){
                    if (strcmp(classBookedBy[i], currentStudentId) == 0) {
                    printf("%-5d %-15s %-15s %-12s RM%-10.2f\n", classId[i], classSubject[i], classInstructor[i], classTime[i], currentPrice);
                    shown++;
                    }
                }

                if (shown == 0){
                    printf("(You have not booked any classes yet.)\n");
                    printf("\nBook a class first before making a payment.\n");
                    break;
                }

                printf("\nEnter class ID you are paying for (or 0 to cancel): ");
                scanf("%d", &id);

                if (id == 0){
                    printf("\nPayment cancelled. Returning to menu.\n");
                    break;
                }

                for (int i = 0; i < classCount; i++)
                    if (classId[i] == id && strcmp(classBookedBy[i], currentStudentId) == 0) {
                        found = 1; break;
                    }

                if (!found){
                    printf("\nYou can only pay for classes you have booked.\n");
                    break;
                }
                if (payCount >= MAX){ 
                    printf("\nPayment storage full.\n"); 
                    break; 
                }

                payAmount[payCount] = currentPrice;
    
                printf("Amount to pay: RM%.2f\n", payAmount[payCount]);
                printf("Enter date (DD/MM/YYYY): ");
                scanf("%19s", payDate[payCount]);

                strcpy(payStuId[payCount], currentStudentId);
                payClassId[payCount] = id;
                payCount++;
                savePayments();

                printf("\nPayment of RM%.2f recorded successfully.\n", currentPrice);
                break;
            }
            case 2: {
                int shown = 0;

                printf("\nYour Payment History (%s)\n", currentStudentId);
                printf("%-10s %-12s %-15s\n", "Class ID", "Amount", "Date");
                printf("----------------------------------------\n");
                for (int i = 0; i < payCount; i++) {
                    if (strcmp(payStuId[i], currentStudentId) == 0){
                        printf("%-10d RM%-10.2f %-15s\n", payClassId[i], payAmount[i], payDate[i]);
                        shown++;
                    }
                }
                if (shown == 0) 
                    printf("(No payment records for you yet.)\n");
                break;
            }
            case 3:
                printf("\nReturning to main menu.\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice != 3);
    return 0;
}

int rating(){
    int choice;

    do {
        printf("\nInstructor Rating");
        printf("\n---------------------------------");
        printf("\n1. Rate Instructors\n2. View Ratings\n3. Back\n");

        printf("\nEnter choice: ");
        scanf("%d", &choice);
        switch(choice){
            case 1: {
                int id, score, found = 0;

                if (classCount == 0){ 
                    printf("\nNo classes available.\n"); 
                    break; 
                }

                printf("\nYour Booked Classes (%s)\n", currentStudentId);
                printf("%-5s %-15s %-15s %-12s\n", "ID", "Subject", "Instructor", "Time");
                printf("--------------------------------------------------------------\n");
    
                int shown = 0;
                for (int i = 0; i < classCount; i++) {
                    if (strcmp(classBookedBy[i], currentStudentId) == 0) {
                        printf("%-5d %-15s %-15s %-12s\n", classId[i], classSubject[i], classInstructor[i], classTime[i]);
                        shown++;
                    }
                }

                if (shown == 0){
                    printf("(You have not booked any classes yet.)\n");
                    printf("\nYou must book a class before you can rate an instructor.\n");
                    break;
                }

                printf("\nEnter class ID to rate (or 0 to cancel/back): ");
                scanf("%d", &id);

                if (id == 0){
                    printf("\nRating cancelled. Returning to menu.\n");
                    break;
                }

                for (int i = 0; i < classCount; i++)
                    if (classId[i] == id && strcmp(classBookedBy[i], currentStudentId) == 0){ 
                        found = 1; 
                        break; 
                    }

                if (!found){ 
                    printf("\nYou can only rate classes you have booked.\n"); 
                    break; 
                }
    
                if (rateCount >= MAX){ 
                    printf("\nRating storage full.\n"); 
                    break; 
                }

                printf("Enter score (1-5): ");
                scanf("%d", &score);
                if (score < 1 || score > 5){ 
                    printf("\nInvalid score. Must be 1-5.\n"); 
                    break; 
                }

                printf("Enter comment: ");
                scanf(" %[^\n]", rateComment[rateCount]);

                strcpy(rateStuId[rateCount], currentStudentId);
                rateClassId[rateCount] = id;
                rateScore[rateCount]   = score;
                rateCount++;
                saveRatings();

                printf("\nThank you for your rating!\n");
                break;
            }
            case 2: {
                if (rateCount == 0){ 
                    printf("\nNo ratings yet.\n"); 
                    break; 
                }

                printf("\n%-8s %-8s %-7s %-30s\n", "Student", "Class ID", "Score", "Comment");
                printf("------------------------------------------------------------------\n");
                for (int i = 0; i < rateCount; i++)
                    printf("%-8s %-8d %-7d %-30s\n", rateStuId[i], rateClassId[i], rateScore[i], rateComment[i]);
                break;
            }
            case 3:
                printf("\nReturning to main menu.\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice != 3);
    return 0;
}