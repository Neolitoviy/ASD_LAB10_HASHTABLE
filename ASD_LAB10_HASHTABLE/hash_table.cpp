#define _CRT_SECURE_NO_WARNINGS
#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int TABLE_SIZE = 17; // розмірність геш-таблиці
#define LOAD_FACTOR 0.7 // коефіцієнт завантаження таблиці

Item** hashTable;
int size = 0;

// Гешування в UNIX OS
unsigned int hashPJW(const char* str) {
	 unsigned int hash = 0;
	 unsigned int test = 0;
	 for (; *str; str++) {
		  hash = (hash << 4) + (unsigned char)(*str);

		  // Перевіряємо, чи є старші 4 біти хешу ненульовими
		  if ((test = hash & 0xf0000000) != 0) {

				// Якщо так, то ми виконуємо операцію XOR між гешом і тестом, зсунутим на 24 біти вправо,
				// і зберігаємо результат у геші, виключаючи старші 4 біти.

				hash = ((hash ^ (test >> 24)) & (0xfffffff));
		  }
	 }
	 return hash % TABLE_SIZE; // Повертаємо значення гешу, обмежене розміром геш-таблиці
}

void resizeHashTable() {
	 int oldSize = TABLE_SIZE;
	 Item** oldTable = hashTable;
	 TABLE_SIZE *= 2;

	 // Створюємо нову таблицю з подвоєним розміром
	 hashTable = (Item**)malloc(sizeof(Item*) * TABLE_SIZE);
	 for (int i = 0; i < TABLE_SIZE; i++) {
		  hashTable[i] = NULL;
	 }

	 // Перерахуємо хеш для кожного елемента в старій таблиці і встановимо його в нову таблицю
	 for (int i = 0; i < oldSize; i++) {
		  if (oldTable[i] != NULL) {
				int index = hashPJW(oldTable[i]->film);
				while (hashTable[index] != NULL) {
					 index = (index + i * i) % TABLE_SIZE;
				}
				hashTable[index] = oldTable[i];
		  }
	 }

	 // Звільнити пам'ять старої таблиці
	 free(oldTable);
}

void insert(char* film, int year) {

	 // Створюємо новий елемент
	 Item* item = (Item*)malloc(sizeof(Item));
	 strcpy(item->film, film);
	 item->year = year;

	 int index = hashPJW(film);
	 int i = 0;
	 while (hashTable[(index + i * i) % TABLE_SIZE] != NULL) {
		  i++;
	 }

	 // Додаємо його до геш-таблиці
	 hashTable[(index + i * i) % TABLE_SIZE] = item;
	 size++;

	 // Перевіряємо, чи не перевищено допустиме навантаження
	 if ((float)size / TABLE_SIZE >= LOAD_FACTOR) {
		  resizeHashTable();
	 }
}

Item* search(const char* film) {
	 int index = hashPJW(film);
	 int i = 0;

	 //Пошук і порівняння за допомогою квадратичного пробування
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

					 Item* tmpItem = hashTable[nextIndex];
					 hashTable[nextIndex] = NULL;

					 insert(tmpItem->film, tmpItem->year);

					 free(tmpItem);

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
		  insert(film, year);
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