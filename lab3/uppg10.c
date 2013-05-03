#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
	char			*number;
	int			numbers;
	int			negative;
} NUMBER;


NUMBER number_add(NUMBER num1, NUMBER num2);

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
	num->number = realloc(num->number, sizeof(char) * num->numbers);
	memmove(&num->number[1], num->number, num->numbers - 1);
	num->number[0] = number;

	return;
}


void number_insert_high(NUMBER *num, char number) {
	num->numbers++;
	num->number = realloc(num->number, sizeof(char) * num->numbers);
	num->number[num->numbers - 1] = number;

	return;
}


void number_delete_high(NUMBER *num) {
	num->numbers--;
	num->number = realloc(num->number, sizeof(char) * num->numbers);

	return;
}


void number_delete_low(NUMBER *num) {
	num->numbers--;
	memmove(&num->number[1], num->number, num->numbers);
	num->number = realloc(num->number, sizeof(char) * num->numbers);

	return;
}


char number_get_digit(NUMBER *num, int digit) {
	if (digit >= num->numbers || digit < 0)
		return 0;
	return num->number[digit];
}


int number_get_digits(NUMBER *num) {
	return num->numbers;
}


int number_sign(NUMBER *num) {
	return (num->negative ? -1 : 1);
}


void number_set_sign(NUMBER *num, int sign) {
	num->negative = (sign < 0);

	return;
}

void number_strip_leading_zero(NUMBER *num) {
	int i;
	for (i = number_get_digits(num) - 1; i >= 1; i--) {
		if (number_get_digit(num, i))
			return;
		number_delete_high(num);
	}

	return;
}

int number_greater(NUMBER num1, NUMBER num2) {
	int i, i1, i2;

	i = (num1.numbers > num2.numbers ? num1.numbers : num2.numbers);
	for (; i >= 0; i--) {
		i1 = number_get_digit(&num1, i);
		i2 = number_get_digit(&num2, i);
		if (i1 < i2)
			return 0;
		if (i1 > i2)
			return 1;
	}

	return 0;
}

NUMBER number_subtract(NUMBER num1, NUMBER num2) {
	int borrow, sub, i, j, flip;
	NUMBER tmp, res = number_new();

	if (num2.negative) {
		num2.negative = 0;
		return number_add(num1, num2);
	}

	if (num1.negative) {
		num1.negative = 0;
		res = number_add(num2, num1);
		res.negative = 1;
		return res;
	}

	flip = !number_greater(num1, num2);
	if (flip) {
		tmp = num1;
		num1 = num2;
		num2 = tmp;
	}

	for (i = sub = borrow = 0; i < num1.numbers || i < num2.numbers || borrow; i++, sub = 0) {
		if (num2.numbers > i)
			sub = num2.number[i];
		sub += borrow;
		if (num1.numbers > i) {
			num1.number[i] -= sub;
			for (j = 0; num1.number[i] < 0; j++, num1.number[i] += 10);
			number_insert_high(&res, num1.number[i]);
			borrow = j;
		} else {
			number_insert_high(&res, sub % 10);
			borrow = sub / 10;
		}
	}
	
	if (flip)
		res.negative = 1;

	return res;
}


NUMBER number_add(NUMBER num1, NUMBER num2) {
	int carry, add, i;
	NUMBER res = number_new();

	if (num1.negative) {
		num1.negative = 0;
		return number_subtract(num2, num1);
	}

	if (num2.negative) {
		num2.negative = 0;
		return number_subtract(num1, num2);
	}

	for (i = add = carry = 0; i < num1.numbers || i < num2.numbers || carry; i++, add = 0) {
		if (num1.numbers > i)
			add += num1.number[i];
		if (num2.numbers > i)
			add += num2.number[i];
		add += carry;
		carry = 0;
		number_insert_high(&res, add % 10);
		carry = add / 10;
	}

	return res;
}


char *number_to_string(NUMBER *num) {
	int i, j;
	char *str = malloc(num->numbers + 2);

	number_strip_leading_zero(num);
	if (num->negative) {
		*str = '-';
		i = 1;
	} else
		i = 0;
	for (j = 0; j < num->numbers; j++)
		str[num->numbers - j - 1 + i] = num->number[j] + 0x30;
	str[j + i] = 0;
	return str;
}


NUMBER number_from_string(char *str) {
	NUMBER num;
	int i;

	num = number_new();
	if (*str == '-') {
		num.negative = 1;
		str++;
	}

	for (i = 0; str[i]; i++)
		number_insert_low(&num, str[i] - 0x30);
	return num;
}


int main(int argc, char **argv) {
	NUMBER num1, num2, num3;
	char arg1[512], arg2[512];

	fprintf(stderr, "Enter two numbers to add: ");
	fscanf(stdin, "%s %s", arg1, arg2);
	num1 = number_from_string(arg1);
	num2 = number_from_string(arg2);
	num3 = number_add(num1, num2);
	fprintf(stderr, "(%s) + (%s) = %s\n", arg1, arg2, number_to_string(&num3));
	fprintf(stderr, "Enter two numbers to subtract: ");
	fscanf(stdin, "%s %s", arg1, arg2);
	num1 = number_from_string(arg1);
	num2 = number_from_string(arg2);
	num3 = number_subtract(num1, num2);
	fprintf(stderr, "(%s) - (%s) = %s\n", arg1, arg2, number_to_string(&num3));

	return 0;
}

