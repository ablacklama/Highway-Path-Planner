# CarND-Path-Planning-Project
[Self-Driving Car Engineer Nanodegree Program](https://www.udacity.com/course/self-driving-car-engineer-nanodegree--nd013)


### Project Goals
The goal of this project is to safely navigate a simulated car around a highway track. This involves safely navigating traffic and maintaining a reasonable speed by changing lanes if cars ahead are too slow. Doing this requires using sensor fusion and localization data to plan a path for the car.

## Implementation

### 1. CarData Class
This class handles most of the interpretation of sensor fusion and localization data. It also keeps track of what state our finite state machine is in. It stores cars detected by the sensor fusion data in Car objects that keep track of what lane that car is in and where it is predicted to be at the end of current path. This helps with functions like "ClosestInLane" which uses the cars current position to return the closest Car ahead in a given lane. The class also includes a "Safe" function for checking if it's safe to change to a lane, and a "Print" function for debugging.
 
