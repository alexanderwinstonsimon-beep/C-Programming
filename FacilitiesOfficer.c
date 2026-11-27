#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FacilitiesOfficer.h"
#include "main.h"

struct user{
    char username[30];
    char password[30];
};

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

// login system

void registeruser(){
    struct user newuser;
    FILE *fptr;
    fptr = fopen("facilityofficers.txt", "a");
    if(fptr==NULL){
        printf("Error: cannot find file!");
        return;
    }

    printf("\n--- Register New Account ---\n");
    printf("Enter username: ");
    scanf("%30s", newuser.username);
    
    printf("Enter password: ");
    scanf("%30s", newuser.password);

    fprintf(fptr, "%s, %s\n", newuser.username, newuser.password);
    printf("Registration successful!\n");
    fclose(fptr);

    return;
}

int loginuser(){
    struct user inputuser;
    struct user storeduser;
    char line[100];
    int loginsuccess = 0;

    printf("\n--- Login ---\n");
    printf("Username: ");
    scanf("%30s", inputuser.username);
    
    printf("Password: ");
    scanf("%30s", inputuser.password);

    FILE *fptr = fopen("facilityofficers.txt", "r");
    if(fptr==NULL){
        printf("No registered users found.\n");
    }

    while (fgets(line, sizeof(line), fptr)){
        if (sscanf(line, "%30[^,], %30s", storeduser.username, storeduser.password)==2){ 
            if(strcmp(inputuser.username, storeduser.username) == 0 && strcmp(inputuser.password, storeduser.password) == 0){
                loginsuccess = 1;
                break;
            }
        }
    }

    fclose(fptr);

    if(loginsuccess==1){
        printf("\nLogin successful! Welcome, %s.\n", inputuser.username);
        return 1;
    } else{
        printf("\nInvalid username or password.\n");
        return 0;
    }
}

// inventory management

void addequipment(){
    char continueyn = 'Y';
    FILE *fptr = fopen("equipment.txt", "a");
    if(fptr == NULL){
        printf("Error: Cannot open file!\n");
        return;
    }

    struct equipmentlist item;
    do{
        printf("Enter equipment ID: ");
        scanf("%d", &item.equipmentid);

        printf("Enter equipment name: ");
        scanf(" %50[^\n]", &item.equipmentname);

        printf("Enter quantity of item in stock: ");
        scanf("%d", &item.equipmentstock);
    
        printf("You have successfully added the following entry: \n");
        printf("Equipment ID: %d | Equipment Name: %s | Equipment Stock: %d\n", item.equipmentid, item.equipmentname, item.equipmentstock);
        printf("Equipment added successfully!\n");
        printf("Continue? (Y/N): ");
        scanf(" %c", &continueyn);
        fprintf(fptr, "%d, %s, %d \n", item.equipmentid, item.equipmentname, item.equipmentstock);
    } while(continueyn == 'Y' || continueyn == 'y');

    fclose(fptr);
    return;
}

void viewequipment(){
    char veexityn;
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
        if (sscanf(line, "%d,%49[^,],%d", &item.equipmentid, item.equipmentname, &item.equipmentstock) == 3){
            printf("%-10d %-25s %-10d\n", item.equipmentid, item.equipmentname, item.equipmentstock);
        }
    }
    printf("Exit? (Y): ");
    scanf(" %c", &veexityn);

    fclose(fptr);
    return;
}

void searchequipment(){
    FILE *fptr = fopen("equipment.txt", "r");
    if (fptr == NULL) {
        printf("Error: Cannot open file!\n");
        return;
    }

    char secontinueyn = 'Y';

    do{
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
        printf("Continue? (Y/N): ");
        scanf(" %c", &secontinueyn);
    } while(secontinueyn=='Y' || secontinueyn=='y');

    fclose(fptr);
    return;
}

void updateequipment() {
    int updatechoice;
    char uecontinueyn;
    
    do{
        printf("\n1. Update equipment ID\n");
        printf("2. Update equipment name\n");
        printf("3. Update equipment stock\n");
        printf("4. Delete equipment entry\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &updatechoice);

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

        while(fgets(line, sizeof(line), file)){
            if(sscanf(line, "%d,%49[^,],%d", &item.equipmentid, item.equipmentname, &item.equipmentstock) == 3) {
                if(item.equipmentid == targetid) {
                    found = 1;
                    printf("\nEquipment found -> ID: %d | Name: %s | Quantity: %d\n", item.equipmentid, item.equipmentname, item.equipmentstock);
                    switch (updatechoice) {
                        case 1:
                            printf("Enter updated equipment ID: ");
                            scanf("%d", &item.equipmentid);
                            getchar();
                            break;
                        case 2:
                            printf("Enter updated equipment name: ");
                            fgets(item.equipmentname, sizeof(item.equipmentname), stdin);
                            item.equipmentname[strcspn(item.equipmentname, "\n")] = '\0';
                            break;
                        case 3:
                            printf("Enter updated equipment stock: ");
                            scanf("%d", &item.equipmentstock);
                            getchar();
                            break;
                        case 4:
                        
                        case 5:
                            break;
                        default:
                            printf("Invalid choice!\n");
                            break;
                    }
                }
                fprintf(temp, "%d,%s,%d\n", item.equipmentid, item.equipmentname, item.equipmentstock);
            }
        }

        fclose(file);
        fclose(temp);

        if(found){
            remove("equipment.txt");
            rename("temp.txt", "equipment.txt");
            printf("Record updated successfully.\n");
        }else{
            remove("temp.txt");
            printf("Equipment ID %d not found.\n", targetid);
        }
        printf("Continue? (Y/N): ");
        scanf(" %c", &uecontinueyn);
    } while(uecontinueyn=='Y'||uecontinueyn=='y');
    return;
}

// problem management

void viewissues() {
    FILE *fptr = fopen("facilityissues.txt", "r");
    if(fptr == NULL){
        printf("Error: Cannot open facilityissues.txt!\n");
        return;
    }

    struct facilityissues issue;
    char line[200];
    char vicontinueyn;

    printf("\n%-10s %-35s %-15s\n", "ID", "Description", "Status");
    printf("------------------------------------------------------------\n");

    while(fgets(line, sizeof(line), fptr)){
        if (sscanf(line, "%d,%150[^,],%20[^\n]", &issue.issueid, issue.issuedesc, issue.status) == 3) {
            printf("%-10d %-35s %-15s\n", issue.issueid, issue.issuedesc, issue.status);
        }
    }
    printf("Exit? (Y): ");
    scanf(" %c", &vicontinueyn);

    fclose(fptr);
    return;
}

void searchissues() {
    FILE *fptr = fopen("facilityissues.txt", "r");
    if (fptr == NULL) {
        printf("Error: Cannot open facilityissues.txt!\n");
        return;
    }

    char sicontinueyn = 'Y';
    do{
        int targetid, found = 0;
        struct facilityissues issue;
        char line[200];

        printf("Enter facility issue ID to search: ");
        scanf("%d", &targetid);

        while (fgets(line, sizeof(line), fptr)) {
            if (sscanf(line, "%d,%150[^,],%20[^\n]", &issue.issueid, issue.issuedesc, issue.status) == 3) {
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

        printf("Continue? (Y/N): ");
        scanf(" %c", &sicontinueyn);
    } while(sicontinueyn=='Y'||sicontinueyn=='y');
    fclose(fptr);
    return;
}

void updateissues(){
    char uicontinueyn;
    do{
        FILE *file = fopen("facilityissues.txt", "r");
        FILE *temp = fopen("temp.txt", "w");
        if (file == NULL || temp == NULL) {
            printf("Error: Cannot open file!\n");
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

        while(fgets(line, sizeof(line), file)){
            if(sscanf(line, "%d,%150[^,],%20[^\n]", &issue.issueid, issue.issuedesc, issue.status) == 3) {
                if(issue.issueid == targetid){
                    found = 1;
                    printf("\nIssue found -> ID: %d | Description: %s | Status: %s\n", issue.issueid, issue.issuedesc, issue.status);
                    printf("Enter updated status: ");
                    scanf("%s", issue.status);
                }
                fprintf(temp, "%d, %s, %s\n", issue.issueid, issue.issuedesc, issue.status);
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
        printf("Continue? (Y/N): ");
        scanf(" %c", &uicontinueyn);
    } while(uicontinueyn=='Y'||uicontinueyn=='y');

    return;
}

// stock report

void viewsummary(){
    FILE *fptr;
    fptr = fopen("equipment.txt", "r");
    if (fptr == NULL) {
        printf("Error: Cannot open equipment.txt!\n");
        return;
    }

    struct equipmentlist item;
    char line[150];
    int totaltypes = 0;
    int totalstock = 0;
    int summaryid;
    struct reports summary;
    char vscontinueyn;
    
    FILE *report;
    report = fopen("reports.txt", "a");
    if(report==NULL){
        printf("Error: Cannot open file!");
        return;
    }

    printf("\n====================================================\n");
    printf("          EQUIPMENT INVENTORY STOCK REPORT          \n");
    printf("====================================================\n");
    printf("%-10s | %-28s | %-8s\n", "ID", "Equipment Name", "Stock");
    printf("----------------------------------------------------\n");

    while(fgets(line, sizeof(line), fptr)){
        if(sscanf(line, "%d,%49[^,\n],%d", &item.equipmentid, item.equipmentname, &item.equipmentstock) == 3){
            printf("%-10d | %-28s | %-8d\n", item.equipmentid, item.equipmentname, item.equipmentstock);
            totaltypes++;
            totalstock += item.equipmentstock;
        }
    }

    printf("----------------------------------------------------\n");
    printf("Summary: %d Equipment Types | Total Units in Stock: %d\n", totaltypes, totalstock);
    printf("====================================================\n\n");
    printf("Enter ID of this summary: ");
    scanf("%d", &summaryid);

    printf("Exit? (Y): ");
    scanf(" %c", &vscontinueyn);

    fclose(fptr);
    fprintf(report, "%d, %d, %d\n", summaryid, totaltypes, totalstock);
    fclose(report);
    return;
}

void searchreports(){
    char srcontinueyn = 'Y';
        FILE *report;
        report=fopen("reports.txt", "r");
        if(report == NULL){
            printf("Error: Cannot open file!");
            return;
        } else{
            do{
                int targetid, found = 0;
                struct reports summary;
                char line[150];

                printf("Enter report ID to search: ");
                scanf("%d", &targetid);

                while(fgets(line, sizeof(line), report)){
                    if(sscanf(line, "%d, %d, %d", &summary.id, &summary.equipmenttype, &summary.totalstock) == 3){
                        if(summary.id == targetid){
                            printf("\nReport found -> ID: %d | Equipment Types: %d | Equipment Stock: %d\n", summary.id, summary.equipmenttype, summary.totalstock);
                            found = 1;
                            break;
                        }
                    }
                }
                if(!found){
                    printf("Report with ID %d not found.\n", targetid);
                }
            printf("Continue? (Y/N): ");
            scanf(" %c", &srcontinueyn);
            } while(srcontinueyn=='Y'||srcontinueyn=='y');
        }

    fclose(report);
    return;
}

//facility officer menu options
void inventorymanagement(){
    char imcontinueyn = 'Y';
    do{
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
                break;
            case 2:
                viewequipment();
                break;
            case 3:
                searchequipment();
                break;
            case 4:
                updateequipment();
                break;
            case 5:
                imcontinueyn = 'N';
                break;
            default:
                printf("Invalid option! Please try again!");
                break;
        }
    } while(imcontinueyn=='Y');
    return;
}

void problemmanagement(){
    int pmchoice;
    char pmcontinueyn = 'Y';

    do{
        printf("1. View Facility Issues\n");
        printf("2. Search Facility Issues\n");
        printf("3. Update Facility Issues\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &pmchoice);

        switch(pmchoice){
            case 1:
                viewissues();
                break;
            case 2:
                searchissues();
                break;
            case 3:
                updateissues();
                break;
            case 4:
                pmcontinueyn = 'N';
                break;
            default:
                printf("Invalid option! Please try again!");
                break;
        }
    } while(pmcontinueyn=='Y');
    return;
}

void inventorystockreports(){
    int isrchoice;
    char isrcontinueyn = 'Y';
    do{
        printf("1. View Inventory Stock Summary\n");
        printf("2. Search Inventory Stock Reports\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &isrchoice);
    
        switch(isrchoice){
            case 1:
                viewsummary();
                break;
            case 2:
                searchreports();
                break;
            case 3:
                isrcontinueyn = 'N';
                break;
            default:
                printf("Invalid option! Please try again!");
                break;
        }
    } while(isrcontinueyn=='Y');
    return;
}

//actual menu

int successfullogin(){
    int fochoice;
    char focontinueyn = 'Y';
    do{
        printf("1. Equipment Inventory Management\n");
        printf("2. Facility Problem Management\n");
        printf("3. Equipment Inventory Stock Reports\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &fochoice);

        switch(fochoice){
            case 1:
                inventorymanagement();
                break;
            case 2:
                problemmanagement();
                break;
            case 3:
                inventorystockreports();
                break;
            case 4:
                focontinueyn = 'N';
                break;
            default:
                printf("Invalid option! Please try again!");
                break;
        }
    } while(focontinueyn=='Y');
    return 0;
}

int facilitiesofficermenu(){
    int fochoice;
    int authenticated = 0;
    
    while(!authenticated){
        printf("\n1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf("Enter choice: ");

        if(scanf("%d", &fochoice)!=1){
            while(getchar()!='\n');
            printf("Invalid input! Please try again!");
            continue;
        }

        switch(fochoice){
            case 1:
                authenticated = loginuser();
                break;
            case 2:
                registeruser();
                break;
            case 3:
                break;
            default:
                printf("Invalid option! Please try again!");
        }
    }
    successfullogin();
}
