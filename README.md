
# Virtual Drums
A repository containing the arduino codes and the python code used to make a six piece drum set (excluding the toms).
FL Studio was the music software used to play the sounds.

# Mechanical and Electronic Part
Both the drum sticks were fitted with a 6-DOF IMU sensor(MPU 6050) and a magnetometer each. The magnetometer readings were interpreted to get the horizontal position of the drum sticks ( to figure out which piece of the drum set is beeing played). The IMU readings were interpreted to detect any jerk of the drum stick(to figure out if a drum piece has been hit).

# Source Programs
* notomleft.ino : Arduino code for the left drum stick.
* notomright.ino : Arduino code for the right drum stick.
* twodrums.py : Python code which acts as interface between the arduino and FL Studio.

# Dependencies
* Arduino IDE 1.6
* Python (version 3)
* FL Studio 12


