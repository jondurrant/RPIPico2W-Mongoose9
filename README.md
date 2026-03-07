# RPIPico2W-Mongoose9
A set of examples of Mongoose  On Pico 2 W.  These examples support a number of
episode on my YouTube channel [DrJonEA](https://youtube.com/@drjonea)

## Cloning and Building
The projects in this repo are build for the Pico 2 W, though would also run fine on a Pico W with a config change. They have been compiled and tested usin gthe Pico SDK 2.2.0. 

I have a guide on building project over on my [blog](https://drjonea.co.uk/2025/12/15/building-my-projects-from-repo/).

When cloning the repo please use the "recurse submodules" option as I have submodules to include several libraries.

## Examples

### RT-Graph and RT Graph Extended
Using mongoose to display realtime graphs of a BNO055 IMU. Watching the Roll, Pitch and Yaw of the sensor, plus acceleration on the X, Y and Z axis (in the extended version). This helped me get my head around this IMU very quickly.

### MQTT
Connecting to the Flespi IoT broker over MQTT using the mongoose library. Then using Python and Paho as a test client to send messages to the Pico 2 W and receive the responses back.

### Web File Manager
Managing files stored on the Pico 2 W in a flash based filesystem. Using mongoose to build a Web File Manager to upload and download files, list, rename and delete the files.  

I've used the library [pico_vfs](https://github.com/oyama/pico-vfs) for the file system and POSIX  file function implementation. There are some experiment project in the "exp" folder that helped be build towards this example. 

