// RobotBuilder Version: 1.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in th future.
#include "SetSpecificAngle.h"
#include "../WaveConstants.h"
#include <math.h>
namespace
{
    const double c_timoutSeconds = 1.5; //PID stops after this time:give jess candy please
    //Arm Pitch Parameters
    const double c_setpointPitchTolerance = 0.02; // volts
}
SetSpecificAngle::SetSpecificAngle() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::armPitchWithPID);
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}
SetSpecificAngle::SetSpecificAngle(double angle) {
    Requires(Robot::armPitchWithPID);
    m_setAngle = angle;
}
// Called just before this Command runs the first time
void SetSpecificAngle::Initialize() {
    Robot::armPitchWithPID->setAbsoluteAngle(m_setAngle);
    SetTimeout(c_timoutSeconds);
	
}
// Called repeatedly when this Command is scheduled to run
void SetSpecificAngle::Execute() {
//    std::cout << "  Arm PID: Actual Volts: " << Robot::armPitchWithPID->pitchAngleSensor->GetAverageVoltage() 
//            << ", Set Volts:" << Robot::armPitchWithPID->GetSetpoint() << ", Output: " << Robot::armPitchWithPID->pitchMotor->Get() << std::endl;
}
// Make this return true when this Command no longer needs to run execute()
bool SetSpecificAngle::IsFinished() {
	double c_setpointPitchTolerance = 0.02;
	bool withinArmError = (fabs(Robot::armPitchWithPID->ReturnPIDInput() - Robot::armPitchWithPID->convertAngleToVolts(m_setAngle, false)) < c_setpointPitchTolerance);
    if(withinArmError || IsTimedOut())
    {
        return true;
    }
    else
    {
        return false;
    }
}
// Called once after isFinished returns true
void SetSpecificAngle::End() {
    //Stop trying to set the position once we've reached it.
    Robot::armPitchWithPID->Disable();
}
// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void SetSpecificAngle::Interrupted() {
    End();
}
