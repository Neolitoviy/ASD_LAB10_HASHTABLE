#define _CRT_SECURE_NO_WARNINGS
#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int TABLE_SIZE = 17; // ��������� ���-�������
#define LOAD_FACTOR 0.7 // ���������� ������������ �������

HashItem** hashTable;
int size = 0;

// ��������� � UNIX OS
unsigned int hashPJW(const char* str) {
    unsigned int hash = 0;
    unsigned int test = 0;
    for (; *str; str++) {
        hash = (hash << 4) + (unsigned char)(*str);

        // ����������, �� � ������ 4 ��� ���� �����������
        if ((test = hash & 0xf0000000) != 0) {

            // ���� ���, �� �� �������� �������� XOR �� ����� � ������, �������� �� 24 ��� ������,
            // � �������� ��������� � ����, ���������� ������ 4 ���.

            hash = ((hash ^ (test >> 24)) & (0xfffffff));
        }
    }
    return hash % TABLE_SIZE; // ��������� �������� ����, �������� ������� ���-�������
}

void resizeHashTable() {
    int oldSize = TABLE_SIZE;
    TABLE_SIZE *= 2;  // �������� ����� ������� �����

    // ��������� ���� �������
    HashItem** newTable;
    newTable = (HashItem**)malloc(TABLE_SIZE * sizeof(HashItem*));
    for (int i = 0; i < TABLE_SIZE; i++) {
        newTable[i] = NULL;
    }

    // ���������� ��� � ����� ������� �� ����
    for (int i = 0; i < oldSize; i++) {
        if (hashTable[i] != NULL) {
            int index = hashPJW(hashTable[i]->key);
            int j = 0;
            while (newTable[(index + j * j) % TABLE_SIZE] != NULL) {
                j++;
            }
            newTable[(index + j * j) % TABLE_SIZE] = hashTable[i];
        }
    }

    // ��������� ���'���, ��� ����� ����� �������
    free(hashTable);

    // ��������� ��������� �� �������
    hashTable = newTable;
}

void insert(char* key, char* film, int year) {

    // ��������� ����� �������
    Item* item = (Item*)malloc(sizeof(Item));
    strcpy(item->film, film);
    item->year = year;

    int index = hashPJW(key);
    int i = 0;
    while (hashTable[(index + i * i) % TABLE_SIZE] != NULL) {
        i++;
    }

    // ��������� ����� ���-������� � ������ ���� �� ���-�������
    HashItem* hashItem = (HashItem*)malloc(sizeof(HashItem));
    strcpy(hashItem->key, key);
    hashItem->item = item;
    hashTable[(index + i * i) % TABLE_SIZE] = hashItem;
    size++;

    // ����������, �� �� ���������� ��������� ������������
    if ((float)size / TABLE_SIZE >= LOAD_FACTOR) {
        resizeHashTable();
    }
}

Item* search(const char* key) {
    int index = hashPJW(key);
    int i = 0;

    //����� � ��������� �� ��������� ������������� ����������
    while (hashTable[(index + i * i) % TABLE_SIZE] != NULL) {
        if (strcmp(hashTable[(index + i * i) % TABLE_SIZE]->key, key) == 0) {
            return hashTable[(index + i * i) % TABLE_SIZE]->item;
        }
        i++;
    }

    return NULL;
}

void deleteD(const char* key) {
    int index = hashPJW(key);
    int i = 0;

    // ������ � ��������� ������� �� ��������� ������������� ����������
    while (hashTable[(index + i * i) % TABLE_SIZE] != NULL) {
        if (strcmp(hashTable[(index + i * i) % TABLE_SIZE]->key, key) == 0) {
            free(hashTable[(index + i * i) % TABLE_SIZE]->item);
            free(hashTable[(index + i * i) % TABLE_SIZE]);
            hashTable[(index + i * i) % TABLE_SIZE] = NULL;
            size--;
            return;
        }
        i++;
    }
}

void loadDataFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("��������� ������� ���� %s\n", filename);
        return;
    }

    char key[50];
    char film[50];
    int year;
    while (fscanf(file, "%s %s %d", key, film, &year) != EOF) {
        insert(key, film, year);
    }

    fclose(file);
}

void printHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashTable[i] != NULL) {
            printf("������: %d, ����: %s, Գ���: %s, г�: %d\n", i, hashTable[i]->key, hashTable[i]->item->film, hashTable[i]->item->year);
        }
    }
}