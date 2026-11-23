
int filemanagement(){
    FILE *fptr;
    char search[100],replace[100],names[20][100],line[100];
    int count=0;
    fptr=fopen("equipment.txt","r");
    if(fptr==NULL){
        printf("Error: Cannot open file!");
    } else{
        while(fgets(line,100,fptr)!=NULL){
            strcpy(names[count],line);
            count++;
        }
        fclose(fptr);
    }

    printf("Enter name to search:");
    scanf(" %100[^\n]",search);

    printf("Enter replacement name:");
    scanf(" %100[^/n]",replace);
    //to update the  names in the array
    for(int i=o;i<count;i++){
        if(strncmp(names[i], search, strlen(search)) == 0 && (names[i][strlen(search)] == '\n' || names[i][strlen(search)] == '/0'){
            strcpy(names[i], replace);
            strcat(names[i], "\n");
        }
    }
    //rewrite into file
    fptr = fopen("equipment.txt","w");
    if(fptr==NULL){
        printf("Error: Cannot write into file!");
    } else{
        for(int i=0;i<count;i++){
            fprintf(fptr,"%s",names[i]);
        }
        fclose(fptr);
        printf("Update completed!\n");
    }
}

struct filestruct{
    int equipmentid;
    char equipmentname[50];
    int equipmentstock;
}

void filearray(){
    char line[100];
    filestruct=[];
    FILE *fptr;
    fptr=fopen("equipment.txt","r");
    if(fptr=="NULL"){
        printf("Error: Cannot open file!");
    } else{
        while(fgets(line,100,file)!=NULL){
            strcpy(names[count],line);            
            printf("e");
        }
    }
}
