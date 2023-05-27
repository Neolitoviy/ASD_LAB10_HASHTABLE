#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct {
    char film[50];
    int year;
} Item;

extern int TABLE_SIZE;
extern Item** hashTable;

unsigned int hashPJW(const char* key);
void resizeHashTable();
void insert(char* film, int year);
Item* search(const char* key);
void deleteD(const char* key);
void loadDataFromFile(const char* filename);
void printHashTable();

#endif