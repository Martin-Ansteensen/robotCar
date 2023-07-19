# robotCar - Not finished (stalled)
A remote controlled robot car with a meccanum drivetrain.
The inspiration for this project was taken from the many First Tech Challenge (FTC) robots that use a meccanum wheel drivetrain with position tracking. Howerver, the components used in FTC robots are quite expensive, and I wanted to design a low-cost version of these robots,

## Objectives
- [x] Create a meccanum drivetrain that enables the car to drive in all directions.
- [x] Create a remote control for the car
- [ ] Follow a path (predetermined in the program using some sort of a grid), using three omniwheels tracking the movement of the car.


## Design
### Meccanum drivetrain
A meccanum wheel consists of two disks with smaller wheels (rollers) attached in between them. The axis of the rollers are at 45° to the wheel plane and 45° to the axis perpendicular to the disks. A meccanum drivetraing consits of four meccanum wheels, each with it's own DC-motor. By adjusting the speed of each wheel, one can make the drivetrain move in any direction.
### Position tracking (used for odometry)
Three omni wheels (almost like meccanum wheels, but the axis of the rollers are parallel to the wheel plane), each with it's own rotary encoder. To ensure contact with the ground and avoid slipping, the omni wheels were mounted on a small platform with a rubber band forcing them into the ground. By constantly checking the change of the three encoders, the change in the robot's position can be found. 
### Electronics
- Gyro (mpu6050). Expermineted with using motion data from the sensor to caculate changes in the position, but the data had to much jitter.
- Arduino Mega (main brain of car).
- Three Arduino Pro Mini (in an effort to detect all steps from the rotary encoder, each encoder was connected to it's own designated Arduino responsible for counting the steps of the encoder).
- Four DC-motors
- Two L298N Motor Drivers
- Battery pack (used by motors)
- Power bank (used by the other electronics to avoid voltage spikes caused by motors)
- Buck converter (transform voltage from power bank)
- LCD (display the position of the robot)
- Four KY-040 Rotary Encoders
- nrf24l01 (radio module)

## Path following
The interesting aspect of this project was to make it follow a path (a simple path: drive straight to the left for 10cm, 10cm forward, 10cm diagonally, or a more complex one: a circle, any mathematical curve). This is not easily achieveable as the the meccanum wheels used for the drivetrain slips a lot, which removes the possibilty of using encoders on the drivetrain motors to track the position.
By using motion data from the encoders (measuring the rotation of the omni wheels, the robot should be able to know it's position, and correct to follow a given path.
Howver, the encoders I used skipped a lot steps, which caused the motion data to be total rubbish. I wasn't able to solve the issue.
