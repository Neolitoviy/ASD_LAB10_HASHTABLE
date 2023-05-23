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
        printf("\n1. ��������\n");
        printf("2. ��������\n");
        printf("3. �����\n");
        printf("4. ������� ���-�������\n");
        printf("5. ����������� ����� ����� � �����\n");
        printf("6. �����\n");
        printf("������ ��: ");

        int choice;
        scanf("%d", &choice);

        char key[50];
        char film[50];
        int year;

        switch (choice) {
        case 1: {
            printf("������ (���� ����� ��): ");
            scanf("%s %s %d", key, film, &year);
            insert(key, film, year);
            break;
        }
        case 2: {
            printf("������ ���� ��� ���������: ");
            scanf("%s", key);
            deleteD(key);
            break;
        }
        case 3: {
            printf("������ ���� ��� ������: ");
            scanf("%s", key);
            Item* item = search(key);
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
                    free(hashTable[i]->item);
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