// RobotBuilder Version: 1.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in th future.
#include "WatchForBall.h"
#include "WaveWait.h"
WatchForBall::WatchForBall() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::claw);
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::ballCollector);
}
// Called just before this Command runs the first time
void WatchForBall::Initialize() {
    m_alreadyHaveBall = false;
    Robot::ballCollector->setRollerOutput(1);
}
// Called repeatedly when this Command is scheduled to run
void WatchForBall::Execute() {
   
    bool ballIsThere = Robot::ballCollector->isCatchBallSensorTripped();
    
	if(ballIsThere && !m_alreadyHaveBall){
		m_alreadyHaveBall = true;
		WaveWait(.18);
		Robot::claw->closeCharybdis();
		Robot::ballCollector->setRollerOutput(0);
	}
	
	if(!ballIsThere){
//	    std::cout << "No ball there" << std::endl;
	    m_alreadyHaveBall = false;
	    Robot::ballCollector->setRollerOutput(1);
	}
	
//	std::cout << "{Watching For Ball: " << m_alreadyHaveBall << "}" << std::endl;
}
// Make this return true when this Command no longer needs to run execute()
bool WatchForBall::IsFinished() {
	return m_alreadyHaveBall;
}
// Called once after isFinished returns true
void WatchForBall::End() {
    Robot::ballCollector->setRollerOutput(0);
//    std::cout << "Watching is finished..." << std::endl;
}
// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void WatchForBall::Interrupted() {
//    std::cout << "Watching has been interrupted" << std::endl;
	End();
}