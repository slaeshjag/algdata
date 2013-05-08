/* uppg9.c - Steven Arnow <s@rdw.se>,  2013 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>


#define	ZeroMemory(ptr, size)		(memset((ptr), 0, size))
#define	HASH_TABLE_SIZE			64


typedef struct {
	int				*index;
	unsigned int			entries;
} HASH_TABLE;

typedef struct {
	const char			*name;
	const char			**prop;
	int				props;
} BOOK_INFO;


unsigned int str_hash(const char *str, int range) {
	unsigned int hash, i;
	const unsigned char *stra = (void *) str;

	hash = 0;
	for (i = 0; str[i]; hash += stra[i] * (i + stra[i] * 4 + 1), i++);
	hash ^= 0xAAAAAAAA;
	hash %= range;

	return hash;
}


HASH_TABLE *hash_table_new(int size) {
	HASH_TABLE *hash;

	hash = malloc(sizeof(HASH_TABLE) * size);
	ZeroMemory(hash, sizeof(HASH_TABLE) * size);

	return hash;
}


void hash_table_add(HASH_TABLE *hash, int hash_val, int index) {
	hash[hash_val].index = realloc(hash[hash_val].index, sizeof(int) * (hash[hash_val].entries + 1));
	hash[hash_val].index[hash[hash_val].entries++] = index;
	return;
}


BOOK_INFO *hash_lookup(BOOK_INFO *book, HASH_TABLE *hash, char *compare) {
	int i;
	unsigned int hash_d;

	hash_d = str_hash(compare, HASH_TABLE_SIZE);

	for (i = 0; i < hash[hash_d].entries; i++)
		if (!strcmp(compare, book[hash[hash_d].index[i]].name))
			return &book[hash[hash_d].index[i]];
	return NULL;
}


int start_listen(int port) {
	struct sockaddr_in addr;
	struct hostent *hp;
	int sock;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;
	
	hp = gethostbyname("127.0.0.1");
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = 0;

	if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		fprintf(stderr, "Errno: %s\n", strerror(errno));
		close(sock);
		return -1;
	}

	listen(sock, 2);

	return sock;
}


BOOK_INFO *book_read(const char *book_file, int *books_p, HASH_TABLE *hash) {
	BOOK_INFO *book;
	int books, i;
	char buff[512];
	FILE *fp;

	if (!(fp = fopen(book_file, "r"))) {
		fprintf(stderr, "unable to open %s\n", book_file);
		return NULL;
	}

	fscanf(fp, "%i\n", &books);

	book = calloc(books, sizeof(BOOK_INFO));

	for (i = 0; i < books; i++) {
		loop:
		fgets(buff, 512, fp);
		if (*buff == '\n')
			goto loop;
		if (strchr(buff, '\n'))
			*strchr(buff, '\n') = 0;
		book[i].name = strdup(buff);
		hash_table_add(hash, str_hash(buff, HASH_TABLE_SIZE), i);
		
		while (!feof(fp)) {
			fgets(buff, 512, fp);
			if (*buff == '\n')
				break;
			if (strchr(buff, '\n'))
				*strchr(buff, '\n') = 0;
			book[i].prop = realloc(book[i].prop, ++book[i].props);
			book[i].prop[book[i].props - 1] = strdup(buff);
		}
	}

	*books_p = books;

	return book;
}


int sock_getline(int sock, char *buff) {
	for (;; buff++ ) {
		if (recv(sock, buff, 1, 0) < 0) {
			*buff = 0;
			return -1;
		}

		if (*buff == '\n') {
			*buff = 0;
			return 0;
		}

		if (*buff == '\r')
			*buff = 0;
	}

	return 0;
}
		

void print_book_info(HASH_TABLE *hash, BOOK_INFO *book, char *name, int sock) {
	BOOK_INFO *book_e;
	int i;

	if (!(book_e = hash_lookup(book, hash, name))) {
		send(sock, "No such book\n", strlen("No such book\n"), 0);
		return;
	}

	send(sock, book_e->name, strlen(book_e->name), 0);
	send(sock, "\n", 1, 0);
	
	for (i = 0; i < book_e->props; i++) {
		send(sock, book_e->prop[i], strlen(book_e->prop[i]), 0);
		send(sock, "\n", 1, 0);
	}

	return;
}


int main(int argc, char **argv) {
	int socket, client, socklen, books, i;
	struct sockaddr addr;
	BOOK_INFO *book;
	HASH_TABLE *hash;
	char buff[512];
	
	if ((socket = start_listen(atoi(argv[2]))) < 0) {
		fprintf(stderr, "Unable to listen on port 5555\n");
		return -1;
	}

	fprintf(stderr, "Reading %s... ", argv[1]);
	hash = hash_table_new(HASH_TABLE_SIZE);
	book = book_read(argv[1], &books, hash);
	fprintf(stderr, "read %i books\n", books);

	while ((client = accept(socket, &addr, &socklen))) {
		
		for (;;) {
			send(client, "> \n", 2, 0);
			if (sock_getline(client, buff) < 0) {
				close(client);
				break;
			}

			if (!strcmp(buff, "list")) {
				for (i = 0; i < books; i++) {
					send(client, book[i].name, strlen(book[i].name), 0);
					send(client, "\n", 1, 0);
				}
			}

			if (strstr(buff, "show") == buff) 
				print_book_info(hash, book, &buff[5], client);
		}
	}

	return 0;
}
