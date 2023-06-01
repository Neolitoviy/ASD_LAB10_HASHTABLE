#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct {
    char film[50];
    int year;
} Item;

extern int TABLE_SIZE;
extern Item** hashTable;

unsigned int hashPJW(const char*);
int findIndex(const char*);
void resizeHashTable();
void insert(const char*, int, int);
Item* search(const char*);
void deleteD(const char*);
void loadDataFromFile(const char*);
void printHashTable();

#endif