/* Name: cmd_Vel-To-Serial_Write
 *
 * Description: This program node receives velocity commands from the teleop_twist_joy ROS node reformats it, then sends to the Serial_Publish ROS node.
 * 
 * Goals:
 *  - Receive the data from joy_teleop_twist and store in variables to pull out x_vel and r_rot (or z_rot)
 *  - Decide what command is most appropriate given the information received
 *  - Translate the x velocity data and rotation data into appropriate PWM signals (0 to 255)
 *  - Package the Command(s) and Values into a "String" of HEX values ie, 
 *  - Send the packaged data to Serial_Publish ROS node
 * 
 * Notes:
 * *  - Don't worry about deadzone or percentages, the ROS joy node handles all that
 *  - x_vel is received as a float in meters/second. Assume + is forwards, - is reverse
 *  - r_rot is received as a float in rads/second. Assume + is cw rotation, - is ccw rotation
 *  - Bit stream: hhhh hhhs cccc cccc vvvv vvvv vvvv vvvv vvvv vvvv vvvv vvvv 
 *      - h = header bit = 001; s = special instruction bit (0 for normal, 1 for special)
 *      - cc = command for Motor X where "00" = disable, "01" = forwards, "10" = reverse, "11" = enable
 *      - vvvv vvvv = PWM value for Motor X, direction does not matter here, that's done on arduino
 *      - No footer bits so it can be combined with other instructions in Serial_Publish, it will be first
 *  - Should we need to access other controller information such as button presses to switch modes, we can, idk how yet. Try to avoid
 *  - Add whatever includes/constants/variables as needed.
 * */

//////////////////////////
///////// SETUP //////////
//////////////////////////

/* Includes*/
#include <iostream>
#include <cstddef>

/* Constants */


/* Global Variables */
double x_vel = twistMsg.linear.x;   //receive linear x velocity data in m/s from teleop_twist_joy
double r_rot = twistMsg.angular.z;  //reveive angular z rotation data in rad/s from teleop_twist_joy

///////////////////////////
////// MAIN PROGRAM //////
//////////////////////////

int main() {

/* Decide Command */


/* Value Adjustments */


/* Packaging */


/* Sending */


/* exit */
return 0;
}

//////////////////////////
/////// FUNCTIONS ///////
//////////////////////////

/* Command Parsing Functions */


/* Value Adjustment Functions */


/* Packaging Functions */


/* Serial Communication Functions */


/* Anything Else */






