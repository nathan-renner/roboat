objects = *.o
binaries = navloop queue testMotors testGPS

all: navloop queue testMotors testGPS

navloop: main.o
	g++ main.o -o navloop

queue: writeWaypointQueue.o
	g++ writeWaypointQueue.o -o queue

testMotors: testMotors.o
	g++ testMotors.o -o testMotors

testGPS: testGPS.o
	g++ testGPS.o -o testGPS

main.o:
	g++ -c src/main.cpp -lwiringPi -lwiringPiDev -lpthread -lm

testGPS.o:
	g++ -c src/testGPS.cpp -lwiringPi -lwiringPiDev -lpthread -lm

testMotors.o:
	g++ -c src/testMotors.cpp -lwiringPi -lwiringPiDev -lpthread -lm

writeWaypointQueue.o:
	g++ -c src/writeWaypointQueue.cpp

clean_objects:
	rm -f $(objects)

clean_all:
	rm -f $(binaries) $(objects)