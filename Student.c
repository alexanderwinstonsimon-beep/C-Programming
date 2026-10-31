#include <stdio.h>
#include <string.h>

int bookClass();
int payment();
int rating();

int main(){
    int choice;

    do {
        printf("\nStudent Management System");
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
            case 1: 
                break;
            case 2: 
                break;
            case 3: 
                break;
            case 4: 
                break;
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
            case 1: 
                break;
            case 2: 
                break;
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
            case 1: 
                break;
            case 2: 
                break;
            case 3: 
                printf("\nReturning to main menu.\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice != 3);
    return 0;
}