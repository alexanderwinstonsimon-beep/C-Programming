#include <stdio.h>
#include <string.h>

int bookClass(){
    int choice;
    
    printf("\n1. Book\n");
    printf("2. View\n");
    printf("3. Reschedule\n");
    printf("4. Cancel\n");
    printf("5. Exit\n");

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
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 1, 2, 3, 4, 5);

    return 0;
}

int main(){
    int choice;
    
    printf("\n1. Book Class Management\n");
    printf("2. Payment Management\n");
    printf("3. Instructor Rating\n");
    printf("4. Exit\n");

    do {
        printf("\nEnter choice: ");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                bookClass()
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 1, 2, 3, 4);

    return 0;
}