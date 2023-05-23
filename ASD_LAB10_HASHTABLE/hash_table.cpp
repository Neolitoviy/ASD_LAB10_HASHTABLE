#define _CRT_SECURE_NO_WARNINGS
#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int TABLE_SIZE = 17; // розмірність геш-таблиці
#define LOAD_FACTOR 0.7 // коефіцієнт завантаження таблиці

HashItem** hashTable;
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
    TABLE_SIZE *= 2;  // Збільшуємо розмір таблиці вдвічі

    // Створюємо нову таблицю
    HashItem** newTable;
    newTable = (HashItem**)malloc(TABLE_SIZE * sizeof(HashItem*));
    for (int i = 0; i < TABLE_SIZE; i++) {
        newTable[i] = NULL;
    }

    // Переносимо дані зі старої таблиці до нової
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

    // Звільняємо пам'ять, яку займає стара таблиця
    free(hashTable);

    // Оновлюємо посилання на таблицю
    hashTable = newTable;
}

void insert(char* key, char* film, int year) {

    // Створюємо новий елемент
    Item* item = (Item*)malloc(sizeof(Item));
    strcpy(item->film, film);
    item->year = year;

    int index = hashPJW(key);
    int i = 0;
    while (hashTable[(index + i * i) % TABLE_SIZE] != NULL) {
        i++;
    }

    // Створюємо новий геш-елемент і додаємо його до геш-таблиці
    HashItem* hashItem = (HashItem*)malloc(sizeof(HashItem));
    strcpy(hashItem->key, key);
    hashItem->item = item;
    hashTable[(index + i * i) % TABLE_SIZE] = hashItem;
    size++;

    // Перевіряємо, чи не перевищено допустиме навантаження
    if ((float)size / TABLE_SIZE >= LOAD_FACTOR) {
        resizeHashTable();
    }
}

Item* search(const char* key) {
    int index = hashPJW(key);
    int i = 0;

    //Пошук і порівняння за допомогою квадратичного пробування
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

    // Шукаємо і видаляємо елемент за допомогою квадратичного пробування
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
        printf("Неможливо відкрити файл %s\n", filename);
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
            printf("Індекс: %d, Ключ: %s, Фільм: %s, Рік: %d\n", i, hashTable[i]->key, hashTable[i]->item->film, hashTable[i]->item->year);
        }
    }
}