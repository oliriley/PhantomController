#!/usr/bin/perl

#############################################
#
# SensAble Technologies, Inc. 2005
#
# run HD console examples
#############################################

sub RunExampleLoop
{
    my($option) = 0;
    while (1)
    {
	print "\n=================================\n";
	print "Choose from the following examples\n";
	print "    (a) AnchoredSpringForce\n";
	print "    (b) Calibration\n";
	print "    (c) ErrorHandling\n";
	print "    (d) FrictionlessPlane\n";
	print "    (e) FrictionlessSphere\n";
	print "    (f) HelloHapticDevice\n";
	print "    (g) PreventWarmMotors\n";
	print "    (h) QueryDevice\n";
	print "    (i) ServoLoopDutyCycle\n";
	print "    (j) ServoLoopRate\n";
	print "    (k) Vibration\n";
	print "    (q) Exit\n";
	print "\nChoice: ";
	chomp($option = <STDIN>);
	if ($option eq 'a')
	{
		system "AnchoredSpringForce/AnchoredSpringForce";
	}
	elsif ($option eq 'b')
	{
		system "Calibration/Calibration";
	}
	elsif ($option eq 'c')
	{
		system "ErrorHandling/ErrorHandling";
	}
	elsif ($option eq 'd')
	{
		system "FrictionlessPlane/FrictionlessPlane";
	}
	elsif ($option eq 'e')
	{
		system "FrictionlessSphere/FrictionlessSphere";
	}
	elsif ($option eq 'f')
	{
		system "HelloHapticDevice/HelloHapticDevice";
	}
	elsif ($option eq 'g')
	{
		system "PreventWarmMotors/PreventWarmMotors";
	}
	elsif ($option eq 'h')
	{
		system "QueryDevice/QueryDevice";
	}
	elsif ($option eq 'i')
	{
		system "ServoLoopDutyCycle/ServoLoopDutyCycle";
	}
	elsif ($option eq 'j')
	{
		system "ServoLoopRate/ServoLoopRate";
	}
	elsif ($option eq 'k')
	{
		system "Vibration/Vibration";
	}
	elsif ($option eq 'q')
	{
		exit;
	}
	else
	{
		print "\n<Try again>\n\n"
	}
    }
}


&RunExampleLoop

