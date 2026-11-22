#include <stdio.h>
#include <string.h>
#include "Facilities Officer.h"
#include "main.h"

struct equipmentlist{
    int equipmentid;
    char equipmentname[50];
    int equipmentstock;
}

struct facilityissues{
    int issueid;
    char issuedesc[150];
    char status;
}

struct reports{
    int id;
    int equipmenttype;
    int equipmentstock;
}

//inventory management
void addequipment(){
    FILE *fptr;
    fptr=fopen("equipment.txt","a");
    if(fptr==NULL){
        printf("Error: Cannot open file!");
        return;
    } else{
        struct equipmentlist item;
        printf("Enter equipment ID: ");
        scanf("%d", &item.equipmentid);
        getchar();

        printf("Enter equipment name: ");
        fgets(item.equipmentname, sizeof(item.equipmentname), stdin);
        item.equipmentname[strcspn(item.equipmentname, "\n")] = '\0';

        printf("Enter quantity of item in stock: ");
        scanf("%d", &item.equipmentstock);

        fprintf(file, "%d|%s|%d\n", item.equipmentid, item.equipmentname, item.equipmentstock);
    }
    fclose(file)
}

void viewequipment(){
    FILE *fptr;
    fptr=fopen("equipment.txt", "r");
    if(fptr==NULL){
        printf("Error: Cannot open file!");
        return;
    } else{
        struct equipmentlist item;
        char line[100];

        printf("\n%-10s %-25s %-10s\n", "ID", "Name", "Quantity");
        printf("-----------------------------------------------\n");

        while(fgets(line, sizeof(line), file)){
            if(sscanf(line, "%d,%49[^|],%d", &item.equipmentid, item.equipmentname, &item.equipmentstock)==3){
                printf("%-10d %-25s %-10d\n", item.equipmentid, item.equipmentname, item.equipmentstock);
            }
        }
    }
    fclose(file);
}

void searchequipment(){
    FILE *fptr;
    fptr=fopen("equipment.txt", "r");
    if(file == NULL){
        printf("Error: Cannot open file!");
        return;
    } else{
        int targetid, found = 0;
        struct equipmentlist item;
        char line[100];

        printf("Enter equipment ID to search: ");
        scanf("%d", &targetid);

        while(fgets(line, sizeof(line), file)){
            if(sscanf(line, "%d,%49[^|],%d", &item.equipmentid, item.equipmentname, &item.equipmentstock) == 3){
                if(item.equipmentid == targetid){
                    printf("\nEquipment found -> ID: %d | Name: %s | Quantity: %d\n", item.equipmentid, item.equipmentname, item.equipmentstock);
                    found = 1;
                    break;
                }
            }
        }
        if(!found){
            printf("Equipment with ID %d not found.\n", targetid);
        }
    }
    fclose(file);
}

void updateequipment(){
    int updatechoice;
    printf("1. Update equipment ID\n");
    printf("2. Update equipment name\n");
    printf("3. Update equipment stock\n");
    printf("4. Return\n");
    printf("Enter choice: ");
    scanf("%d", &updatechoice);

    FILE *file = fopen("equipment.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if(file == NULL || temp == NULL){
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

    while(fgets(line, sizeof(line), file)){
        if(sscanf(line, "%d,%49[^|],%d", &item.equipmentid, item.equipmentname, &item.equipmentstock) == 3){
            if(item.equipmentid == targetid){
                found = 1;
                printf("\nEquipment found -> ID: %d | Name: %s | Quantity: %d\n", item.equipmentid, item.equipmentname, item.equipmentstock);

                switch (updatechoice){
                    case 1:
                        printf("Enter updated equipment ID: ");
                        scanf("%d", &item.equipmentid);
                        getchar();
                        break;
                    case 2:
                        printf("Enter updated equipment name: ");
                        fgets(item.equipmentname, sizeof(item.equipmentname), stdin);
                        item.equipmentname[strcspn(item.equipmentname, "\n")] = 0; 
                        break;
                    case 3:
                        printf("Enter updated equipment stock: ");
                        scanf("%d", &item.equipmentstock);
                        getchar();
                        break;
                    case 4:
                        break;
                    default:
                        printf("Invalid choie! Please try again!");
                }
            }
            fprintf(temp, "%d,%s,%d\n", item.equipmentid, item.equipmentname, item.equipmentstock);
        }
    }

    fclose(file);
    fclose(temp);

    if (found){
        remove("equipment.txt");
        rename("temp.txt", "equipment.txt"); 
    } else{
        remove("temp.txt");
        printf("Equipment ID %d not found.\n", targetid);
    }
}

//facility problem management
void viewissues(){
    FILE *fptr;
    fptr=fopen("facilityissues.txt", "r");
    if(fptr==NULL){
        printf("Error: Cannot open file!");
        return;
    } else{
        struct facilityissues issue;
        char line[150];

        printf("\n%-10s %-25s %-10s\n", "ID", "Description", "Status");
        printf("-----------------------------------------------\n");

        while(fgets(line, sizeof(line), file)){
            if(sscanf(line, "%d,%49[^|],%49[^|]", &issue.issueid, issue.issuedesc, &issue.status)==3){
                printf("%-10d %-25s %-10s\n", issue.issueid, issue.issuedesc, issue.status);
            }
        }
    }
    fclose(file);
}

void searchissues(){
    FILE *fptr;
    fptr=fopen("facilityissues.txt", "r");
    if(file == NULL){
        printf("Error: Cannot open file!");
        return;
    } else{
        int targetid, found = 0;
        struct facilityissues issue;
        char line[150];

        printf("Enter equipment ID to search: ");
        scanf("%d", &targetid);

        while(fgets(line, sizeof(line), file)){
            if(sscanf(line, "%d,%49[^|],%s", &issue.issueid, issue.issuedesc, issue.status) == 3){
                if(item.equipmentid == targetid){
                    printf("\nIssue found -> ID: %d | Description: %s | Status: %d\n", issue.issueid, issue.issuedesc, issue.status);
                    found = 1;
                    break;
                }
            }
        }
        if(!found){
            printf("Equipment with ID %d not found.\n", targetid);
        }
    }
    fclose(file);
}

void updateissues(){
    FILE *file = fopen("facilityissues.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if(file == NULL || temp == NULL){
        printf("Error: Cannot open file!\n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    int targetid, found = 0;
    struct facilityissues issue;
    char line[150];
    char statusyn;

    printf("Enter ID of facility issue to update: ");
    scanf("%d", &targetid);
    getchar();

    while(fgets(line, sizeof(line), file)){
        if(sscanf(line, "%d,%49[^|],%s", &issue.issueid, issue.issuedesc, issue.status) == 3){
            if(issue.issueid == targetid){
                found = 1;
                printf("\nIssue found -> ID: %d | Description: %s | Status: %d\n", issue.issueid, issue.issuedesc, issue.status);
                if(issue.status == "INCOMPLETE"){
                    printf("Would you like to update this issue to resolved? (Y/N): ");
                    scanf("%c", statusyn);
                    if(statusyn == "Y" || "y"){
                        issue.status == "RESOLVED";
                        printf("Status updated to resolved.");
                    } else if(statusyn == "N" || "n"){
                        printf("No changes made.")
                    } else{
                        printf("Invalid input.")
                    }
                } else if(issue.status == "RESOLVED"){
                    printf("Would you like to revert this issue to incomplete? (Y/N): ");
                    scanf("%c", statusyn);
                    if(statusyn == "Y" || "y"){
                        issue.status == "INCOMPLETE";
                        printf("Status updated to incomplete.");
                    } else if(statusyn == "N" || "n"){
                        printf("No changes made.")
                    } else{
                        printf("Invalid input.")
                    }
                } else{
                    printf("Error.");
                }
            }
            fprintf(temp, "%d,%s,%d\n", issue.issueid, issue.issuedesc, issue.status);
        }
    }

    fclose(file);
    fclose(temp);

    if (found){
        remove("facilityissues.txt");
        rename("temp.txt", "facilityissues.txt"); 
    } else{
        remove("temp.txt");
        printf("Facility issue ID %d not found.\n", targetid);
    }
}

//inventory stock reports
void viewsummary(){
    int equipmentquantities[100];
    FILE *fptr;
    fptr=fopen("equipment.txt", "r");
    FILE *report = fopen("reports.txt");
    if(fptr==NULL){
        printf("Error: Cannot open file!");
        return;
    } else{
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
                totalTypes++;
                totalStock += item.equipmentstock;
            }
        }

    printf("----------------------------------------------------\n");
    printf("Summary: %d Equipment Types | Total Units in Stock: %d\n", totalTypes, totalStock);
    printf("====================================================\n\n");
    fclose(fptr);

    fopen("reports.txt","w");
        if(report==NULL){
            printf("Error: Cannot open file!");
            return;
        } else{
            struct reports report;
            fprintf(file, "%d|%d|%d\n", report.id, report.equipmenttype, report.equipmentstock);
        }
    fclose(report);
}

void searchreports(){
    FILE *fptr;
    fptr=fopen("reports.txt", "r");
    if(file == NULL){
        printf("Error: Cannot open file!");
        return;
    } else{
        int targetid, found = 0;
        struct reports report;
        char line[150];

        printf("Enter report ID to search: ");
        scanf("%d", &targetid);

        while(fgets(line, sizeof(line), file)){
            if(sscanf(line, "%d,%d,%d", &report.id, &report.equipmenttype, &report.equipmentstock) == 3){
                if(report.id == targetid){
                    printf("\nReport found -> ID: %d | Equipment Types: %d | Equipment Stock: %d\n", &report.id, &report.equipmenttype, &report.equipmentstock);
                    found = 1;
                    break;
                }
            }
        }
        if(!found){
            printf("Report with ID %d not found.\n", targetid);
        }
    }
    fclose(file);    
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
            updatequipment();
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
void facilitiesofficermenu(){
    int fochoice;
    printf("1. Equipment Inventory Management\n");
    printf("2. Facility Problem Management\n");
    printf("3. Equipment Inventory Stock Reports\n");
    printf("4. Logout\n");
    printf("Enter choice: ");
    scanf("%d", &fochoice);

    switch(choice){
        case 1:
            inventorymanagement();
        case 2:
            problemmanagement();
        case 3:
            inventorystockreport();
        case 4;
            break;
        default:
            printf("Invalid option! Please try again!");
    }
    return 0;
}