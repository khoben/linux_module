/*
 * phone_book.h
 *
 *  Created on: Sep 25, 2019
 *      Author: khoben
 */

#ifndef PHONE_BOOK_H_
#define PHONE_BOOK_H_

#include <linux/string.h>
#include <linux/vmalloc.h>

#define MAX_RECORDS 10

struct PhoneBookRecord
{
	char* fio;
	char* adress;
	char* phoneNumber;
};

struct PhoneBook
{
	struct PhoneBookRecord data[MAX_RECORDS];
	int lastId;
};

struct PhoneBook*
createPhoneBook(void);

void
loadPhoneBook(struct PhoneBook book);

void
putRecord(struct PhoneBook *book, char* fio, char* adress, char* phoneNumber);

char*
getRecord(char* text, struct PhoneBook book, char* requestString);

char*
phoneBookRecordToString(char* text, unsigned bufferLenght,
		struct PhoneBookRecord record);

#endif /* PHONE_BOOK_H_ */
