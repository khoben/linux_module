/*
 * phone_book.c
 *
 *  Created on: Sep 25, 2019
 *      Author: khoben
 */

#include "phone_book.h"

struct PhoneBook *createPhoneBook(void)
{
	struct PhoneBook *newBook;
	newBook = vmalloc(sizeof(struct PhoneBook));
	newBook->lastId = 0;
	return newBook;
}

void loadPhoneBook(struct PhoneBook book)
{

}

void putRecord(struct PhoneBook *book, char* fio, char* adress,
		char* phoneNumber)
{
	struct PhoneBookRecord r;
	r.fio = fio;
	r.phoneNumber = phoneNumber;
	r.adress = adress;
	book->data[book->lastId++] = r;
}

char*
phoneBookRecordToString(char* text, unsigned bufferLenght,
		struct PhoneBookRecord record)
{
	char str[1024] =
	{ };
	strcat(str, record.fio);
	strcat(str, " ");
	strcat(str, record.adress);
	strcat(str, " ");
	strcat(str, record.phoneNumber);
	strcat(text, str);
	return text;
}

char*
getRecord(char* text, struct PhoneBook book, char* requestString)
{
	const int bufferLenght = 1024;
	const int bufferLenghtOneRow = 100;
	char answer[1024] = "Search for ";
	char buffer[1024] =
	{ };
	int lenOfRequest;
	int i;
	bool isEmpty = false;

	lenOfRequest = strlen(requestString) - 1;

	if (lenOfRequest == 0)
	{
		isEmpty = true;
		strncpy(answer, "Empty request. List all records:\n", bufferLenght);
	}
	else
	{
		strncpy(buffer, requestString, lenOfRequest);
		strcat(answer, buffer);
		strcat(answer, ":");
	}

	for (i = 0; i < book.lastId; i++)
	{
		if (isEmpty == false)
		{
			if (strncmp(book.data[i].fio, buffer, lenOfRequest) == 0
					|| strncmp(book.data[i].adress, buffer, lenOfRequest) == 0
					|| strncmp(book.data[i].phoneNumber, buffer, lenOfRequest)
							== 0)
			{
				strcat(answer, "\n");
				strncpy(answer,
						phoneBookRecordToString(answer, bufferLenghtOneRow,
								book.data[i]), bufferLenghtOneRow);
				strcat(answer, "\n");
			}
		}
		else
		{
			strcat(answer, "\n");
			strncpy(answer,
					phoneBookRecordToString(answer, bufferLenghtOneRow,
							book.data[i]), bufferLenghtOneRow);
			strcat(answer, "\n");
		}
	}
	strncpy(text, answer, bufferLenght);
	return text;
}
