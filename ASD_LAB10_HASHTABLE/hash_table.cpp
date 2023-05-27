#define _CRT_SECURE_NO_WARNINGS
#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int TABLE_SIZE = 17; // ��������� ���-�������
#define LOAD_FACTOR 0.7 // ���������� ������������ �������

Item** hashTable;
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
	 Item** oldTable = hashTable;
	 TABLE_SIZE *= 2;

	 // ��������� ���� ������� � �������� �������
	 hashTable = (Item**)malloc(sizeof(Item*) * TABLE_SIZE);
	 for (int i = 0; i < TABLE_SIZE; i++) {
		  hashTable[i] = NULL;
	 }

	 // ���������� ��� ��� ������� �������� � ����� ������� � ���������� ���� � ���� �������
	 for (int i = 0; i < oldSize; i++) {
		  if (oldTable[i] != NULL) {
				int index = hashPJW(oldTable[i]->film);
				while (hashTable[index] != NULL) {
					 index = (index + i * i) % TABLE_SIZE;
				}
				hashTable[index] = oldTable[i];
		  }
	 }

	 // �������� ���'��� ����� �������
	 free(oldTable);
}

void insert(char* film, int year) {

	 // ��������� ����� �������
	 Item* item = (Item*)malloc(sizeof(Item));
	 strcpy(item->film, film);
	 item->year = year;

	 int index = hashPJW(film);
	 int i = 0;
	 while (hashTable[(index + i * i) % TABLE_SIZE] != NULL) {
		  i++;
	 }

	 // ������ ���� �� ���-�������
	 hashTable[(index + i * i) % TABLE_SIZE] = item;
	 size++;

	 // ����������, �� �� ���������� ��������� ������������
	 if ((float)size / TABLE_SIZE >= LOAD_FACTOR) {
		  resizeHashTable();
	 }
}

Item* search(const char* film) {
	 int index = hashPJW(film);
	 int i = 0;

	 //����� � ��������� �� ��������� ������������� ����������
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
				
				// ���������
				free(hashTable[index]);
				hashTable[index] = NULL;

				// ��������
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
		  printf("��������� ������� ���� %s\n", filename);
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
				printf("������: %d, Գ���: %s, г�: %d\n", i, hashTable[i]->film, hashTable[i]->year);
		  }
	 }
}