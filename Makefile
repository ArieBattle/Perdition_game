CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk2

walk2: walk2.cpp log.cpp
	g++ $(CFLAGS) walk2.cpp log.cpp arielleB.cpp franciscoU.cpp theodoreB.cpp anahiV.cpp libggfonts.a -Wall -Wextra \
	/usr/lib/x86_64-linux-gnu/libopenal.so \
	/usr/lib/x86_64-linux-gnu/libalut.so \
	$(LFLAGS) -owalk2

clean:
	rm -f walk2
	rm -f *.o

