default:
	gcc driver.c bitboards.c attack.c generate.c position.c make.c square.c -o chess

run:
	./chess

clean:
	rm ./chess