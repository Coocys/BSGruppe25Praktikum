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
                storage[i].key = (char*) malloc(sizeof (char*));
                strcpy(storage[i].key, key);
                storage[i].value = (char*) malloc(sizeof (char*));
                strcpy(storage[i].value, value);
                for(int i; i < 10; ++i){
                    storage->subscribers[i] = 0;
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
    storage[indexOfKey].subscribers = NULL;

    return 0;
}

int sub(char* key, int clientNo){

    int indexOfKey = getIndexOfKey(key);

    // If key is not in storage return with -1
    if (indexOfKey == -1)
        return -1;

    for(int i = 0; i < 10; ++i){
        if(storage[indexOfKey].subscribers[i] == 0){
            storage[indexOfKey].subscribers[i] = clientNo;
            return 0;
        }
    }

    return -1;
}

int pub(char* key, int* subscribers) {

    int indexOfKey = getIndexOfKey(key);
    int j = 0;

    // If key is not in storage return with -1
    if (indexOfKey == -1)
        return -1;

    if(storage[indexOfKey].subscribers[0] == 0)
        return -1;

    while(storage[indexOfKey].subscribers[j] != 0){
        subscribers[j] = storage[indexOfKey].subscribers[j];
        ++j;
    }

    if(!subscribers)
        return -1;

    return 0;
}

void removeSub(int subscriber){
    int subscribers[10];
    int subPos, lastPos = 12;

    for(int i; i < (sizeof(storage)/ sizeof(Pair)); ++i){

        if(storage[i].subscribers[0] == 0)
            continue;

        for(int j = 0; j < 10; ++j){

            if(storage[i].subscribers[j] == 0)
                lastPos = j-1;

            if(storage[i].subscribers[j] == subscriber)
                subPos = j;

            if(subPos != 12 && lastPos != 12) {
                if (subPos != lastPos)
                    subscribers[subPos] = storage[i].subscribers[lastPos];

                storage[i].subscribers[lastPos] = 0;
            }
        }
        subPos = 0;
        lastPos = 0;
    }
}