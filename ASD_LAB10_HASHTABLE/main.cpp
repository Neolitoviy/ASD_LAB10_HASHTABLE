#define _CRT_SECURE_NO_WARNINGS
#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    system("chcp 1251 & cls");
    hashTable = (Item**)malloc(TABLE_SIZE * sizeof(Item*));
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    loadDataFromFile("data.txt");

    while (1) {
        printf("\n1. ��������\n");
        printf("2. ��������\n");
        printf("3. �����\n");
        printf("4. ������� ���-�������\n");
        printf("5. ����������� ����� ����� � �����\n");
        printf("6. �����\n");
        printf("������ ��: ");

        int choice;
        scanf("%d", &choice);

        char film[50];
        int year;

        switch (choice) {
        case 1: {
            printf("������ (����� ��): ");
            scanf("%s %d",film, &year);
            insert(film, year, 0);
            break;
        }
        case 2: {
            printf("������ ����� ��� ���������: ");
            scanf("%s", film);
            deleteD(film);
            break;
        }
        case 3: {
            printf("������ ����� ��� ������: ");
            scanf("%s", film);
            Item* item = search(film);
            if (item != NULL) {
                printf("��'��� ��������: Գ���: %s, г�: %d\n", item->film, item->year);
            }
            else {
                printf("��'��� �� ��������\n");
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
            // ��������� �񳺿 �������� ���'��
            for (int i = 0; i < TABLE_SIZE; i++) {
                if (hashTable[i] != NULL) {
                    free(hashTable[i]);
                }
            }
            return 0;
        }
        default: {
            printf("������������ ����\n");
            rewind(stdin);
        }
        }
    }

    return 0;
}