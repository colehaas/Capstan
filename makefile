default:
	gcc -Wall driver.c bitboards.c attack.c generate.c position.c make.c square.c perft.c -o chess

run:
	./chess

clean:
	rm ./chess