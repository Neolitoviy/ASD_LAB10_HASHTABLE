#define _CRT_SECURE_NO_WARNINGS
#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int TABLE_SIZE = 17; // розмірність геш-таблиці
#define LOAD_FACTOR 0.7 // коефіцієнт завантаження таблиці

Item** hashTable;
int size = 0;

unsigned int hashPJW(const char* str) {
	 unsigned int hash = 0;
	 unsigned int test = 0;
	 for (; *str; str++) {
		  hash = (hash << 4) + (unsigned char)(*str);
		  if ((test = hash & 0xf0000000) != 0) {
				hash = ((hash ^ (test >> 24)) & (0xfffffff));
		  }
	 }
	 return hash % TABLE_SIZE;
}

int findIndex(const char* film) {
	 int index = hashPJW(film);
	 int i = 0;
	 while (hashTable[(index + i * i) % TABLE_SIZE] != NULL) {
		  i++;
	 }
	 return (index + i * i) % TABLE_SIZE;
}

void insert(const char* film, int year, int check_load_factor) {
	 if (search(film) != NULL) {
		  printf("Фільм %s вже існує в геш-таблиці\n", film);
		  return;
	 }
	 // Створюємо новий елемент
	 Item* item = (Item*)malloc(sizeof(Item));
	 strcpy(item->film, film);
	 item->year = year;

	 // Додаємо його до геш-таблиці
	 hashTable[findIndex(film)] = item;
	 size++;

	 if (check_load_factor && (float)size / TABLE_SIZE >= LOAD_FACTOR) {
		  resizeHashTable();
	 }
}

void resizeHashTable() {
	 int oldSize = TABLE_SIZE;
	 Item** oldTable = hashTable;
	 TABLE_SIZE *= 2; // Збільшення розміру на 2

	 hashTable = (Item**)malloc(sizeof(Item*) * TABLE_SIZE);
	 for (int i = 0; i < TABLE_SIZE; i++) {
		  hashTable[i] = NULL;
	 }

	 for (int i = 0; i < oldSize; i++) {
		  if (oldTable[i] != NULL) {
				insert(oldTable[i]->film, oldTable[i]->year, 0);
				free(oldTable[i]);
		  }
	 }

	 free(oldTable);
}

Item* search(const char* film) {
	 int index = hashPJW(film);
	 int i = 0;

	 while (hashTable[(index + i * i) % TABLE_SIZE] != NULL) {
		  if (strcmp(hashTable[(index + i * i) % TABLE_SIZE]->film, film) == 0) {
				return hashTable[(index + i * i) % TABLE_SIZE];
		  }
		  i++;
	 }

	 return NULL;
}

void deleteD(const char* key) {
    unsigned int index = hashPJW(key);

    while (hashTable[index] != NULL) {
        if (strcmp(hashTable[index]->film, key) == 0) {
            
            // Видалення
            free(hashTable[index]);
            hashTable[index] = NULL;

            // Зсування
            unsigned int nextIndex = (index + 1) % TABLE_SIZE;
            while (hashTable[nextIndex] != NULL) {

                hashTable[index] = hashTable[nextIndex];
                hashTable[nextIndex] = NULL;

                nextIndex = (nextIndex + 1) % TABLE_SIZE;
            }
            return;
        }

        ++index;
        index %= TABLE_SIZE;
    }
}

void loadDataFromFile(const char* filename) {
	 FILE* file = fopen(filename, "r");
	 if (file == NULL) {
		  printf("Неможливо відкрити файл %s\n", filename);
		  return;
	 }
	 char film[50];
	 int year;
	 while (fscanf(file, "%s %d", film, &year) != EOF) {
		  insert(film, year, 1);
	 }
	 fclose(file);
}

void printHashTable() {
	 for (int i = 0; i < TABLE_SIZE; i++) {
		  if (hashTable[i] != NULL) {
				printf("Індекс: %d, Фільм: %s, Рік: %d\n", i, hashTable[i]->film, hashTable[i]->year);
		  }
	 }
}