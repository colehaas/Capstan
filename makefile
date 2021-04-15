default:
	gcc -Wall *.c -o chess

run:
	./chess

clean:
	rm ./chess 