## 🤖 Test Robot Repository 🤖

The following is a short description of each folder & file you'll find at the top folder of this repository:

<hr>

* `/Arduino Code` - contains Arduino "Sketches" that encapsulate code.
    1. `Encoder_Read_Test`: Reads Encoder Data
    2. `Motor_Control_Test`: Tests Individual Motor Controls
    3. `Moving_All_Motors`: Comprehensive attempt to control all 4 motors w/ encoders.
	4. `Joystick_Controlled_Acceleration`: Skeleton file, relating to joystick (controller) inputs.

<hr>

* `/config` - an RViz config file, RViz is a 3D visualization tool for ROS (Robot Operating System) that allows us to visualize the components of the robot and its environment. To be used with RViz like `rviz2 -d /path/to/view_bot.rviz`

<hr>

* `/description` - contains XACRO (like XML) files that describe properties of the robot, environment, structured around the main `robot.urdf.xacro` file.

<hr>

* `/encoderExample` - an example of how to use an encoder with an Arduino to measure the rotation and distance traveled by a wheel.

<hr>

* `/launch` - contains python files for setup & launch of the ROS2 robot simulation environment. `launch_sim` and `rsp` (Robot State Publisher).

<hr>

* `/worlds` - contains world description files for Gazebo simulations, assisting in the developer's robot simulation setup.

<hr>

* `CMakeLists.txt` - this is the project's main CMake file for generating C++ builds, it generates large makefiles which are necessary for running compiled C++ code.

<hr> 

* `LICENSE.md` - contains a free Apache license, but this repository is considered private.
* `package.xml` - defines the package metadata for ROS.
* `packages.txt` - **lists the names of packages that need to be installed via** `apt` (Ubuntu/Other OS).
* `README.md` - this file you're reading!
* `SETUP.md` - a quick start guide & message to new devs/users.

### Read `SETUP.md` for a helpful guide to getting started!