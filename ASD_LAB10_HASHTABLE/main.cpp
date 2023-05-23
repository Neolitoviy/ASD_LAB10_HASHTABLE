#define _CRT_SECURE_NO_WARNINGS
#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    system("chcp 1251 & cls");
    hashTable = (HashItem**)malloc(TABLE_SIZE * sizeof(HashItem*));
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    loadDataFromFile("data.txt");

    while (1) {
        printf("\n1. Вставити\n");
        printf("2. Видалити\n");
        printf("3. Пошук\n");
        printf("4. Вивести геш-таблицю\n");
        printf("5. Завантажити більше даних з файлу\n");
        printf("6. Вийти\n");
        printf("Введіть дію: ");

        int choice;
        scanf("%d", &choice);

        char key[50];
        char film[50];
        int year;

        switch (choice) {
        case 1: {
            printf("Введіть (ключ фільм рік): ");
            scanf("%s %s %d", key, film, &year);
            insert(key, film, year);
            break;
        }
        case 2: {
            printf("Введіть ключ для видалення: ");
            scanf("%s", key);
            deleteD(key);
            break;
        }
        case 3: {
            printf("Введіть ключ для пошуку: ");
            scanf("%s", key);
            Item* item = search(key);
            if (item != NULL) {
                printf("Об'єкт знайдено: Фільм: %s, Рік: %d\n", item->film, item->year);
            }
            else {
                printf("Об'єкт не знайдено\n");
            }
            break;
        }
        case 4: {
            printHashTable();
            break;
        }
        case 5: {
            loadDataFromFile("more_data.txt");
            break;
        }
        case 6: {
            // Звільнення всієї динамічної пам'яті
            for (int i = 0; i < TABLE_SIZE; i++) {
                if (hashTable[i] != NULL) {
                    free(hashTable[i]->item);
                    free(hashTable[i]);
                }
            }
            return 0;
        }
        default: {
            printf("Неправильний вибір\n");
            rewind(stdin);
        }
        }
    }

    return 0;
}