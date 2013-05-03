#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
	char			*number;
	int			numbers;
	int			negative;
} NUMBER;


NUMBER number_new() {
	NUMBER num;
	num.number = NULL;
	num.numbers = 0;
	num.negative = 0;
	
	return num;
}


void number_destroy(NUMBER *num) {
	free(num->number);
	num->number = NULL;
	num->numbers = 0;

	return;
}


void number_insert_low(NUMBER *num, char number) {
	num->numbers++;
	num->number = realloc(num, sizeof(char) * num->numbers);
	memmove(&num->number[1], num->number, num->numbers - 1);
	num->number = num;

	return;
}


void number_insert_high(NUMBER *num, char number) {
	num->numbers++;
	num->number = realloc(num, sizeof(char) * num->numbers);
	num->number[num->numbers - 1] = number;

	return;
}


void number_delete_high(NUMBER *num) {
	num->numbers--;
	num->number = realloc(num, sizeof(char) * num->numbers);

	return;
}


void number_delete_low(NUMBER *num) {
	num->numbers--;
	memmove(&num->number[1], num->number, num->numbers);
	num->numbers = realloc(sizeof(char) * num->numbers);

	return;
}


char number_get_digit(NUMBER *num, int digit) {
	if (digit >= num->numbers || digit < 0)
		return -10;
	return num->number[digit];
}


int number_get_digits(NUMBER *num) {
	return num->numbers;
}


int number_add(NUMBER


char *number_to_string(NUMBER *num) {
	int i;
	char *str = malloc(num->numbers + 1);

	for (i = 0; i < num->numbers; i++)
		str[i] = num->number[i] + 0x30;
	str[i] = 0;
	return str;
}


NUMBER number_from_string(char *str) {
	NUMBER num;
	int i;

	num = number_new();
	for (i = 0; str[i]; i++)
		number_insert_low(str[i]);
	return num;
}


NUMBER number_add(NUMBER num1, NUMBER num2) {
	




int main(int argc, char **argv) {
	
