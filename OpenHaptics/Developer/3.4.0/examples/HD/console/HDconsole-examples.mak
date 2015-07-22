# Makefile - ConsoleExamples

#export DEBUG

ifndef examples_dir
export examples_dir = /usr/share/3DTouch/examples
endif

.PHONY: all
all: \
	AnchoredSpringForce \
	Calibration \
	ErrorHandling \
	FrictionlessPlane \
	FrictionlessSphere \
	HelloHapticDevice \
	PreventWarmMotors \
	QueryDevice \
	ServoLoopDutyCycle \
	ServoLoopRate \
	Vibration \
	CommandMotorDAC \
	CommandJointTorque \
#	Viscosity

.PHONY: AnchoredSpringForce
AnchoredSpringForce:
	$(MAKE) -C AnchoredSpringForce -f AnchoredSpringForce.mak

.PHONY: Calibration
Calibration:
	$(MAKE) -C Calibration -f Calibration.mak

.PHONY: CommandMotorDAC
CommandMotorDAC:
	$(MAKE) -C CommandMotorDAC -f CommandMotorDAC.mak

.PHONY: CommandJointTorque
CommandMotorDAC:
	$(MAKE) -C CommandJointTorque -f CommandJointTorque.mak

.PHONY: ErrorHandling
ErrorHandling:
	$(MAKE) -C ErrorHandling -f ErrorHandling.mak

.PHONY: FrictionlessPlane
FrictionlessPlane:
	$(MAKE) -C FrictionlessPlane -f FrictionlessPlane.mak

.PHONY: FrictionlessSphere
FrictionlessSphere:
	$(MAKE) -C FrictionlessSphere -f FrictionlessSphere.mak

.PHONY: HelloHapticDevice
HelloHapticDevice:
	$(MAKE) -C HelloHapticDevice -f HelloHapticDevice.mak

.PHONY: PreventWarmMotors
PreventWarmMotors:
	$(MAKE) -C PreventWarmMotors -f PreventWarmMotors.mak

.PHONY: QueryDevice
QueryDevice:
	$(MAKE) -C QueryDevice -f QueryDevice.mak

.PHONY: ServoLoopDutyCycle
ServoLoopDutyCycle:
	$(MAKE) -C ServoLoopDutyCycle -f ServoLoopDutyCycle.mak

.PHONY: ServoLoopRate
ServoLoopRate:
	$(MAKE) -C ServoLoopRate -f ServoLoopRate.mak

.PHONY: Vibration
Vibration:
	$(MAKE) -C Vibration -f Vibration.mak

.PHONY: clean
clean:
	$(MAKE) -C AnchoredSpringForce -f AnchoredSpringForce.mak clean
	$(MAKE) -C Calibration -f Calibration.mak clean
	$(MAKE) -C QueryDevice -f QueryDevice.mak clean
	$(MAKE) -C ErrorHandling -f ErrorHandling.mak clean
	$(MAKE) -C FrictionlessPlane -f FrictionlessPlane.mak clean
	$(MAKE) -C FrictionlessSphere -f FrictionlessSphere.mak clean
	$(MAKE) -C HelloHapticDevice -f HelloHapticDevice.mak clean
	$(MAKE) -C PreventWarmMotors -f PreventWarmMotors.mak clean
	$(MAKE) -C ServoLoopDutyCycle -f ServoLoopDutyCycle.mak clean
	$(MAKE) -C ServoLoopRate -f ServoLoopRate.mak clean
	$(MAKE) -C Vibration -f Vibration.mak clean
	$(MAKE) -C CommandMotorDAC -f CommandMotorDAC.mak clean
	$(MAKE) -C CommandJointTorque -f CommandJointTorque.mak clean
#	$(MAKE) -C Viscosity -f Viscosity.mak clean

.PHONY: install
install:
	$(MAKE) -C AnchoredSpringForce -f AnchoredSpringForce.mak install
	$(MAKE) -C Calibration -f Calibration.mak install
	$(MAKE) -C QueryDevice -f QueryDevice.mak install
	$(MAKE) -C ErrorHandling -f ErrorHandling.mak install
	$(MAKE) -C FrictionlessPlane -f FrictionlessPlane.mak install
	$(MAKE) -C FrictionlessSphere -f FrictionlessSphere.mak install
	$(MAKE) -C HelloHapticDevice -f HelloHapticDevice.mak install
	$(MAKE) -C PreventWarmMotors -f PreventWarmMotors.mak install
	$(MAKE) -C ServoLoopDutyCycle -f ServoLoopDutyCycle.mak install
	$(MAKE) -C ServoLoopRate -f ServoLoopRate.mak install
	$(MAKE) -C Vibration -f Vibration.mak install
	$(MAKE) -C CommandMotorDAC -f CommandMotorDAC.mak install
	$(MAKE) -C CommandJointTorque -f CommandJointTorque.mak install
#	$(MAKE) -C Viscosity -f Viscosity.mak install
	install -m 644 -o 0 -g 0 Makefile $(examples_dir)/HD/console/
#	install -m 755 -o 0 -g 0 run_hd_console_examples.pl $(examples_dir)/HD/console/


