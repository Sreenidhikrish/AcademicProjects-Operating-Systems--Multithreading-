CC=gcc 
CFLAGS=-Wall -g -pthread

program: compute_perfect_num.o
	$(CC) -o compute_perfect_num compute_perfect_num.o -lm -lpthread 
program.o: compute_perfect_num.c 
	$(CC) $(CFLAGS) -c compute_perfect_num.c              

clean:
	rm -f compute_perfect_num compute_perfect_num.o
run: compute_perfect_num
	./compute_perfect_num
