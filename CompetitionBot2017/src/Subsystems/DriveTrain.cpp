#include "DriveTrain.h"
#include "../RobotMap.h"

DriveTrain::DriveTrain() : Subsystem("DriveTrain") {
}

void DriveTrain::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	// SetDefaultCommand(new MySpecialCommand());
}

void DriveTrain::Initialize() {
	if (!definedYet) {
		driveBase = new frc::RobotDrive(DRIVE_MOTOR_LF, DRIVE_MOTOR_LR, DRIVE_MOTOR_RF, DRIVE_MOTOR_RR);
		//driveBase->SetSafetyEnabled(false);
		leftEncoder = new frc::Encoder(DRIVE_ENCODER_LA, DRIVE_ENCODER_LB);
		rightEncoder = new frc::Encoder(DRIVE_ENCODER_RA, DRIVE_ENCODER_RB);
		gyro = new frc::ADXRS450_Gyro();
		leftEncoder->SetDistancePerPulse(6*pi);
		rightEncoder->SetDistancePerPulse(6*pi);
		leftEncoder->SetReverseDirection(true);
		leftEncoder->SetSamplesToAverage(7);
		rightEncoder->SetSamplesToAverage(7);

		definedYet = true;
	}
}

void DriveTrain::Drive(double acceleration, double steering) {

	//Reverse controls if true
	if (controlsSwapped) {
		acceleration = -acceleration;
	}
	//Update the dashboard variables
	frc::SmartDashboard::PutNumber("Acc", acceleration);
	frc::SmartDashboard::PutNumber("Steer", steering);
	driveBase->ArcadeDrive(acceleration, steering);
	frc::SmartDashboard::PutNumber("Left Encoder (Raw)", ((double)leftEncoder->GetRaw()/1440)*6*pi);
	frc::SmartDashboard::PutNumber("Right Encoder (Raw)", ((double)rightEncoder->GetRaw()/1440)*6*pi);
	double speed = (((double)leftEncoder->GetRate()/1440)*6*pi+((double)rightEncoder->GetRate()/1440)*6*pi)/2;
	double speedKph = speed*3600*2.54/100000;
	frc::SmartDashboard::PutNumber("Speed (in-s)", speed);
	frc::SmartDashboard::PutNumber("Speed (Km-s)", speedKph);
	frc::SmartDashboard::PutNumber("Gyro (Radians)", gyro->GetAngle());
	frc::SmartDashboard::PutBoolean("Controls Swapped?", controlsSwapped);
}


void DriveTrain::ManualDrive(double left, double right) {
	driveBase->SetLeftRightMotorOutputs(left, right);
	frc::SmartDashboard::PutNumber("Left Encoder (Raw)", ((double)leftEncoder->GetRaw()/1440)*6*pi);
	frc::SmartDashboard::PutNumber("Right Encoder (Raw)", ((double)rightEncoder->GetRaw()/1440)*6*pi);
	double speed = (((double)leftEncoder->GetRate()/1440)*6*pi+((double)rightEncoder->GetRate()/1440)*6*pi)/2;
	double speedKph = speed*3600*2.54/100000;
	frc::SmartDashboard::PutNumber("Speed (in-s)", speed);
	frc::SmartDashboard::PutNumber("Speed (Km-s)", speedKph);
	frc::SmartDashboard::PutNumber("Gyro (Radians)", gyro->GetAngle());
}


//This inverts controls and updates the dashboard with the new conditions
void DriveTrain::SwapControls() {
	controlsSwapped = !controlsSwapped;
}

//Return encoder average (Aproximate distance)
double DriveTrain::GetEncoderAverageDistance() {
	//return (((double)leftEncoder->GetRaw()/1440)*6*pi+((double)rightEncoder->GetRaw()/1440)*6*pi)/2;
	return ((double)leftEncoder->GetRaw()/1440)*6*pi;
}
//Return current angle
double DriveTrain::GetGyroAngle() {
	return gyro->GetAngle();
}
//Reset both bools
void DriveTrain::Reset() {
	controlsSwapped = false;
	isInUse = false;
}
