#include<stdio.h>
#include<string.h>
#include "Instructor.h"
#include "main.h"
struct Class
{
    int classID;
    char martialArt[30];
    char instructorName[50];
    char date[20];
    char time[20];
    int availableSlots;
};

struct FacilityProblem
{
    int problemID;
    char facility[50];
    char description[150];
    char date[20];
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

void readLine(char text[], int size)
{
    fgets(text, size, stdin);
    text[strcspn(text, "\n")] = '\0';
}
void instructorMenu()
{
    int choice;

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

    printf("Enter Martial Art: ");
    scanf(" %29[^\n]", c.martialArt);

    printf("Enter Instructor Name: ");
    scanf(" %49[^\n]", c.instructorName);

    printf("Enter Date: ");
    scanf(" %19[^\n]", c.date);

    printf("Enter Time: ");
    scanf(" %19[^\n]", c.time);

    printf("Enter Available Slots: ");
    scanf("%d", &c.availableSlots);

    // Check duplicate Class ID
    fp = fopen("class_schedule.txt", "r");

    if (fp != NULL)
    {
        while (fscanf(fp,
            "%d|%29[^|]|%49[^|]|%19[^|]|%19[^|]|%d\n",
            &temp.classID,
            temp.martialArt,
            temp.instructorName,
            temp.date,
            temp.time,
            &temp.availableSlots) == 6)
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
    fp = fopen("class_schedule.txt", "a");

    if (fp == NULL)
    {
        printf("\nUnable to open file!\n");
        return;
    }

    fprintf(fp, "%d|%s|%s|%s|%s|%d\n",
        c.classID,
        c.martialArt,
        c.instructorName,
        c.date,
        c.time,
        c.availableSlots);

    fclose(fp);

    printf("\nClass Schedule Added Successfully!\n");
}

void viewClassSchedule()
{
    FILE *fp;
    struct Class c;

    fp = fopen("class_schedule.txt", "r");

    if (fp == NULL)
    {
        printf("\nNo class schedules found.\n");
        return;
    }

    printf("\n========== CLASS SCHEDULE ==========\n");
    printf("%-10s %-15s %-30s %-20s %-10s %-15s\n", "Class ID", "Martial Art", "Instructor Name", "Date", "Time", "Available Slots");
    printf("---------------------------------------------------------------------------------------------------------------\n");

    while (fscanf(fp,
        "%d|%29[^|]|%49[^|]|%19[^|]|%19[^|]|%d\n",
        &c.classID,
        c.martialArt,
        c.instructorName,
        c.date,
        c.time,
        &c.availableSlots) == 6)
    {
        printf("%-10s %-15s %-30s %-20s %-10s %-15s\n",
            c.classID, c.martialArt, c.instructorName, c.date, c.time, c.availableSlots);
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

    fp = fopen("class_schedule.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    while (fscanf(fp,
        "%d|%29[^|]|%49[^|]|%19[^|]|%19[^|]|%d\n",
        &c.classID,
        c.martialArt,
        c.instructorName,
        c.date,
        c.time,
        &c.availableSlots) == 6)
    {
        if (searchID == c.classID)
        {
            printf("\nClass Found!\n");

            printf("Class ID: %d\n", c.classID);
            printf("Martial Art: %s\n", c.martialArt);
            printf("Instructor: %s\n", c.instructorName);
            printf("Date: %s\n", c.date);
            printf("Time: %s\n", c.time);
            printf("Available Slots: %d\n", c.availableSlots);

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

    fp = fopen("class_schedule.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    tempFile = fopen("class_temp.txt", "w");

    if (tempFile == NULL)
    {
        printf("Unable to create temporary file!\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp,
        "%d|%29[^|]|%49[^|]|%19[^|]|%19[^|]|%d\n",
        &c.classID,
        c.martialArt,
        c.instructorName,
        c.date,
        c.time,
        &c.availableSlots) == 6)
    {
        if (searchID == c.classID)
        {
            found = 1;

            printf("\nClass Found! Enter New Information.\n");

            printf("Enter New Martial Art: ");
            readLine(c.martialArt, sizeof(c.martialArt));

            printf("Enter New Instructor Name: ");
            readLine(c.instructorName, sizeof(c.instructorName));

            printf("Enter New Date: ");
            readLine(c.date, sizeof(c.date));

            printf("Enter New Time: ");
            readLine(c.time, sizeof(c.time));

            printf("Enter New Available Slots: ");
            scanf("%d", &c.availableSlots);     

            if (c.availableSlots < 0)
            {
                printf("Invalid slots. Keeping original record.\n");
                fclose(fp);
                fclose(tempFile);
                remove("class_temp.txt");
                return;
            }
        }

        fprintf(tempFile, "%d|%s|%s|%s|%s|%d\n",
                c.classID,
                c.martialArt,
                c.instructorName,
                c.date,
                c.time,
                c.availableSlots);
    }

    fclose(fp);
    fclose(tempFile);

    if (found == 1)
    {
        remove("class_schedule.txt");
        rename("class_temp.txt", "class_schedule.txt");

        printf("\nClass Schedule Updated Successfully!\n");
    }
    else
    {
        remove("class_temp.txt");
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

    fp = fopen("facility_problem.txt", "r");

    if (fp != NULL)
    {
        while (fscanf(fp,
            "%d|%49[^|]|%149[^|]|%19[^|]|%19[^\n]\n",
            &temp.problemID,
            temp.facility,
            temp.description,
            temp.date,
            temp.status) == 5)
        {
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

    printf("Enter Facility Name: ");
    readLine(p.facility, sizeof(p.facility));

    printf("Enter Problem Description: ");
    readLine(p.description, sizeof(p.description));

    printf("Enter Date (DD/MM/YYYY): ");
    readLine(p.date, sizeof(p.date));

    strcpy(p.status, "Pending");

    fp = fopen("facility_problem.txt", "a");

    if (fp == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    fprintf(fp, "%d|%s|%s|%s|%s\n",
            p.problemID,
            p.facility,
            p.description,
            p.date,
            p.status);

    fclose(fp);

    printf("\nFacility Problem Added Successfully!\n");
}

void viewFacilityProblems()
{
    FILE *fp;

    struct FacilityProblem p;

    fp = fopen("facility_problem.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    printf("\n=============== FACILITY PROBLEMS ===============\n");

    while (fscanf(fp,
        "%d|%49[^|]|%149[^|]|%19[^|]|%19[^\n]\n",
        &p.problemID,
        p.facility,
        p.description,
        p.date,
        p.status) == 5)
    {
        printf("\nProblem ID: %d\n", p.problemID);
        printf("Facility: %s\n", p.facility);
        printf("Description: %s\n", p.description);
        printf("Date: %s\n", p.date);
        printf("Status: %s\n", p.status);

        printf("------------------------------------\n");
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

    fp = fopen("instructor_rating.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    printf("\n=============== INSTRUCTOR RATINGS ===============\n");

    while (fscanf(fp,
        "%d|%49[^|]|%49[^|]|%f|%149[^\n]\n",
        &r.ratingID,
        r.instructorName,
        r.studentName,
        &r.ratingScore,
        r.feedback) == 5)
    {
        printf("\nRating ID: %d\n", r.ratingID);
        printf("Instructor: %s\n", r.instructorName);
        printf("Student: %s\n", r.studentName);
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
