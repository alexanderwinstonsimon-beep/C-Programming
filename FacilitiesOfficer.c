#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Facilities Officer.h"
#include "main.h"

struct equipmentlist{
    int equipmentid;
    char equipmentname[50];
    int equipmentstock;
};

struct facilityissues{
    int issueid;
    char issuedesc[150];
    char status[20];
};

struct reports{
    int id;
    int equipmenttype;
    int totalstock;
};

// inventory management

void addequipment(){
    FILE *fptr = fopen("equipment.txt", "a");
    if (fptr == NULL) {
        printf("Error: Cannot open file!\n");
        return;
    }

    struct equipmentlist item;
    printf("Enter equipment ID: ");
    scanf("%d", &item.equipmentid);
    getchar();

    printf("Enter equipment name: ");
    scanf("%c", item.equipmentname);
    fgets(item.equipmentname, sizeof(item.equipmentname), stdin);
    item.equipmentname[strcspn(item.equipmentname, "\n")] = '\0';

    printf("Enter quantity of item in stock: ");
    scanf("%d", &item.equipmentstock);
    fprintf(fptr, "%d,%s,%d\n", item.equipmentid, item.equipmentname, item.equipmentstock);
    fclose(fptr);
    printf("Equipment added successfully!\n");
    return;
}

void viewequipment(){
    FILE *fptr = fopen("equipment.txt", "r");
    if (fptr == NULL) {
        printf("Error: File is empty!\n");
        return;
    }

    struct equipmentlist item;
    char line[100];
    printf("\n%-10s %-25s %-10s\n", "ID", "Name", "Quantity");
    printf("-----------------------------------------------\n");

    while (fgets(line, sizeof(line), fptr)) {
        if (sscanf(line, "%d,%49[^,],%d", &item.equipmentid, item.equipmentname, &item.equipmentstock) == 3) {
            printf("%-10d %-25s %-10d\n", item.equipmentid, item.equipmentname, item.equipmentstock);
        }
    }
    fclose(fptr);
    return;
}

void searchequipment(){
    FILE *fptr = fopen("equipment.txt", "r");
    if (fptr == NULL) {
        printf("Error: Cannot open file!\n");
        return;
    }

    int targetid, found = 0;
    struct equipmentlist item;
    char line[100];

    printf("Enter equipment ID to search: ");
    scanf("%d", &targetid);

    while (fgets(line, sizeof(line), fptr)){
        if (sscanf(line, "%d,%49[^,],%d", &item.equipmentid, item.equipmentname, &item.equipmentstock) == 3) {
            if (item.equipmentid == targetid){
                printf("\nEquipment found -> ID: %d | Name: %s | Quantity: %d\n", item.equipmentid, item.equipmentname, item.equipmentstock);
                found = 1;
                break;
            }
        }
    }
    if (!found) {
        printf("Equipment with ID %d not found.\n", targetid);
    }
    fclose(fptr);
    return
}

void updateequipment() {
    int updatechoice;
    printf("\n1. Update equipment ID\n");
    printf("2. Update equipment name\n");
    printf("3. Update equipment stock\n");
    printf("4. Return\n");
    printf("Enter choice: ");
    scanf("%d", &updatechoice);

    if (updatechoice == 4) return;

    FILE *file = fopen("equipment.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (file == NULL || temp == NULL) {
        printf("Error: Cannot open file!\n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    int targetid, found = 0;
    struct equipmentlist item;
    char line[150];

    printf("Enter equipment ID to update: ");
    scanf("%d", &targetid);
    getchar();

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,%49[^,],%d", &item.equipmentid, item.equipmentname, &item.equipmentstock) == 3) {
            if (item.equipmentid == targetid) {
                found = 1;
                printf("\nEquipment found -> ID: %d | Name: %s | Quantity: %d\n", item.equipmentid, item.equipmentname, item.equipmentstock);
                switch (updatechoice) {
                    case 1:
                        printf("Enter updated equipment ID: ");
                        scanf("%d", &item.equipmentid);
                        getchar();
                    case 2:
                        printf("Enter updated equipment name: ");
                        fgets(item.equipmentname, sizeof(item.equipmentname), stdin);
                        item.equipmentname[strcspn(item.equipmentname, "\n")] = '\0';
                    case 3:
                        printf("Enter updated equipment stock: ");
                        scanf("%d", &item.equipmentstock);
                        getchar();
                    case 4:
                        break;
                    default:
                        printf("Invalid choice!\n");
                }
            }
            fprintf(temp, "%d,%s,%d\n", item.equipmentid, item.equipmentname, item.equipmentstock);
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove("equipment.txt");
        rename("temp.txt", "equipment.txt");
        printf("Record updated successfully.\n");
    } else {
        remove("temp.txt");
        printf("Equipment ID %d not found.\n", targetid);
    }
    return;
}

// problem management

void viewissues() {
    FILE *fptr = fopen("facilityissues.txt", "r");
    if (fptr == NULL) {
        printf("Error: Cannot open facilityissues.txt!\n");
        return;
    }

    struct facilityissues issue;
    char line[200];
    printf("\n%-10s %-35s %-15s\n", "ID", "Description", "Status");
    printf("------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), fptr)) {
        if (sscanf(line, "%d,%149[^,],%19[^\n]", &issue.issueid, issue.issuedesc, issue.status) == 3) {
            printf("%-10d %-35s %-15s\n", issue.issueid, issue.issuedesc, issue.status);
        }
    }
    fclose(fptr);
    return;
}

void searchissues() {
    FILE *fptr = fopen("facilityissues.txt", "r");
    if (fptr == NULL) {
        printf("Error: Cannot open facilityissues.txt!\n");
        return;
    }

    int targetid, found = 0;
    struct facilityissues issue;
    char line[200];

    printf("Enter facility issue ID to search: ");
    scanf("%d", &targetid);

    while (fgets(line, sizeof(line), fptr)) {
        if (sscanf(line, "%d,%149[^,],%19[^\n]", &issue.issueid, issue.issuedesc, issue.status) == 3) {
            if (issue.issueid == targetid) {
                printf("\nIssue found -> ID: %d | Description: %s | Status: %s\n", issue.issueid, issue.issuedesc, issue.status);
                found = 1;
                break;
            }
        }
    }
    if (!found) {
        printf("Issue with ID %d not found.\n", targetid);
    }
    fclose(fptr);
    return;
}

void updateissues() {
    FILE *file = fopen("facilityissues.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (file == NULL || temp == NULL) {
        printf("Error: Cannot open facilityissues.txt!\n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    int targetid, found = 0;
    struct facilityissues issue;
    char line[200];
    char statusyn;

    printf("Enter ID of facility issue to update: ");
    scanf("%d", &targetid);
    getchar();

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,%149[^,],%19[^\n]", &issue.issueid, issue.issuedesc, issue.status) == 3) {
            if (issue.issueid == targetid) {
                found = 1;
                printf("\nIssue found -> ID: %d | Description: %s | Status: %s\n", issue.issueid, issue.issuedesc, issue.status);

                if (strcmp(issue.status, "INCOMPLETE") == 0) {
                    printf("Would you like to update this issue to RESOLVED? (Y/N): ");
                    scanf(" %c", &statusyn);
                    if (statusyn == 'Y' || statusyn == 'y') {
                        strcpy(issue.status, "RESOLVED");
                        printf("Status updated to RESOLVED.\n");
                    } else if (statusyn == 'N' || statusyn == 'n') {
                        printf("No changes made.\n");
                    } else {
                        printf("Invalid input.\n");
                    }
                } else if (strcmp(issue.status, "RESOLVED") == 0) {
                    printf("Would you like to revert this issue to INCOMPLETE? (Y/N): ");
                    scanf(" %c", &statusyn);
                    if (statusyn == 'Y' || statusyn == 'y') {
                        strcpy(issue.status, "INCOMPLETE");
                        printf("Status updated to INCOMPLETE.\n");
                    } else if (statusyn == 'N' || statusyn == 'n') {
                        printf("No changes made.\n");
                    } else {
                        printf("Invalid input.\n");
                    }
                } else {
                    printf("Unknown status in file.\n");
                }
            }
            fprintf(temp, "%d,%s,%s\n", issue.issueid, issue.issuedesc, issue.status);
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove("facilityissues.txt");
        rename("temp.txt", "facilityissues.txt");
    } else {
        remove("temp.txt");
        printf("Facility issue ID %d not found.\n", targetid);
    }
    return;
}

// stock report

void viewsummary() {
    FILE *fptr;
    fprt = fopen("equipment.txt", "r");
    if (fptr == NULL) {
        printf("Error: Cannot open equipment.txt!\n");
        return;
    }

    struct equipmentlist item;
    char line[150];
    int totaltypes = 0;
    int totalstock = 0;

    printf("\n====================================================\n");
    printf("          EQUIPMENT INVENTORY STOCK REPORT          \n");
    printf("====================================================\n");
    printf("%-10s | %-28s | %-8s\n", "ID", "Equipment Name", "Stock");
    printf("----------------------------------------------------\n");

    while (fgets(line, sizeof(line), fptr)) {
        if (sscanf(line, "%d,%49[^,\n],%d", &item.equipmentid, item.equipmentname, &item.equipmentstock) == 3) {
            printf("%-10d | %-28s | %-8d\n", item.equipmentid, item.equipmentname, item.equipmentstock);
            totaltypes++;
            totalstock += item.equipmentstock;
        }
    }

    printf("----------------------------------------------------\n");
    printf("Summary: %d Equipment Types | Total Units in Stock: %d\n", totaltypes, totalstock);
    printf("====================================================\n\n");
    fclose(fptr);

    struct reports summary;
    FILE *report;
    report = fopen("reports.txt", "w");
    if(report==NULL){
        printf("Error: Cannot open file!");
        return;
    } else{
        fprintf(report, "%d|%d|%d\n", summary.id, summary.equipmenttype, summary.totalstock);
    }
    fclose(report);
    return;
}

void searchreports(){
    FILE *fptr;
    fptr=fopen("reports.txt", "r");
    if(fptr == NULL){
        printf("Error: Cannot open file!");
        return;
    } else{
        int targetid, found = 0;
        struct reports summary;
        char line[150];

        printf("Enter report ID to search: ");
        scanf("%d", &targetid);

        while(fgets(line, sizeof(line), fptr)){
            if(sscanf(line, "%d,%d,%d", &summary.id, &summary.equipmenttype, &summary.totalstock) == 3){
                if(report.id == targetid){
                    printf("\nReport found -> ID: %d | Equipment Types: %d | Equipment Stock: %d\n", &report.id, &report.equipmenttype, &report.totalstock);
                    found = 1;
                    break;
                }
            }
        }
        if(!found){
            printf("Report with ID %d not found.\n", targetid);
        }
    }
    fclose(fptr);
    return;
}

//facility officer menu options
void inventorymanagement(){
    int imchoice;
    printf("1. Add Equipment\n");
    printf("2. View Equipment\n");
    printf("3. Search Equipment\n");
    printf("4. Update Equipment\n");
    printf("5. Exit\n");
    printf("Enter choice: ");
    scanf("%d", &imchoice);

    switch(imchoice){
        case 1:
            addequipment();
        case 2:
            viewequipment();
        case 3:
            searchequipment();
        case 4:
            updateequipment();
        case 5:
            break;
        default:
            printf("Invalid option! Please try again!");
    }
    return;
}

void problemmanagement(){
    int pmchoice;
    printf("1. View Facility Issues\n");
    printf("2. Search Facility Issues\n");
    printf("3. Update Facility Issues\n");
    printf("4. Exit\n");
    printf("Enter choice: ");
    scanf("%d", &pmchoice);

    switch(pmchoice){
        case 1:
            viewissues();
        case 2:
            searchissues();
        case 3:
            updateissues();
        case 4:
            break;
        default:
            printf("Invalid option! Please try again!");
    }
    return;
}

void inventorystockreports(){
    int isrchoice;
    printf("1. View Inventory Stock Summary\n");
    printf("2. Search Inventory Stock Reports\n");
    printf("3. Exit\n");
    printf("Enter choice: ");
    scanf("%d", &isrchoice);
    
    switch(isrchoice){
        case 1:
            viewsummary();
        case 2:
            searchreports();
        case 3:
            break;
        default:
            printf("Invalid option! Please try again!");
    }
    return;
}

//actual menu
int facilitiesofficermenu(){
    int fochoice;
    printf("1. Equipment Inventory Management\n");
    printf("2. Facility Problem Management\n");
    printf("3. Equipment Inventory Stock Reports\n");
    printf("4. Logout\n");
    printf("Enter choice: ");
    scanf("%d", &fochoice);

    switch(fochoice){
        case 1:
            inventorymanagement();
        case 2:
            problemmanagement();
        case 3:
            inventorystockreports();
        case 4:
            break;
        default:
            printf("Invalid option! Please try again!");
    }
    return 0;
}
