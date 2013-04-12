CFLAGS += -Wall -O3

default:
	$(CC) $(CFLAGS) uppg16.c -ldarnit -o uppg16
	$(CC) $(CFLAGS) uppg19.c common.c -ldarnit -o uppg19
	$(CC) $(CFLAGS) uppg20.c common.c -ldarnit -o uppg20
	$(CC) $(CFLAGS) uppg21.c common.c -ldarnit -o uppg21
	$(CC) $(CFLAGS) uppg23.c -ldarnit -o uppg23

clean:
	rm -f uppg16
	rm -f uppg19
	rm -f uppg20
	rm -f uppg21
	rm -f uppg23
