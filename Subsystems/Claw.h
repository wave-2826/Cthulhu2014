// RobotBuilder Version: 1.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in th future.
#ifndef CLAW_H
#define CLAW_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
/**
 *
 *
 * @author ExampleAuthor
 */
//
// This class controls the claw tha grips the ball while it is in the robot.
//
class Claw: public Subsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
    bool m_clawIsOpen;
    bool m_tuskIsOpen;
    bool m_fangIsOpen;
    bool m_charybdisIsOpen;
public:
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	Solenoid* claw;
	Solenoid* fang;
	Solenoid* tusk;
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	Claw();
	void InitDefaultCommand();
    void openCharybdis();
    void closeCharybdis();
    void openTusk();
    void closeTusk();
    void openClaw();
    void closeClaw();
    void openFang();
    void closeFang();
    void shootPosition();
    bool isClawOpen();
    bool isTuskOpen();
    bool isCharybdisOpen();
    bool isFangOpen();
    
    
};
#endif