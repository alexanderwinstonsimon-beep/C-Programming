#include<stdio.h>
#include<string.h>
#include "Instructor.h"
#include "main.h"
struct Class
{
    int classID;
    char subject[50];
    char instructorName[50];
    char time[50];
    char bookedBy[20];
};

struct FacilityProblem
{
    int problemID;
    char description[150];
    char status[20];
};

struct Rating
{
    int ratingID;
    char instructorName[50];
    char studentName[50];
    float ratingScore;
    char feedback[150];
};

void viewClassSchedule();
void classScheduleMenu();
void addClassSchedule();
void searchClassSchedule();
void updateClassSchedule();

void facilityProblemMenu();
void addFacilityProblem();
void viewFacilityProblems();

void viewInstructorRating();
char currentInstructorId[20];
char currentInstructorName[50];

int instructorLogin(void)
{
    char nameInput[50], pass[30];
    char line[300];
    int attempts = 3;

    while (attempts > 0)
    {
        printf("\n===== Instructor Login =====\n");
        printf("Name: ");
        scanf(" %49[^\n]", nameInput);
        printf("Password: ");
        scanf("%29s", pass);

        FILE *fp = fopen("Text_Files/instructors.txt", "r");
        if (fp == NULL)
        {
            printf("\nUnable to open instructor records.\n");
            return 0;
        }

        while (fgets(line, sizeof(line), fp))
        {
            char id[20], name[50], password[30], contact[30], status[20];

            if (sscanf(line, "%19[^|]|%49[^|]|%29[^|]|%29[^|]|%19[^\r\n]",
                       id, name, password, contact, status) == 5 &&
                strcmp(nameInput, name) == 0 &&
                strcmp(pass, password) == 0)
            {
                fclose(fp);
                if (strcmp(status, "active") != 0)
                {
                    printf("\nThis instructor account is not active.\n");
                    return 0;
                }

                strcpy(currentInstructorId, id);
                strcpy(currentInstructorName, name);
                printf("\nLogin successful. Welcome, %s (ID: %s)!\n",
                       name, id);
                return 1;
            }
        }

        fclose(fp);
        attempts--;
        printf("\nInvalid username or password. Attempts left: %d\n", attempts);
    }

    printf("\nToo many failed attempts. Returning to main menu.\n");
    return 0;
}

void readLine(char text[], int size)
{
    fgets(text, size, stdin);
    text[strcspn(text, "\n")] = '\0';
}
void instructorMenu()
{
    int choice;
    if (!instructorLogin())
        return;

    while (1)
    {
        printf("\n========== INSTRUCTOR MENU ==========\n");
        printf("1. Class Schedule Management\n");
        printf("2. Issue Facility Problem\n");
        printf("3. View Instructor Rating\n");
        printf("4. Logout\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                classScheduleMenu();
                break;

            case 2:
                facilityProblemMenu();
                break;

            case 3:
                viewInstructorRating();
                break;

            case 4:
                printf("\nLogging out...\n");
                return;

            default:
                printf("\nInvalid choice!\n");
        }
    }
}
void classScheduleMenu()
{
    int choice;

    while (1)
    {
        printf("\n========== CLASS SCHEDULE MANAGEMENT ==========\n");
        printf("1. Add Class Schedule\n");
        printf("2. View Class Schedule\n");
        printf("3. Search Class Schedule\n");
        printf("4. Update Class Schedule\n");
        printf("5. Back to Instructor Menu\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addClassSchedule();
                break;

            case 2:
                viewClassSchedule();
                break;

            case 3:
                searchClassSchedule();
                break;

            case 4:
                updateClassSchedule();
                break;

            case 5:
                return;

            default:
                printf("\nInvalid choice!\n");
        }
    }
}

void addClassSchedule()
{
    FILE *fp;
    struct Class c, temp;
    int duplicate = 0;
    
    printf("\nEnter Class ID: ");
    scanf("%d", &c.classID);

    printf("Enter Subject: ");
    scanf(" %49[^\n]", c.subject);

    strcpy(c.instructorName, currentInstructorName);
    printf("Instructor: %s\n", c.instructorName);

    printf("Enter Time: ");
    scanf(" %49[^\n]", c.time);

    strcpy(c.bookedBy, "-");

    // Check duplicate Class ID
    fp = fopen("Text_Files/classes.txt", "r");

    if (fp != NULL)
    {
        while (fscanf(fp,
            "%d|%49[^|]|%49[^|]|%49[^|]|%19[^\n]\n",
            &temp.classID,
            temp.subject,
            temp.instructorName,
            temp.time,
            temp.bookedBy) == 5)
        {
            if (temp.classID == c.classID)
            {
                duplicate = 1;
                break;
            }
        }

        fclose(fp);
    }

    if (duplicate == 1)
    {
        printf("\nClass ID already exists!\n");
        return;
    }

    // Save new class schedule
    fp = fopen("Text_Files/classes.txt", "a");

    if (fp == NULL)
    {
        printf("\nUnable to open file!\n");
        return;
    }

    fprintf(fp, "%d|%s|%s|%s|%s\n",
        c.classID,
        c.subject,
        c.instructorName,
        c.time,
        c.bookedBy);

    fclose(fp);

    printf("\nClass Schedule Added Successfully!\n");
}

void viewClassSchedule()
{
    FILE *fp;
    struct Class c;

    fp = fopen("Text_Files/classes.txt", "r");

    if (fp == NULL)
    {
        printf("\nNo class schedules found.\n");
        return;
    }

    printf("\n========== CLASS SCHEDULE ==========\n");
    printf("%-10s %-25s %-25s %-20s %-15s\n",
           "Class ID", "Subject", "Instructor", "Time", "Booked By");
    printf("------------------------------------------------------------------------------------------\n");

    while (fscanf(fp,
        "%d|%49[^|]|%49[^|]|%49[^|]|%19[^\n]\n",
        &c.classID,
        c.subject,
        c.instructorName,
        c.time,
        c.bookedBy) == 5)
    {
        printf("%-10d %-25s %-25s %-20s %-15s\n",
            c.classID, c.subject, c.instructorName, c.time, c.bookedBy);
    }

    fclose(fp);
}
void searchClassSchedule()
{
    FILE *fp;

    struct Class c;

    int searchID;
    int found = 0;

    printf("\nEnter Class ID to search: ");
    scanf("%d", &searchID);

    fp = fopen("Text_Files/classes.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    while (fscanf(fp,
        "%d|%49[^|]|%49[^|]|%49[^|]|%19[^\n]\n",
        &c.classID,
        c.subject,
        c.instructorName,
        c.time,
        c.bookedBy) == 5)
    {
        if (searchID == c.classID)
        {
            printf("\nClass Found!\n");

            printf("Class ID: %d\n", c.classID);
            printf("Subject: %s\n", c.subject);
            printf("Instructor: %s\n", c.instructorName);
            printf("Time: %s\n", c.time);
            printf("Booked By: %s\n", c.bookedBy);

            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found == 0)
    {
        printf("\nClass not found!\n");
    }
}
void updateClassSchedule()
{
    FILE *fp;
    FILE *tempFile;
    struct Class c;
    int searchID;
    int found = 0;

    printf("\nEnter Class ID to update: ");
    scanf("%d", &searchID);
    getchar(); // Clear the newline character from the input buffer

    fp = fopen("Text_Files/classes.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    tempFile = fopen("Text_Files/class_temp.txt", "w");

    if (tempFile == NULL)
    {
        printf("Unable to create temporary file!\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp,
        "%d|%49[^|]|%49[^|]|%49[^|]|%19[^\n]\n",
        &c.classID,
        c.subject,
        c.instructorName,
        c.time,
        c.bookedBy) == 5)
    {
        if (searchID == c.classID)
        {
            found = 1;
            printf("\nClass Found! Enter New Information.\n");

            printf("Enter New Subject: ");
            readLine(c.subject, sizeof(c.subject));

            printf("Enter New Time: ");
            readLine(c.time, sizeof(c.time));

            strcpy(c.instructorName, currentInstructorName);
        }

        fprintf(tempFile, "%d|%s|%s|%s|%s\n",
                c.classID,
                c.subject,
                c.instructorName,
                c.time,
                c.bookedBy);
    }

    fclose(fp);
    fclose(tempFile);

    if (found == 1)
    {
        remove("Text_Files/classes.txt");
        rename("Text_Files/class_temp.txt", "Text_Files/classes.txt");

        printf("\nClass Schedule Updated Successfully!\n");
    }
    else
    {
        remove("Text_Files/class_temp.txt");
        printf("\nClass ID not found!\n");
    }
}

void facilityProblemMenu()
{
    int choice;

    while (1)
    {
        printf("\n======= FACILITY PROBLEM MENU =======\n");
        printf("1. Add Facility Problem\n");
        printf("2. View Facility Problems\n");
        printf("3. Back to Instructor Menu\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addFacilityProblem();
                break;

            case 2:
                viewFacilityProblems();
                break;

            case 3:
                return;

            default:
                printf("\nInvalid choice!\n");
        }
    }
}

void addFacilityProblem()
{
    FILE *fp;

    struct FacilityProblem p;
    struct FacilityProblem temp;

    int duplicate = 0;

    printf("\n======= ADD FACILITY PROBLEM =======\n");

    printf("Enter Problem ID: ");
    scanf("%d", &p.problemID);
    getchar(); // Clear the newline character from the input buffer

    fp = fopen("Text_Files/facilityissues.txt", "r");

    if (fp != NULL)
    {
        char line[200];
        while (fgets(line, sizeof(line), fp))
        {
            if (sscanf(line, "%d,", &temp.problemID) != 1)
                continue;
            if (p.problemID == temp.problemID)
            {
                duplicate = 1;
                break;
            }
        }

        fclose(fp);
    }

    if (duplicate == 1)
    {
        printf("Problem ID already exists!\n");
        return;
    }

    printf("Enter Problem Description: ");
    readLine(p.description, sizeof(p.description));

    strcpy(p.status, "INCOMPLETE");

    fp = fopen("Text_Files/facilityissues.txt", "a");

    if (fp == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    fprintf(fp, "%d, %s, %s\n",
            p.problemID, p.description, p.status);

    fclose(fp);

    printf("\nFacility Problem Added Successfully!\n");
}

void viewFacilityProblems()
{
    FILE *fp;

    struct FacilityProblem p;

    fp = fopen("Text_Files/facilityissues.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    printf("\n=============== FACILITY PROBLEMS ===============\n");

    char line[200];
    while (fgets(line, sizeof(line), fp))
    {
        if (sscanf(line, "%d, %149[^,], %19[^\n]",
                   &p.problemID, p.description, p.status) == 3)
        {
            printf("\nProblem ID: %d\n", p.problemID);
            printf("Description: %s\n", p.description);
            printf("Status: %s\n", p.status);

            printf("------------------------------------\n");
        }
    }

    fclose(fp);
}


// VIEW INSTRUCTOR RATINGS 

void viewInstructorRating()
{
    FILE *fp;
    struct Rating r;
    int count = 0;
    float total = 0;

    fp = fopen("Text_Files/ratings.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    printf("\n=============== INSTRUCTOR RATINGS ===============\n");

    while (fscanf(fp,
        "%49[^|]|%d|%f|%149[^\n]\n",
        r.studentName,
        &r.ratingID,
        &r.ratingScore,
        r.feedback) == 4)
    {
        printf("\nStudent: %s\n", r.studentName);
        printf("Class ID: %d\n", r.ratingID);
        printf("Rating: %.1f / 5.0\n", r.ratingScore);
        printf("Feedback: %s\n", r.feedback);

        total = total + r.ratingScore;
        count++;
    }

    if (count > 0)
    {
        printf("\nTotal Ratings: %d\n", count);
        printf("Overall Average Rating: %.2f / 5.0\n",
               total / count);
    }
    else
    {
        printf("\nNo ratings available.\n");
    }

    fclose(fp);
}


int mains(){
    instructorMenu();
    return 0;
}
