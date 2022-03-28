from cmath import sqrt
from hashlib import new
import numpy

waypointQueue = {{0, 0}};
heading = {0, 0};
location = {0, 0};

def convertToPolar(point): #Cartesian point to polar point
    newPoint = {}
    newPoint[0] = sqrt((point[0]*point[0]) + (point[1]*point[1]))
    newPoint[1] = numpy.arctan(point[0], point[1])
    return newPoint

def turn(angleDelta): #Will send turn instruction to Arduino
    return

def changeForce(forceDelta): #Will send instruction to Arduino to change motor power
    return

def distanceCalc(pointA, pointB): #Calculates distance between two points
    distance = sqrt(pow(abs(pointA[0]-pointB[0]), 2)+pow(abs(pointA[1]-pointB[1])))
    return distance

def main():
    if (distanceCalc(location, waypointQueue[0]) < 0.1) :
        waypointQueue.pop(0)
    else :
        newHeading = {};
        newHeading[0] = waypointQueue[0][0] - location[0]
        newHeading[1] = waypointQueue[0][1] - location[1]
        newHeading = convertToPolar(newHeading)
        turn(newHeading[1]-heading[1])
    
    