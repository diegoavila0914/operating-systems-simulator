#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

// Global Constants
#define EXIT 0
#define INVALID -1
#define MEMORY 1
#define FILES 2
#define THREAD 3
#define FIRST 0
#define BEST 1
#define WORST 2
#define NEXT 3
#define PAGE 4
#define FIFO 0
#define LRU 1
#define FRAMES 3
#define LOOP 3

// Function Prototypes
int displayMenu();
void clearScreen();
void memoryManagement();
void nextFit(int blockSize[], int blocks, int processSize[], int processes);
void firstFit(int blockSize[], int blocks, int processSize[], int processes);
void bestFit(int blockSize[], int blocks, int processSize[], int processes);
void worstFit(int blockSize[], int blocks, int processSize[], int processes);
void displayProcess(int allocation[], int processes, int processSize[]);
void pageReplacement();
void lru();
int findLRU(int time[]);
void fifo();
void displayPages(int page, int allocation[]);
void fileAttributes();
void printAttributes(char name[], struct stat statBuff);
void multiThreads();
void* threadFunction(void* vargp);


// Main Fucntion
int main()
{
    // variable declaration
    int choice = INVALID;

    // while it is not equal to 0, perform select OS Management
    while(choice != EXIT){
        choice = displayMenu();
        switch(choice)
        {
        case 1:
            memoryManagement();
            break;
        case 2:
            fileAttributes();
            break;
        case 3:
            multiThreads();
            break;
        case 4:
            pageReplacement();
            break;
        case 0:
            exit(0);
        default:
            printf("\nError! Reference Source not found.\n\n");

        }
    }

    return 0;

}

//Display Menu
int displayMenu(){
    //Variable Declaration
    int choice = INVALID;

    // while choice is equal to -1, display the managemet menu
    while(choice == INVALID)
    {
        printf("\t\t\t\t****** Operating System Management Menu**********\n\n");
        printf("Select the OS program to run, enter the number of your selction.\n");
        printf("1. Memory Management\n");
        printf("2. File Managment\n");
        printf("3. Multithreading\n");
        printf("4. Page Replacement\n");
        printf("0. Exit\n");
        scanf("%d", &choice);

        if(choice <= EXIT && choice >= 5)
            choice = INVALID;

    }

    return choice;

}

//ClearScreen function
void clearScreen(){

    char enter;
    printf("\n\t\t\t(Hit Enter to Continue)\n\n\n");
    scanf("%c", &enter);
    // Windows
    system("cls");
    // Mac/Linux
//  system("clear");

}

// Displays all the partioning algorithms for memory management
void memoryManagement(){

    clearScreen();

    //Variable Declaration
    int algorithm;

    printf("\n********* Memory Manegment *********\n\n");
    // Loops through each memory managemnt algorithms
    for(algorithm = 0; algorithm <= NEXT; algorithm++){
        //declares arrays and integer varibales
        int blockSize[] = {80, 10, 65, 35, 70};
        int processSize[] = {25, 70, 5, 45, 60};
        int blocks = sizeof(blockSize) / sizeof(blockSize[0]);
        int processes = sizeof(processSize) / sizeof(processSize[0]);

        //Condtional staments that calls partioning algorithms accordingly
        if(algorithm == FIRST){
            firstFit(blockSize, blocks, processSize, processes);
        }
        else if(algorithm == BEST){
            bestFit(blockSize, blocks, processSize, processes);
        }
        else if(algorithm == WORST){
            worstFit(blockSize, blocks, processSize, processes);
        }
        else if(algorithm == NEXT){
            nextFit(blockSize, blocks, processSize, processes);
        }

    }
   /* while(algorithm <= NEXT){

//        printf("\n0. First Fit\n");
//        printf("1. Best Fit\n");
//        printf("2. Worst Fit\n");
//        printf("3. Next Fit\n");
//        printf("Please Enter the Algorithm You Want: ");
//        scanf("%d", &algorithm);

        int blockSize[] = {80, 10, 65, 35, 70};
        int processSize[] = {25, 70, 5, 45, 60};
        int blocks = sizeof(blockSize) / sizeof(blockSize[0]);
        int processes = sizeof(processSize) / sizeof(processSize[0]);

        if(algorithm == FIRST){
            firstFit(blockSize, blocks, processSize, processes);
        }
        else if(algorithm == BEST){
            bestFit(blockSize, blocks, processSize, processes);
        }
        else if(algorithm == WORST){
            worstFit(blockSize, blocks, processSize, processes);
        }
        else if(algorithm == NEXT){
            nextFit(blockSize, blocks, processSize, processes);
        }
       // else if(algorithm != FIRST || algorithm != BEST || algorithm != WORST || algorithm != NEXT)
           //
        //printf("\nError! Reference Source not found.\n\n");


        algorithm++;
    }*/

}

// Next Fit PArtioning Algorithm
void nextFit(int blockSize[], int blocks, int processSize[], int processes){
    // Variable Declaration
    int allocation[processes];
    int id = 0;

    memset(allocation, INVALID, sizeof(allocation));

    // Outer loop
    for(int i = 0; i < processes; i++){
         // Inner loop
        while(id < blocks){
            // updates allocation array if curr block size is greater than curr process size
            if(blockSize[id] >= processSize[i]){
                allocation[i] = id;
                blockSize[id] = blockSize[id] - processSize[i];
                break;

            }
            else
                id = (id + 1) % blocks;

        }
    }

    printf("\n****** Next Fit ********\n\n");

    // Calls displayProcess function
    displayProcess(allocation, processes, processSize);

}

// First Fit Partioning Algorithm
void firstFit(int blockSize[], int blocks, int processSize[], int processes){

    // Variable Declaration
    int allocation[processes];

    memset(allocation, INVALID, sizeof(allocation));

    // Outer loop
    for(int i = 0; i < processes; i++){
        // Inner loop
        for(int j = 0; j < blocks; j++){
            // updates allocation array if curr block size is greater than curr process size
            if(blockSize[j] >= processSize[i]){
                allocation[i] = j;
                blockSize[j] = blockSize[j] - processSize[i];
                break;

            }
        }
    }

    printf("\n****** First Fit ********\n\n");

    // Calls displayProcess function
    displayProcess(allocation, processes, processSize);

}

// Best Fit Partioning Algorithm
void bestFit(int blockSize[], int blocks, int processSize[], int processes){
    // Variable Declaration
    int allocation[processes];

    memset(allocation, INVALID, sizeof(allocation));

    // Outer loop
    for(int i = 0; i < processes; i++){
        int bestIdx = INVALID;

         // Inner loop
        for(int j = 0; j < blocks; j++){
            // if curr block size is greater than curr process size
            if(blockSize[j] >= processSize[i]){
                if(bestIdx == INVALID)
                    bestIdx = j;
                else if(blockSize[bestIdx] > blockSize[j])
                    bestIdx = j;
            }
        }

        // updates allocation array if best fit value is not equal to INVALID
        if(bestIdx != INVALID){
            allocation[i] = bestIdx;
            blockSize[bestIdx] = blockSize[bestIdx] - processSize[i];
        }

    }

    printf("\n****** Best Fit ********\n\n");

    // Calls displayProcess function
    displayProcess(allocation, processes, processSize);

}

// Worst Fit PArtioning Algorithm
void worstFit(int blockSize[], int blocks, int processSize[], int processes){
    // Variable Declaration
    int allocation[processes];

    memset(allocation, INVALID, sizeof(allocation));

    // Outer Loop
    for(int i = 0; i < processes; i++){
        int wstIdx = INVALID;

         // Inner loop
        for(int j = 0; j < blocks; j++){
            // if curr block size is greater than curr process size
            if(blockSize[j] >= processSize[i]){
                if(wstIdx == INVALID)
                    wstIdx = j;
                else if(blockSize[wstIdx] < blockSize[j])
                    wstIdx = j;
            }
        }

        // updates allocation array if worst fit value is not equal to INVALID
        if(wstIdx != INVALID){
            allocation[i] = wstIdx;
            blockSize[wstIdx] = blockSize[wstIdx] - processSize[i];
        }

    }

    printf("\n****** Worst Fit ********\n\n");

    // Calls displayProcess function
    displayProcess(allocation, processes, processSize);

}

// Displays data for each partioning algorihtm
void displayProcess(int allocation[], int processes, int processSize[]){

    printf("\nProcess No.\tProcess Size\tBlock no.\n");
    // Loops through num of procesess to print process num, size and block num
    for(int i = 0; i < processes; i++){
        printf("%d\t\t%d\t\t", i+1, processSize[i]);

        // if index of allocation array is -1(INVALID)
        if(allocation[i] == INVALID)
        {
           printf("Not allocated\n");
        }
        // outputs current allocation
        else
        {
            printf("%d\n", (allocation[i] + 1));
        }

    }

    printf("\n");

}

// Displays Page replacements functions accordingly
void pageReplacement(){
    clearScreen();
    // VAriable Declaration
    int algorithm;

    // Loops through each page replacemnt function
    for(algorithm = 0; algorithm <= LRU; algorithm++){
        if(algorithm == FIFO)
            fifo();
        else if(algorithm == LRU)
            lru();
    }

}

// First in First out Page Replacement Function
void fifo(){
    // Displays Output headers
    printf("*********** Page Replacement ************\n\n");
    printf("*********** First In First Out ***********\n\n");
    printf("\tPage\tFrame 1\tFrame 2\tFrame 3\n\n");

    // Variable Declartion
    int pageRequests[] = {4, 1, 2, 4, 2, 5, 1, 3, 6};
    int pageFaults = 0;
    int allocation[FRAMES];
    int present;
    int pages = sizeof(pageRequests) / sizeof(pageRequests[0]);

    //Initializes allocation array
    memset(allocation, INVALID, sizeof(allocation));

    // Outer loop, iterating through the num of pages
    for(int i = 0; i < pages; i++){
        present = 0;
        // Inner loop, iterating through the num of frames
        for(int j = 0; j < FRAMES; j++){
            // Increments presents variable and decrements pageFaults if current pageRequests equals current frame allocation
            if(pageRequests[i] == allocation[j]){
                present++;
                pageFaults--;
            }

        }
        // Increments pageFaults variable
        pageFaults++;

        // Updates allocation array if pageFAults is less than/equal to FRAMES and if present equals zero
        if(pageFaults <= FRAMES && present == 0)
            allocation[i] = pageRequests[i];

        // Replaces the page at the front of the queue if present variable equals zero
        else if(present == 0)
            allocation[(pageFaults - 1) % FRAMES] = pageRequests[i];

        // Calls displayProcess function
        displayPages(pageRequests[i], allocation);

    }

     // Displays total num of page faults in fifo
     printf("\nTotal Page Faults = %d\n\n", pageFaults);

}

// Least recentely Used Page Replacement Function
void lru(){
    // Displays Output headers
    printf("*********** Least Recently Used ***********\n\n");
    printf("\tPage\tFrame 1\tFrame 2\tFrame 3\n\n");

    // Variable Declartion
    int pageRequests[] = {4, 1, 2, 4, 2, 5, 1, 3, 6};
    int pageFaults = 0;
    int allocation[FRAMES];
    int pages = sizeof(pageRequests) / sizeof(pageRequests[0]);
    int counter;
    int time[10];
    int flag1;
    int flag2;
    int position = 0;

    //Initializes allocation array
    memset(allocation, INVALID, sizeof(allocation));

    // Outer loop, iterating through the num of pages
    for(int i = 0; i < pages; i++){
        flag1 = 0;
        flag2 = 0;
        // Inner loop, iterating through the num of frames
        for(int j = 0; j < FRAMES; j++){
            // if current page request is equal to current frame allocation
            if(pageRequests[i] == allocation[j]){
                counter++;
                time[j] = counter;
                flag1 = 1;
                flag2 = 1;
                break;
            }

        }
        // if flag1 is zero
        if(flag1 == 0){
            // 2nd inner loop, iterating through num of frames
            for(int k = 0; k < FRAMES; k++){
                // if current element in the array is -1
                if(allocation[k] == INVALID){
                    counter++;
                    pageFaults++;
                    allocation[k] = pageRequests[k];
                    time[k] = counter;
                    flag2 = 1;
                    break;
                }

            }

        }
        // if flag2 is zero
        if(flag2 == 0){
            position = findLRU(time);
            counter++;
            pageFaults++;
            allocation[position] = pageRequests[i];
            time[position] = counter;
        }

        // Calls displayProcess function
        displayPages(pageRequests[i], allocation);

    }

    // Displays total num of page faults in lru
    printf("\nTotal Page Faults = %d\n\n", pageFaults);

}

// Finding Least Recentely used function, passing time array
int findLRU(int time[]){
    // Variable Declaration
    int position = 0;
    int minimum = time[0];

    // Loops through the num of frames
    for(int i = 0; i < FRAMES; i++){
        // Updates minimum and psoition variables if value stored in time array is les than minimum
        if(time[i] < minimum){
            minimum = time[i];
            position = i;
        }

    }

    return position;

}

// Dipslays values of the Page Replacements
void displayPages(int page, int allocation[]){
    printf("\t%d", page);

    // Loops through the num of frames
    for(int i = 0; i < FRAMES; i++){
        // ouputs a dash if curent index is negative one
        if(allocation[i] == INVALID){
           printf("\t-");
        }
        // outputs current allocation
        else{
            printf("\t%d", allocation[i]);
        }

    }

    // Outputs New line
    printf("\n");
}

// File Attributes Function
void fileAttributes(){
    // Variable Declaration
    struct stat statBuff;
    int err;
    struct dirent* de;
    DIR* dr = opendir(".");

    // Cannot open directory if dr variable is NULL
    if(dr == NULL)
        printf("Could not open current directory");

    // While reading the contents of the directory is not NULL
    while(de = readdir(dr)){
        err = stat(de->d_name, &statBuff);
        // Evaluates if err variable is -1/INVALID
        if(err == INVALID){
            printf("Error in Stat");
            printf("\nPress 'Enter' to continue...");
            getchar();
        }

        // Calls print attributes function
        printAttributes(de->d_name, statBuff);

    }

}

// Print Attributes Function
void printAttributes(char name[], struct stat statBuff){
    // Variable Declaration
    time_t t;
    char timeStr[100];

    // File Name
    printf("\t\t\tFile = %s\n\n", name);

    // File device id
    printf("Device id = %li\n", statBuff.st_dev);

    // File serial number
    printf("File serial number = %li\n", statBuff.st_ino);

    // File user id
    printf("File user id = %i\n", statBuff.st_uid);

    // File group id
    printf("File group id = %i\n", statBuff.st_gid);

    // File mode
    printf("File mode = %i\n", statBuff.st_mode);

    // Outputs "Owner permissions"
    printf("Owner permissions: ");
    if( statBuff.st_mode & S_IRUSR)
        printf("read ");
    if( statBuff.st_mode & S_IWUSR)
        printf("write ");
    if( statBuff.st_mode & S_IXUSR)
        printf("execute");

    // Outputs "Group Permissions"
    printf("\nGroup permissions: ");
    if(statBuff.st_mode & S_IRGRP)
        printf("read ");
    if(statBuff.st_mode & S_IWGRP)
        printf("write ");
    if(statBuff.st_mode & S_IXGRP)
        printf("execute");

    // Ouputs "Other permissions"
    printf("\nOthers permissions: ");
    if(statBuff.st_mode & S_IROTH)
        printf("read ");
    if(statBuff.st_mode & S_IWOTH)
        printf("write ");
    if(statBuff.st_mode & S_IXOTH)
        printf("execute");

    // File Creation date
    t = statBuff.st_ctime;
    strcpy(timeStr, ctime(&t));
    printf("\nDate created = %s", timeStr);

    // File last modified date
    t = statBuff.st_mtime;
    strcpy(timeStr, ctime(&t));
    printf("Date modified = %s", timeStr);

    // File last accessed date
    t = statBuff.st_atime;
    strcpy(timeStr, ctime(&t));
    printf("Time file was last accessed = %s", timeStr);

    // File size
    printf("File size = %li bytes\n\n", statBuff.st_size);

}

// Multithreading Function
void multiThreads(){
    // Variable Declaration
    const int SIZE = 7;
    int i;
    int error;
    pthread_t tid[SIZE];

    // Loops through constant int SIZE(7)
    for(i = 0; i < SIZE; i++){
        error = pthread_create(&tid[i], NULL, &threadFunction, (void*)&tid[i]);

        // Ouputs explicit text if error does not equal 0
        if(error != 0)
            printf("\nThread can't be created : [%s] Press 'Enter' to continue...", strerror(error));

    }

    // Loops through constant int SIZE(7)
    for(i = 0; i < SIZE; i++){
        pthread_join(tid[i], NULL);

    }

}

// Thread Function
void* threadFunction(void* vargp){
    // Variable Declaration
    int i;
    int* myid = (int*)vargp;

    // Outputs the thread # has started
    printf("-----------------------\n");
    printf("Thread ID %d has started\n", *myid);
    printf("\n-----------------------\n");

    // Loops through 3 iterations to print the thread number
    for(i = 0; i < LOOP; i++){
        printf("Thread ID %d printing iteration %d\n", *myid, i+1);

    }

    // Outputs when thread numver has finshed
    printf("\n------------------------\n");
    printf("Thread ID %d has finished\n", *myid);
    printf("------------------------\n");

    return NULL;

}
