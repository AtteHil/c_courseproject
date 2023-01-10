
#Harjoitustyö Perustaso, Makefile.
HT: HTPerustaso.o HTPerusKirjasto.o
	gcc HTPerustaso.o HTPerusKirjasto.o -o HT -Wall -std=c99 -pedantic

HTPerustaso.o: HTPerustaso.c
	gcc -c HTPerustaso.c -Wall -std=c99 -pedantic

HTPerusKirjasto.o: HTPerusKirjasto.c HTPerusKirjasto.h
	gcc -c HTPerusKirjasto.c -Wall -std=c99 -pedantic

clean: 
	rm *.o HT