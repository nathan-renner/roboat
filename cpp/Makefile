objects = *.o
binaries = navloop queue

all: navloop queue

navloop: main.o
	g++ main.o -o navloop -lwiringPi -lwiringPiDev -lpthread -lm

queue: writeWaypointQueue.o
	g++ writeWaypointQueue.o -o queue

main.o:
	g++ -c src/main.cpp -lwiringPi -lwiringPiDev -lpthread -lm

writeWaypointQueue.o:
	g++ -c src/writeWaypointQueue.cpp

clean_objects:
	rm -f $(objects)

clean_all:
	rm -f $(binaries) $(objects)