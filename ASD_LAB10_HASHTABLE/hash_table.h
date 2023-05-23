#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct {
    char film[50];
    int year;
} Item;

typedef struct {
    char key[50];
    Item* item;
} HashItem;

extern HashItem** hashTable;
extern int TABLE_SIZE;

unsigned int hashPJW(const char* key);
void resizeHashTable();
void insert(char* key, char* film, int year);
Item* search(const char* key);
void deleteD(const char* key);
void loadDataFromFile(const char* filename);
void printHashTable();

#endif