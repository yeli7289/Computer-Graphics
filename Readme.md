# Racing Game

## Author
Yen-Teh Liu

## Usage
cd to the file and ‘make’ makefile.

## Key
w: accelerate

s: de-accelerate

a: left turn

d: right turn

mode 1: third person mode

mode 2: first person mode

'-': mode-=1

'+': mode+=1 

## Features

1. Collision Detection between Cars:
The cars are surrounding by the boxes and the collision function is implemented when the boxes are collided.

2. Collision Effect between cars:
Once the collision is detected, we have to further detect which car bump into which car, the one bump into the others will not able to keep moving toward its direction and will also lose speed.

3. Collision Detection with Track Boundary:
The program will detect when your car bump into the boundary. For the curve, I store all the outside polygon points in to a struct for collision detection process.

4. Collision Effect between car and boundary:
Once you have cross the boundary, your speed will be slower down.

5. Track Generator:
The track generator generate the straight and curve line and mixed together to be a complex racing track.

4. localization:
Each piece of Straight or Curve trace have an id, and I keep track of the cars location by storing how many laps the cars have run and which trace the cars are in. 

5. Computer controlled car (AI car):
This car follows certain path among the track, its path is stored in a linked-list while we generating the track for the first time. The AI car will follow certain strategy to race, if it is in front of the player, it will slow down its speed and vice versa. That's the reason why I have to know both the cars’ location.

