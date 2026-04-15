build:
	gcc -Wall -std=c99 ./src/*.c \
		-o game \
	  -lSDL2 \
		-L/opt/homebrew/lib \
	  -I/opt/homebrew/include -D_THREAD_SAFE 

run:
	./game

clean:
	rm game

nothing:
