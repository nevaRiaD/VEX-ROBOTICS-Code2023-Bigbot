#include "main.h"
#include <string>
#include <vector>
#include <math.h>

//SENSORS
pros::Distance dist(18);

pros::Vision vision(1);


//WHEEL MOTOR PORTS
pros::Motor topRight(13, true);
pros::Motor topLeft(10, false);
pros::Motor botRight(2, false);
pros::Motor botLeft(8, true);

pros::Vision Vision(7);

//WHEEL MOTOR GROUPS
pros::Motor_Group left({10, 17});
pros::Motor_Group right({15, 13});

int distRead;

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void autonomous() {
	pros::vision_signature_s_t sig = pros::Vision::signature_from_utility(1, -6157, -4985, -5571, -5957, -4737, -5347, 7.600, 0);
	vision.set_signature(1, &sig); // Pointer to these values

	while (true) {
		pros::vision_object_s_t obj = vision.get_by_sig(0, 1);

		if (obj.signature == 1) {
			pros::lcd::set_text(1, "Level 10 GYATT!");
		}

		else {
			pros::lcd::set_text(1, "What da hell!");
		}
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() 
{

	// DEFINITIONS

	topLeft.tare_position();
	topRight.tare_position();
	botLeft.tare_position();
	botRight.tare_position();

	int ymotion;
	int xmotion;

	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor left_mtr(1);
	pros::Motor right_mtr(2);

	while (true) {

		pros::lcd::set_text(1, std::to_string(topLeft.get_position()));
		pros::lcd::set_text(2, std::to_string(topRight.get_position()));
		pros::lcd::set_text(3, std::to_string(botLeft.get_position()));
		pros::lcd::set_text(4, std::to_string(botRight.get_position()));

		pros::Controller master(pros::E_CONTROLLER_MASTER);
		// driving control code

		ymotion = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		xmotion = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);

		int right = -xmotion + ymotion; // -power + turn
		int left = xmotion + ymotion; // power + turn

		topLeft.move(left); // Swaps negatives if you want the bot to drive in the other direction
	}
}