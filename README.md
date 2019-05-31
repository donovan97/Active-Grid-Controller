An active grid is a device used to generate high levels of turbulence in a wind tunnel. For the active grid in the Aerodynamics Laboratory at McGill University, this code, ran on an Arduino Uno, serves as the master of an i2c network communicaiting with several different Arduinno Nano slaves, each controlling a servo motor for the active grid. This code provides an interface on an LCD screen and three buttons to change the parameters, namely the minimum length, maximum length, minimum speed and maximum speed of rotation. When the parameters are changed by the user, the Arduino sends the updated parameters to the slaves in the network, thereby changing the operation parameters of the active grid. 
