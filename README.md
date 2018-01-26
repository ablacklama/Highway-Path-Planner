# CarND-Path-Planning-Project
[Self-Driving Car Engineer Nanodegree Program](https://www.udacity.com/course/self-driving-car-engineer-nanodegree--nd013)


### Project Goals
The goal of this project is to safely navigate a simulated car around a highway track. This involves safely navigating traffic and maintaining a reasonable speed by changing lanes if cars ahead are too slow. Doing this requires using sensor fusion and localization data to plan a path for the car.

## Implementation
The parts of this code are explained in the order that makes the most sense to a person. Please look at the code if you want to see how it's actually assembled.

### 1. CarData Class
This class handles most of the interpretation of sensor fusion and localization data. It also keeps track of what state our finite state machine is in. It stores cars detected by the sensor fusion data in `Car` objects that keep track of what lane that car is in and where it is predicted to be at the end of current path. This helps with functions like `ClosestInLane` which uses the cars current position to return the closest Car ahead in a given lane. The class also includes a `Safe` function for checking if it's safe to change to a lane, and a `Print` function for debugging.
 
### 2. Finite State Machine
This keeps track of what the car is currently doing. The states are "default", "following", "preparing lane change". 

#### Default state
In this state the car will drive straight in its lane at about 49.5 miles an hour (so as to not pass the 50mph speed limit). In this state it is constantly calling the `ClosestInLane` function to see if a car is within 30 meters ahead. If such a car exists, it transitions to the "follow" state.

#### Follow state
The follow state adjusts the cars speed to match the car ahead at a safe distance. To adjust the speed, we have a maximum speed adjustment that can be made per step (.224). That makes sure our car doesn't abruptly change speed and cause a lot of jerk. We then adjust the speed by varying amounts depending on whether we are close to the speed we want, and whether we are too close to the car we're following. Additionally, if the car we're following is going more than 5mph below the speed limit, we'd like to pass it. So we move into the "prepare lane change" state.

#### Prepare Lane Change State
Now that we've determained we'd like to change to a faster lane we need to figure out if a lane change is even safe. This is done by calling the `Safe` function. Now that we know which lanes are safe we determain which lane is the best by checking which lane has the most free space ahead of us by calling the `ClosestInLane` function. If no lane is found that's safe, we simply continue following and looking for a safe lane. We also won't change lanes if the lane we are currently in is the best lane. If we do find a good lane next to ours, we simply set the desired lane for our path planner to whatever lane that is. If the desired lane is more than one lane away, we just move one lane towards it (assuming that lane is safe). Once we've changed lanes, we set the state back to default.
