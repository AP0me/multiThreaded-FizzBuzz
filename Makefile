all:
	gcc parllFizzBuzz.c -o parllFizzBuzz -fopenmp
	./parllFizzBuzz