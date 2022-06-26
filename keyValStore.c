#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

typedef struct Pair{
    char* key;
    char* value;
    int subscribers[10];
} Pair;

typedef struct Sub{
    char* key;
    pid_t process_id;
} Sub;


// Count of pairs in the storage
int numOfPairs = 200;
// The storage
Pair* storage;

/// Initialize the storage if not already done.
void initializeStorage(){
    if (storage == NULL) {
        printf("Storage initialized!\n");
        storage = malloc(numOfPairs * sizeof(*storage));
    }
}

/// Get the Index of the pair in search
int getIndexOfKey(char* key){
    for (int i = 0; i < numOfPairs; ++i) {
        if (storage[i].key != NULL && strncmp(storage[i].key, key, strlen(storage[i].key)) == 0){
            return i;
        }
    }
    return -1;
}

/// Print the complete storage
void printStorage(){
    for (int i = 0; i < numOfPairs; ++i) {
        printf("Index in storage %i\n \tKey: %s\n \tValue: %s\n", i, storage[i].key, storage[i].value);
    }
}

int put(char* key, char* value){
    initializeStorage();

    int indexOfKey = getIndexOfKey(key);

    // printf("Index of key: %i\n", indexOfKey);

    // If key is in storage change value of that key
    if (indexOfKey != -1){
        strcpy(storage[indexOfKey].value, value);
        return 1;
    }
    // Else if is space allocate new memory for key and value pointer
    else {
        for (int i = 0; i < numOfPairs; ++i) {
            if (storage[i].key == NULL){
                puts("PUT NEW VALUE");
                storage[i].key = (char*) malloc(sizeof (char*));
                strcpy(storage[i].key, key);
                storage[i].value = (char*) malloc(sizeof (char*));
                strcpy(storage[i].value, value);
                for(int j = 0; j < 10; ++j){
                    storage->subscribers[j] = 0;
                }
                return 0;
            }
        }
    }

    // Else storage is full
    // Maybe create extra function for extending the array (realloc)

    return -1;
}

int get(char* key, char* res){
    int indexOfKey = getIndexOfKey(key);

    //printf("Index of key: %i\n", indexOfKey);

    // If key is not in storage return with -1
    if (indexOfKey == -1)
        return -1;

    // Else copy the value in the string reference
    strcpy(key, storage[indexOfKey].key);
    strcpy(res, storage[indexOfKey].value);

    return 0;
}

int del(char* key){
    int indexOfKey = getIndexOfKey(key);

    // If key is not in storage return with -1
    if (indexOfKey == -1)
        return -1;

    strcpy(key, storage[indexOfKey].key);

    // Free the memory previously allocated
    free(storage[indexOfKey].key);
    free(storage[indexOfKey].value);
    // Reset the pointer of the struct to NULL
    storage[indexOfKey].key = NULL;
    storage[indexOfKey].value = NULL;

    return 0;
}

int sub(char* key, int clientNo){

    printf("sub start\n");
    int indexOfKey = getIndexOfKey(key);

    // If key is not in storage return with -1
    if (indexOfKey == -1)
        return -1;

    printf("sub got key index\n");
    for(int i = 0; i < 10; ++i){
        if(storage[indexOfKey].subscribers[i] == 0){
            storage[indexOfKey].subscribers[i] = clientNo;
            printf("sub added subscriber to array\n");
            printf("index: %i, sub number: %i\n", i, clientNo);
            return 0;
        }
    }
    printf("sub failed\n");
    return -1;
}

int pub(char* key, int * subscribers) {

    printf("pub start\n");
    int indexOfKey = getIndexOfKey(key);
    int j = 0;

    // If key is not in storage return with -1
    if (indexOfKey == -1)
        return -1;

    printf("pub got key index\n");
    if(storage[indexOfKey].subscribers[0] == 0)
        return -1;

    printf("pub sub array not empty\n");
    while(storage[indexOfKey].subscribers[j] != 0){
        subscribers[j] = storage[indexOfKey].subscribers[j];
        ++j;
    }
    j = 0;
    printf("pub filled array\n");


    return 0;
}

void removeSub(int subscriber){
    int subPos, lastPos = 12;

    for(int i; i < 10; ++i){

        if(storage[i].subscribers[0] == 0)
            continue;

        for(int j = 0; j < 10; ++j){

            if(storage[i].subscribers[j] == 0)
                lastPos = j-1;

            if(storage[i].subscribers[j] == subscriber)
                subPos = j;

            if(subPos != 12 && lastPos != 12) {
                if (subPos != lastPos)
                    storage[i].subscribers[subPos] = storage[i].subscribers[lastPos];

                storage[i].subscribers[lastPos] = 0;
            }
        }
        subPos = 0;
        lastPos = 0;
    }
}