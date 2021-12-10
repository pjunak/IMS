OBJS	= main.o
SOURCE	= main.cpp
HEADER	= 
OUT		= simulace
CC	 	= g++
FLAGS	= -g -O2 -c -Wall
LIBS	= -lsimlib -lm 
LFLAGS	=  

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS) $(LIBS)
	rm -f $(OBJS)

main.o: $(SOURCE)
	$(CC) $(FLAGS) $(SOURCE) -std=c++17


clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)