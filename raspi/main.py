from cmath import sqrt
from hashlib import new
import numpy

waypoint_queue = [[0, 0]]
heading = [0, 0]
location = [0, 0]
motor_power = [0, 3]  # 0-100% value, then number of motors operating


def get_arduino_data():  # Pull location and motor power
    return


def convert_to_polar(point):  # Cartesian point to polar point
    new_point = [0, 0]
    new_point[0] = sqrt((point[0]*point[0]) + (point[1]*point[1]))
    new_point[1] = numpy.arctan(point[0], point[1])
    return new_point


def turn(angle_delta):  # Will send turn instruction to Arduino
    return


def change_force(force_delta):  # Will send instruction to Arduino to change motor power, scale from 1-100
    return


def distance_calc(point1, point2):  # Calculates distance between two points
    distance = abs(sqrt(pow(abs(point1[0]-point2[0]), 2)+pow(abs(point1[1]-point2[1]), 2)))
    return distance


def main():
    new_heading = [0, 0]
    if distance_calc(location, waypoint_queue[0]) < 0.1:
        waypointQueue.pop(0)
    else:
        new_heading[0] = waypointQueue[0][0] - location[0]
        new_heading[1] = waypointQueue[0][1] - location[1]
        new_heading = convert_to_polar(new_heading)
        turn(new_heading[1]-heading[1])

    print("Location: " + str(location))
    print("Heading: " + str(new_heading))
    print("Motor power: " + str(motor_power[0]) + "%")
    print(str(motor_power[1]) + " motors active")
