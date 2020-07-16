#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct car_list{
    char plateNumber[10];
    char vecType;
    char strVecType[10];
    char color[10];
    long int inTime;
    char timeIn[20];
    long int outTime;
    char timeOut[20];
    double hour;
    int pay;
    struct car_list *next;
}car;

void printMenu();
void getInput(char*);
char getLetter(char*);
void addVehicle();
void viewRecord();
void removeVehicle();
void viewSpecific();
void initPayment();
void receipt(car*);
int getPayment(car*);
void quit();
void insertHistory(car*);
void load();
void getVecType(char*,char);
void cardesign();
void header();
void headerViewAll();
void quitDesign();

car *head=NULL;

int main(){
    char choice[10],letter;
    load();
    cardesign();
    printf("\n");

    do{
        printMenu();
        getInput(choice);
        letter=getLetter(choice);

        switch(letter){
            case 'a': addVehicle(); break;
            case 'b': initPayment(); break;
            case 'c': removeVehicle(); break;
            case 'd': viewRecord(); break;
            case 'e': viewSpecific(); break;
            case 'f': quit(); quitDesign(); break;
            default: printf("Invalid Entry.\n");
        }
    } while(letter!='f');

    return 0;
}

void printMenu(){
    header();
    printf("What do you want to do?\n");
    printf("\ta) Add vehicle.\n");
    printf("\tb) Initiate payment.\n");
    printf("\tc) Remove vehicle.\n");
    printf("\td) View all cars inside.\n");
    printf("\te) View specific car record.\n");
    printf("\tf) Quit.\n");
    printf("\nEnter the letter of your choice: ");
}

void getInput(char input[]){
    fgets(input,10,stdin);
    input[strlen(input)-1]='\0';
}

char getLetter(char choice[]){
    char aLetter;

    if(!strcmp("a",choice))
        aLetter='a';
    else if(!strcmp("b",choice))
        aLetter='b';
    else if(!strcmp("c",choice))
        aLetter='c';
    else if(!strcmp("d",choice))
        aLetter='d';
    else if(!strcmp("e",choice))
        aLetter='e';
    else if(!strcmp("f",choice))
        aLetter='f';
    else
        aLetter='\0';

    return aLetter;
}

void addVehicle(){
    car *temp;
    char uInput[10];
    int i;
    time_t now;

    temp=(car*) malloc(sizeof(car));

    printf("Enter plate number: ");
    getInput(temp->plateNumber);
    for(i=0;i<strlen(temp->plateNumber);i++){
        if(temp->plateNumber[i]==','){ //to check if there is a comma because ',' will be used as seperator.
            printf("Invalid input!\n");
            free(temp);
            return;
        }
    }

    printf("Enter vehicle type: \n");
    printf("\ta) 2-wheeled\n");
    printf("\tb) 4-wheeled\n");
    printf("\tc) truck\n");
    printf("Enter the letter of your choice: ");
    getInput(uInput);
    temp->vecType=getLetter(uInput);

    if((temp->vecType!='a')&&(temp->vecType!='b')&&(temp->vecType!='c')){
        printf("Invalid input!\n");
        free(temp);
        return;
    }

    printf("Enter vehicle color: ");
    getInput(temp->color);
    for(i=0;i<strlen(temp->color);i++){
        if(temp->color[i]==','){ //to check if there is a comma because ',' will be used as seperator.
            printf("Invalid input!\n");
            free(temp);
            return;
        }
    }
    now=time(NULL);
    temp->inTime=now; //convert year in the format Y-m-d|H:M:S
    strftime(temp->timeIn, 20, "%Y-%m-%d|%H:%M:%S", localtime(&now)); //extracted from http://stackoverflow.com/questions/3053999/c-convert-time-t-to-string-with-format-yyyy-mm-dd-hhmmss

    temp->next=head;
    head=temp; //add node at the beginning
}

void viewRecord(){
    car *temp;
    char strVecType[10];

    headerViewAll();

    if(head==NULL){
         printf("No car to view.\n\n");
    }
    else{
        temp=head;
        printf("\n--------------------------------------------\n");
        while(temp!=NULL){
            printf("Plate #: %s\n", temp->plateNumber);
                getVecType(strVecType,temp->vecType);
            printf("Vehicle type: %s\n", strVecType);
            printf("Color: %s\n", temp->color);
            printf("Time in: %s\n", temp->timeIn);
            temp=temp->next;
            printf("--------------------------------------------\n");
        }

        printf("\n");
    }
}

void viewSpecific(){
    car *temp;
    char plate[10];
    char strVecType[10];

    if(head==NULL){
         printf("No car to view.\n\n");
    }
    else{
        temp=head;
        printf("Enter plate number: ");
        getInput(plate);
        while(temp!=NULL){
            if(!strcmp(plate,temp->plateNumber)){ //search the specific vehicle entered
                printf("\n--------------------------------------------\n");
                printf("Plate #: %s\n", temp->plateNumber);
                    getVecType(strVecType,temp->vecType);
                printf("Vehicle type: %s\n", strVecType);
                printf("Color: %s\n", temp->color);
                printf("Time in: %s", temp->timeIn);
                printf("\n--------------------------------------------\n");
                return;
            }
            temp=temp->next;
        }


        if(temp==NULL)
            printf("No car with the same plate number!\n");

        printf("\n");
    }
}

void removeVehicle(){
    char plate[10];
    car *prev,*temp;
    int c=0;
    time_t now;
    double diff;

    if(head==NULL){
         printf("No car to remove.\n\n");
    }
    else{
        printf("Enter plate number: ");
        getInput(plate);
        prev=NULL;
        temp=head;
        if(head->next==NULL){ //if there is one vehicle in the current list
            if(!strcmp(plate,temp->plateNumber)){
                now=time(NULL);
                temp->outTime=now; //set the time out of the vehicle
                strftime(temp->timeOut, 20, "%Y-%m-%d|%H:%M:%S", localtime(&now)); //extracted from http://stackoverflow.com/questions/3053999/c-convert-time-t-to-string-with-format-yyyy-mm-dd-hhmmss
                diff=temp->outTime-temp->inTime; //subtract time out and time in to get the difference in seconds
                temp->hour=diff/3600.0;//divide by 3600 to convert it to hours
                insertHistory(temp); //write the vehicle record to history.txt
                head=NULL;
                free(head); //after writing to file, delete the record
                return;
            }
        }
        else{
            while(temp!=NULL){ //traverse to every vehicle in the list
                if(!strcmp(plate,temp->plateNumber)){
                    if(prev==NULL){
                        head=temp->next;
                        now=time(NULL);
                        temp->outTime=now;
                        strftime(temp->timeOut, 20, "%Y-%m-%d|%H:%M:%S", localtime(&now)); //extracted from http://stackoverflow.com/questions/3053999/c-convert-time-t-to-string-with-format-yyyy-mm-dd-hhmmss
                        diff=temp->outTime-temp->inTime; //subtract time out and time in to get the difference in seconds
                        temp->hour=diff/3600.0; //divide by 3600 to convert it to hours
                        insertHistory(temp);
                        temp->next=NULL;
                        free(temp);
                        c++;//flag to know if the plate number is found
                        return;
                    }
                    else{
                        prev->next=temp->next;
                        now=time(NULL);
                        temp->outTime=now;
                        strftime(temp->timeOut, 20, "%Y-%m-%d|%H:%M:%S", localtime(&now)); //extracted from http://stackoverflow.com/questions/3053999/c-convert-time-t-to-string-with-format-yyyy-mm-dd-hhmmss
                        diff=temp->outTime-temp->inTime; //subtract time out and time in to get the difference in seconds
                        temp->hour=diff/3600.0; //divide by 3600 to convert it to hours
                        insertHistory(temp);
                        temp->next=NULL;
                        free(temp);
                        c++; //flag to know if the plate number is found
                        return;
                    }
                }
                prev=temp;
                temp=temp->next;
            }
        }

        if(c==0)
            printf("No car with the same plate number!\n");

        printf("\n");
    }
}

void quit(){
    FILE *fp;
    car *temp;
    char strVecType[10];

    fp=fopen("vehicle.txt","w"); //after selecting the quit option write all the current vehicle records to vehicle.txt
    temp=head;

    if(head==NULL)
        return;
    else{
        while(temp!=NULL){
            if(temp->vecType=='a') {
                strcpy(strVecType,"2-wheeled");
            }
            else if(temp->vecType=='b') {
                strcpy(strVecType,"4-wheeled");
            }
            else if(temp->vecType=='c') {
                strcpy(strVecType,"truck");
            }
            fprintf(fp,"%s,%s,%s,%s,%ld\n",temp->plateNumber,strVecType,temp->color,temp->timeIn,temp->inTime);
            temp=temp->next;
        }
    }
    fclose(fp);
}

void initPayment(){
    car *temp=head,*prev;
    time_t now;
    char plate[10];
    double diff;
    int c=0;

    if(head==NULL){
         printf("No car to view.\n\n");
    }
    else{
        printf("Enter plate number: ");
        getInput(plate);
        prev=NULL;
        temp=head;
        if(head->next==NULL){ //if there is one vehicle in the current list
            if(!strcmp(plate,temp->plateNumber)){
                now=time(NULL);
                temp->outTime=now; // set the time out of the vehicle
                strftime(head->timeOut, 20, "%Y-%m-%d|%H:%M:%S", localtime(&now)); //extracted from http://stackoverflow.com/questions/3053999/c-convert-time-t-to-string-with-format-yyyy-mm-dd-hhmmss
                diff=temp->outTime-temp->inTime; //subtract time out and time in to get the difference in seconds
                temp->hour=diff/3600.0; //divide by 3600 to convert it to hours
                temp->pay=getPayment(temp); //compute the payment
                receipt(temp);
                insertHistory(temp); //write to the file history.txt before deleting the vehicle
                head=NULL;
                free(head);
                return;
            }
        }
        else{
            while(temp!=NULL){ //traverse to every vehicle in the list until the plate to be search is found
                if(!strcmp(plate,temp->plateNumber)){
                    if(prev==NULL){
                        head=temp->next;
                        now=time(NULL);
                        temp->outTime=now;
                        strftime(temp->timeOut, 20, "%Y-%m-%d|%H:%M:%S", localtime(&now)); //extracted from http://stackoverflow.com/questions/3053999/c-convert-time-t-to-string-with-format-yyyy-mm-dd-hhmmss
                        diff=temp->outTime-temp->inTime; //subtract time out and time in to get the difference in seconds
                        temp->hour=diff/3600.0; //divide by 3600 to convert it to hours
                        temp->pay=getPayment(temp);
                        receipt(temp);
                        insertHistory(temp);
                        temp->next=NULL;
                        free(temp);
                        c++; //flag to know if the plate number is found
                        return;
                    }
                    else{
                        prev->next=temp->next;
                        now=time(NULL);
                        temp->outTime=now;
                        strftime(temp->timeOut, 20, "%Y-%m-%d|%H:%M:%S", localtime(&now)); //extracted from http://stackoverflow.com/questions/3053999/c-convert-time-t-to-string-with-format-yyyy-mm-dd-hhmmss
                        diff=temp->outTime-temp->inTime; //subtract time out and time in to get the difference in seconds
                        temp->hour=diff/3600.0; //divide by 3600 to convert it to hours
                        temp->pay=getPayment(temp);
                        receipt(temp);
                        insertHistory(temp);
                        temp->next=NULL;
                        free(temp);
                        c++; //flag to know if the plate number is found
                        return;
                    }
                }
                prev=temp; //set the value of prev to temp and point to next record
                temp=temp->next;
            }
        }
        if(c==0)
            printf("No car with the same plate number!\n");

        printf("\n");
    }
}

void receipt(car *temp){
    char *strVecType;
    strVecType=malloc(10*sizeof(char));
    getVecType(strVecType,temp->vecType);
    printf("+---------------------------------------+\n");
    printf("|                                       |\n");
    printf("|             CAR RECEIPT               |\n");
    printf("|                                       |\n");
    printf("|---------------------------------------|\n");
    printf("|                                       |\n");
    printf("|  Plate Number   : %s\t\t        |\n",temp->plateNumber);
    printf("|  Vehicle Type   : %s\t        |\n",strVecType);
    printf("|  Vehicle Color  : %s\t        |\n",temp->color);
    printf("|  Time in        : %s\t|\n",temp->timeIn);
    printf("|  Time out       : %s\t|\n",temp->timeOut);
    printf("|  Total hours    : %lf\t\t|\n",temp->hour);
    printf("|                                       |\n");
    printf("|  TOTAL DUE                 %d         |\n",temp->pay);
    printf("|  Cash                      %d         |\n",temp->pay);
    printf("|                                       |\n");
    printf("|             THANK YOU!                |\n");
    printf("+---------------------------------------+\n");
    printf("\n\nPress enter to continue....");
    getchar();
}

int getPayment(car *curr){
    double hour=ceil(curr->hour); //get the ceiling value for the hour
    int init, add=0;

    if(curr->vecType=='a'){ //if the vehicle type is a, the amount to be paid is 40 pesos
        return 40;
    }
    else if(curr->vecType=='b'){ //if the vehicle type is b, the amount to be paid is 40 pesos for the first three hours. However, if greater than 3 hours then the amount will be (hour-3)*20
        init=40;
        if(hour>3){
            add=(hour-3)*20;
        }
        return init+add;
    }
    else if(curr->vecType=='c'){ //if the vehicle type is c, the amount to be paid is 100 pesos for the first three hours. However, if greater than 3 hours then  the amount will be (hour-3)*40;
        init=100;
        if(hour>3){
            add=(hour-3)*40;
        }
        return init+add;
    }
}

void insertHistory(car *temp){ //write all removed and paid vehicles to history.txt
    FILE *fp;
    char strVecType[10];

    fp=fopen("history.txt","a");
    getVecType(strVecType,temp->vecType);
     fprintf(fp,"%s, %s, %s, %s, %s, %lf, %d\n",temp->plateNumber,strVecType,temp->color,temp->timeIn,temp->timeOut,temp->hour,temp->pay);
     fclose(fp);
}

void load(){ //read file
   FILE *fp = fopen("vehicle.txt", "r");
    const char s[2] = ",";
    char *token, *buff;
    car *temp;
    int i;
    if(fp != NULL){
        char line[100];
        head=NULL;
        while(fgets(line, sizeof line, fp) != NULL){
            temp=head;

            temp=(car*)malloc(sizeof(car));
            token = strtok(line, s); //extracted from http://stackoverflow.com/questions/26443492/read-comma-separated-values-from-a-text-file-in-c
            //token gets the delimeter in the file which is for this example the ","
            for(i=0;i<5;i++){ //for loop to get the string that is sepatated by comma
                if(i==0){ //gets the first string
                   strcpy(temp->plateNumber,token);
             }
                if(i==1){ //gets the second string
                    if(strcmp(token,"2-wheeled")==0){
                        temp->vecType='a';
                     }
                    else if (strcmp(token,"4-wheeled")==0){
                        temp->vecType='b';
                     }
                    else if (strcmp(token,"truck")==0){
                        temp->vecType='c';
                    }
                }
                if(i==2){ //gets the third string
                    strcpy(temp->color,token);
                }
                if(i==3){ //gets the fourth string
                    strcpy(temp->timeIn,token);
                }
                if(i==4){ //gets the fifth string
                    temp->inTime=atol(token);
                }
                token=strtok(NULL,s);
          }
          //program to assign temp to head link list
          if (head==NULL){
              head=temp;
              head->next=NULL;
          }
          else{
                 temp->next=head;
                 head=temp;
          }
        }
    }
    fclose(fp);
}

void getVecType(char str[], char type){
    if(type=='a') {
        strcpy(str,"2-wheeled");
    }
    else if(type=='b') {
        strcpy(str,"4-wheeled");
    }
    else if(type=='c') {
        strcpy(str,"truck");
    }
}

// the following procedures below are for the designs of the program

void cardesign(){

    printf("\t                     ___________________\n");
    printf("\t                 _.-'_____  _________ _`.\n");
    printf("\t                .` ,'      ||         | `.`.\n");
    printf("\t              .` ,'        ||         |   `.`.\n");
    printf("\t            .`  /          ||         |  ,' ] `....___\n");
    printf("\t          _`__.'''''''''''''''''''''''`''''''''|..___ `-.._\n");
    printf("\t        .'                  [='                '     `'-.._`.\n");
    printf("\t      :/.'''''''''''''''''''|''''''''''''''''''|'''''''''''\' \n");
    printf("\t     /|//   ,',---.`.       |                  |  .',---.`.||\n");
    printf("\t    (':/   //' .-. `\\      __________________/  '/' .-. \\|_)\n");
    printf("\t     `-...'||  '-'  ||________,,,,,,,,,,,,,,,__.'||  '-'  ||-'\n");
    printf("\t          '.'.___.','                           '.'.___.','\n");
    printf("\t             '-.m.-'                               '-.m.-'\n"); //extracted from http://www.ascii-code.com/ascii-art/vehicles/cars.php

    printf("\n\t\t  #### ########     ###    ########  ##    ##\n");
    printf("\t\t   ##  ##     ##   ## ##   ##     ## ##   ##\n");
    printf("\t\t   ##  ##     ##  ##   ##  ##     ## ##  ##\n");
    printf("\t\t   ##  ########  ##     ## ########  #####\n");
    printf("\t\t   ##  ##        ######### ##   ##   ##  ##\n");
    printf("\t\t   ##  ##        ##     ## ##    ##  ##   ##\n");
    printf("\t\t  #### ##        ##     ## ##     ## ##    ##\n");

    printf("\n\nPress enter to continue....");

    getchar();

}

void header(){
    int i;

    printf("%c",201);
    for(i=0;i<40;i++){
        printf("%c",205);
    }
    printf("%c",187);
    printf("\n");
    printf("%c",186);
    for(i=0;i<13;i++){
        printf(" ");
    }
    printf("Welcome to iPark!");
    printf("\t\t %c",186);
    printf("\n");
    printf("%c",200);
    for(i=0;i<40;i++){
        printf("%c",205);
    }
    printf("%c",188);
    printf("\n");
}

void headerViewAll(){
    int i;
    printf("\n");
    printf("%c",201);
    for(i=0;i<40;i++){
        printf("%c",205);
    }
    printf("%c",187);
    printf("\n");
    printf("%c",186);
    for(i=0;i<13;i++){
        printf(" ");
    }
    printf("All vehicles: ");
    printf("\t\t %c",186);
    printf("\n");
    printf("%c",200);
    for(i=0;i<40;i++){
        printf("%c",205);
    }
    printf("%c",188);
    printf("\n");
}

void quitDesign(){
    printf("\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
    printf("*                                                                 *\n");
    printf("*                                                                 *\n");
    printf("*            Thank you for using iPark! Come again :)             *\n");
    printf("*                                                                 *\n");
    printf("*                                                                 *\n");
    printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
}

