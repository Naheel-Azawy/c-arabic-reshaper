CC=gcc

arabic-reshape: main.o arabic-reshaper.o
	$(CC) main.o arabic-reshaper.o -o arabic-reshape

main.o: main.c
	$(CC) -c main.c

arabic-reshaper.o: arabic-reshaper.c arabic-reshaper.h arabic-glyphies.h
	$(CC) -c arabic-reshaper.c

clean:
	rm -f *.o arabic-reshape

install: arabic-reshape
	cp arabic-reshape /usr/bin/arabic-reshape

uninstall:
	rm /usr/bin/arabic-reshape

test:
	./test.sh
