build:
	gcc -Wall -std=c99 ./src/*.c \
		-o game \
	  -lSDL2 \
		-lSDL2_ttf \
		-L/opt/homebrew/lib \
	  -I/opt/homebrew/include -D_THREAD_SAFE 

run:
	./game

clean:
	rm game

leak:
	clang -fsanitize=address -Wall -std=c99 -g ./src/*.c \
		-o game \
	  -lSDL2 \
		-lSDL2_ttf \
		-L/opt/homebrew/lib \
	  -I/opt/homebrew/include -D_THREAD_SAFE 


nothing:
