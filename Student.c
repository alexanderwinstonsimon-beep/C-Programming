#include <stdio.h>
#include <string.h>

int bookClass();   // Function prototype declaration
int payment();     // Function prototype declaration
int rating();      // Function prototype declaration

int main(){
    int choice;
    
    printf("\nStudent Management System");
    printf("\n---------------------------------");
    printf("\n1. Book Class Management\n");
    printf("2. Payment Management\n");
    printf("3. Instructor Rating\n");
    printf("4. Exit\n");

    do {
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
    } while (choice < 1 || choice > 4);

    return 0;
}

int bookClass(){
    int choice;
    
    printf("\nBook Class Management");
    printf("\n---------------------------------");
    printf("\n1. Book\n");
    printf("2. View\n");
    printf("3. Reschedule\n");
    printf("4. Cancel\n");
    printf("5. Back\n");

    do {
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
                main(); // Call main function to return to the main menu
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice < 1 || choice > 5);

    return 0;
}

int payment(){
    // Payment management logic goes here
    printf("\nPayment management is under construction.\n");
    return 0;
}

int rating(){
    // Instructor rating logic goes here
    printf("\nInstructor rating is under construction.\n");
    return 0;
}