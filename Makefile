hw6: main.o sieve.o
	gcc -g  main.o sieve.o -o hw6 -lm

main.o: main.c functions.h
	gcc -c -g -Wall main.c  

sieve.o: sieve.c functions.h
	gcc -c -g -Wall sieve.c  

clean:
	rm *.o
	rm hw6
