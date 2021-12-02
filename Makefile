OBJS	= main.o
SOURCE	= main.cpp
HEADER	= 
OUT		= ims
CC	 	= g++
FLAGS	= -g -O2 -c -Wall
LIBS	= -lsimlib -lm 
LFLAGS	=  

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS) $(LIBS)
	rm -f $(OBJS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp -std=c++17


clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)