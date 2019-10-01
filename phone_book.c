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

void freePhoneBook(void)
{

}

void loadPhoneBook(struct PhoneBook *book, char* buf, unsigned bufLenght)
{
	if (buf == NULL || strlen(buf) == 0)
	{
		printk(KERN_ALERT "empty buffer loads");
		return;
	}
	char bufLine[128] =
	{ };
	char fioBuf[128] =
	{ };
	char adressBuf[128] =
	{ };
	char phoneBuf[128] =
	{ };

	char ch = buf[0];
	int id = 0, idCharLine = 0;
	bool isFirstLine = true;
	while (id < bufLenght)
	{
		//skip first line
		if (ch != '\n')
		{
			if (!isFirstLine)
			{
				//append symbols to line buffer
				bufLine[idCharLine] = ch;
				idCharLine++;
			}
		}
		else
		{
			if (isFirstLine)
				isFirstLine = false;
			else
			{
				memset(&fioBuf[0], 0, sizeof(fioBuf));
				memset(&adressBuf[0], 0, sizeof(adressBuf));
				memset(&phoneBuf[0], 0, sizeof(phoneBuf));
				//newline is done
				bufLine[idCharLine] = '\0';
				idCharLine = 0;
				printk(KERN_INFO "line: %s", bufLine);
				sscanf(bufLine, "%s %s %s", fioBuf, adressBuf, phoneBuf);
				//copy struct phonebookrecord to phonebook
				putRecord(book, fioBuf, adressBuf, phoneBuf);
				printk(KERN_INFO "%s %s %s", fioBuf, adressBuf, phoneBuf);
		}
	}

	id++;
	ch = buf[id];
	if (ch == '\0')
		break;
}
}

void putRecord(struct PhoneBook *book, char* fio, char* adress,
	char* phoneNumber)
{
	struct PhoneBookRecord r;
	strcpy(r.fio, fio);
	strcpy(r.phoneNumber, phoneNumber);
	strcpy(r.adress, adress);
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
	if (book.data == NULL)
		return "";
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

	printk(KERN_INFO "start searching..");

	for (i = 0; i < book.lastId; i++)
	{
		if (isEmpty == false)
		{
			if (strncmp(book.data[i].fio, buffer, lenOfRequest) == 0
					|| strncmp(book.data[i].adress, buffer, lenOfRequest) == 0
					|| strncmp(book.data[i].phoneNumber, buffer, lenOfRequest) == 0)
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
