#include "Robot.h"
#include "Commands/FindTarget.h"
#include "Commands/DriveByJoystick.h"
#include "Commands/WatchForBall.h"
#include "Commands/Fire.h"
#include "Commands/Truss.h"
#include "Commands/TurretLoadLeft.h"
#include "Commands/TurretLoadRight.h"
#include "WaveConstants.h"
#include "Commands/TurretByTurntable.h"
#include "Commands/ArmByTurntable.h"
#include <math.h>
//static AxisCamera &s_camera = AxisCamera::GetInstance("192.168.0.90");
//static AxisCamera &s_camera = AxisCamera::GetInstance("10.28.26.11");
static FindTarget* target;
namespace
{
const double p_teleopLoopTime = 0.03;
const double p_autoLoopTime = 0.01;
const double p_disabledLoopTime = 0.05;
}
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INITIALIZATION
ArmPitchWithPID* Robot::armPitchWithPID = 0;
Claw* Robot::claw = 0;
BallCollector* Robot::ballCollector = 0;
CompressorSubSystem* Robot::compressorSubSystem = 0;
DashboardUpdater* Robot::dashboardUpdater = 0;
DriveWithPID* Robot::driveWithPID = 0;
Shooter* Robot::shooter = 0;
TurretWithPID* Robot::turretWithPID = 0;
OI* Robot::oi = 0;
Lights* Robot::lights = 0;
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INITIALIZATION
void Robot::RobotInit() {
	RobotMap::init();
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
	armPitchWithPID = new ArmPitchWithPID();
	claw = new Claw();
	ballCollector = new BallCollector();
	compressorSubSystem = new CompressorSubSystem();
	dashboardUpdater = new DashboardUpdater();
	driveWithPID = new DriveWithPID();
	shooter = new Shooter();
	turretWithPID = new TurretWithPID();
	oi = new OI();
	lights = new Lights();
	lw = LiveWindow::GetInstance();
	// instantiate the command used for the autonomous period
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=AUTONOMOUS
	autonomousCommand = new Autonomous_Command();
	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=AUTONOMOUS
	std::cout << "RobotInit" << std::endl;

	m_lastShift = 0;
	m_lastFire = 0;
	m_lastDpadLeft = 0;
	m_lastDpadRight = 0;
	m_lastTruss = 0;
	m_commandFire = new Fire();
	//m_commandTruss = new Truss();
	m_commandLoadLeft = new TurretLoadLeft();
	m_commandLoadRight = new TurretLoadRight();
}
void Robot::AutonomousInit() {
	Robot::lights->SetLightsAutoMode();
	ballCollector->setSensorPower(true);
	//	if (autonomousCommand != NULL)
	//		autonomousCommand->Start();
	if(!compressorSubSystem->isEnabled())
	{
		compressorSubSystem->startCompressor();
	}
	std::cout << "AutonomousInit" << std::endl;
	autonomousCommand->Start();
}


void Robot::AutonomousPeriodic() {
	Scheduler::GetInstance()->Run();

	DriverStationLCD *HUD = DriverStationLCD::GetInstance();
	m_1secLoopCounter++;
	if(((double)(m_1secLoopCounter) * p_teleopLoopTime) >= 1.0) //update every second
	{
		HUD->PrintfLine(DriverStationLCD::kUser_Line1, "Tank PSI: %f", compressorSubSystem->getTankPSI());
		m_1secLoopCounter = 0;
	}
	HUD->UpdateLCD();

	float desiredAngle = Robot::armPitchWithPID->convertVoltsToAngle(Robot::armPitchWithPID->GetSetpoint(), false);
	std::cout << "Arm Angle: " << Robot::armPitchWithPID->getCurrentAngle() << "Desired Angle: " << desiredAngle << std::endl;
	//std::cout << "Arm Angle: " << armPitchWithPID->getCurrentAngle() << std::endl;
	Wait(p_autoLoopTime);
	//std::cout << "AutonomousPeriodic" << std::endl;
}


void Robot::TeleopInit() {
	DriverStationLCD *HUD = DriverStationLCD::GetInstance();
	HUD->Clear();
	autonomousCommand->Cancel();
	//target = new FindTarget();
	//target->Initialize();
	if(!compressorSubSystem->isEnabled())
	{
		compressorSubSystem->startCompressor();
	}
	ballCollector->setSensorPower(true);
	shooter->SetWinchPin(c_setPin);

	Scheduler::GetInstance()->AddCommand(new DriveByJoystick());
	//Scheduler::GetInstance()->AddCommand(new TurretByTurntable()); //change TurretByTurntable to TurretByJoystick
	//Scheduler::GetInstance()->AddCommand(new ArmByTurntable());
}


void Robot::TeleopPeriodic() {
	DriverStationLCD *HUD = DriverStationLCD::GetInstance();
	Scheduler::GetInstance()->Run();
	m_1secLoopCounter++;
	if(((double)(m_1secLoopCounter) * p_teleopLoopTime) >= 1.0) //update every second
	{
		HUD->PrintfLine(DriverStationLCD::kUser_Line1, "Tank PSI: %f", compressorSubSystem->getTankPSI());
		m_1secLoopCounter = 0;
	}
	HUD->UpdateLCD();

	//	//Driver Right Trigger - Shift
	//	float shift = oi->getDriverJoystick()->GetRawAxis(c_triggerAxis);
	//	if((shift < -0.5) && (!m_lastShift))
	//	{
	//		driveWithPID->shiftGear((driveWithPID->currentlyInLowGear()? false : true));
	//	}
	//	m_lastShift = ((shift < -0.5) ? 1 : 0);
	//	//Driver Left Trigger - DropDown
	//	float dropDown = oi->getDriverJoystick()->GetRawAxis(c_triggerAxis);
	//	if((dropDown > 0.5) && (!m_lastDropDown))
	//	{
	//		driveWithPID->toggleDropDown((driveWithPID->currentlyDroppedDown()? false : true));
	//	}
	//	m_lastDropDown = ((dropDown > 0.5) ? 1 : 0);

	if(Robot::lights->GetPermissible() == true)
	{
		bool shooterBack = Robot::shooter->brandingIronSensor->Get();
		double currentArmAngle = Robot::armPitchWithPID->getCurrentAngle();
		bool withinArmError = (fabs(Robot::armPitchWithPID->ReturnPIDInput() - Robot::armPitchWithPID->convertAngleToVolts(currentArmAngle, false)) < .02);

		if(shooterBack == false && withinArmError == false)
		{
			Robot::lights->SetLightsNoArmNoShoot();
		}
		else if(shooterBack == true && withinArmError == false)
		{
			Robot::lights->SetLightsNoArm();
		}
		else if(shooterBack == true && withinArmError == true)
		{
			Robot::lights->SetLightsReadyForFire();
		}
		else // shooter not back and arm in position
		{
			Robot::lights->SetLightsDisable();
		}
	}

	if(oi->getDriverJoystick()->GetRawAxis(c_triggerAxis) < -.5)
	{
		driveWithPID->shiftGear(true);
	}
	else
	{
		driveWithPID->shiftGear(false);
	}

	if(oi->getDriverJoystick()->GetRawAxis(c_triggerAxis) > .5)
	{
		driveWithPID->toggleDropDown(false);
	}
	else
	{
		driveWithPID->toggleDropDown(true);
	}



	//Operator D-pad - Left
	float dpad = oi->getOperatorJoystick()->GetRawAxis(6);
	//    std::cout << "Dpad value: " << dpad << ", m_lastDpadLeft: " << m_lastDpadLeft << ", m_lastDpadRight: " << m_lastDpadRight << std::endl;
	if((dpad < -0.5) && !m_lastDpadLeft)
	{
		m_commandLoadLeft->Start();
	}
	m_lastDpadLeft = ((dpad < -0.5) ? 1 : 0);
	//Operator D-pad - Right
	if((dpad > 0.5) && !m_lastDpadRight)
	{
		m_commandLoadRight->Start();
	}
	m_lastDpadRight = ((dpad > 0.5) ? 1 : 0);

	//Operator Right Trigger - Fire
	float fire = oi->getOperatorJoystick()->GetRawAxis(c_triggerAxis);
	//    std::cout << "Fire:\fire=" << fire << "\t m_lastShift=" << m_lastFire << "\t ok? " << ((fire < -0.5) && (!m_lastFire)) << std::endl;
	if((fire < -0.5) && (!m_lastFire))
	{
		m_commandFire->Start();
	}
	m_lastFire = ((fire < -0.5) ? 1 : 0);

	//Robot::lights->DebugLights();
	//std::cout << "Robot Turret Volts: " << Robot::turretWithPID->turretAngleSensor->GetAverageVoltage() << "\tRobot Turret Angle" << Robot::turretWithPID->getCurrentAngle() << std::endl;
		
	//std::cout << "TeleopPeriodic" << std::endl;
	float desiredAngle = Robot::armPitchWithPID->convertVoltsToAngle(Robot::armPitchWithPID->GetSetpoint(), false);
	//std::cout << "Arm Angle: " << Robot::armPitchWithPID->getCurrentAngle() << "Desired Angle: " << desiredAngle << std::endl;
	Wait(p_teleopLoopTime);
}


void Robot::TestPeriodic() {
	lw->Run();
	std::cout << "TestPeriodic" << std::endl;
	Wait(.01);
}
void Robot::DisabledInit()
{
	Robot::lights->SetLightsDisable();
}

void Robot::DisabledPeriodic()
{
	autonomousCommand->Cancel();
	compressorSubSystem->stopCompressor();
	ballCollector->setSensorPower(false);
	DriverStationLCD *HUD = DriverStationLCD::GetInstance();
	m_1secLoopCounter++;
	if(((double)(m_1secLoopCounter) * p_teleopLoopTime) >= 1.0) //update every second
	{
		HUD->PrintfLine(DriverStationLCD::kUser_Line1, "Tank PSI: %f", compressorSubSystem->getTankPSI());
		m_1secLoopCounter = 0;
	}

	HUD->PrintfLine(DriverStationLCD::kUser_Line2, "AutoMode: %d", oi->getAutoMode());
	HUD->UpdateLCD();

	//std::cout << "DisabledPeriodic" << std::endl;
	if(target != NULL)
	{
		delete target;
		target = NULL;
	}
	//Robot::oi->debugArduino();
	Wait(p_disabledLoopTime);
}

START_ROBOT_CLASS(Robot);