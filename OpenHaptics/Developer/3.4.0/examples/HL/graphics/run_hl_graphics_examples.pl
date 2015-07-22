#!/usr/bin/perl

#############################################
#
# SensAble Technologies, Inc. 2005
#
# run HL graphics examples
#############################################

sub RunExampleLoop
{
    my($option) = 0;
    while (1)
    {
	print "\n=================================\n";
	print "Choose from the following examples\n";
	print "    (a) Constraints\n";
	print "    (b) Events\n";
	print "    (c) HelloHaptics\n";
	print "    (d) HelloSphere\n";
	print "    (e) HelloSphereDual\n";
	print "    (f) PointManipulation\n";
	print "    (g) ShapeManipulation\n";
	print "    (h) SimpleDeformableSurface\n";
	print "    (i) SimpleRigidBodyDynamics\n";
	print "    (q) Exit\n";
	print "\nChoice: ";
	chomp($option = <STDIN>);
	if ($option eq 'a')
	{
		system "Constraints/Constraints";
	}
	elsif ($option eq 'b')
	{
		system "Events/Events";
	}
	elsif ($option eq 'c')
	{
		system "HelloHaptics/HelloHaptics";
	}
	elsif ($option eq 'd')
	{
		system "HelloSphere/HelloSphere";
	}
	elsif ($option eq 'e')
	{
		system "HelloSphereDual/HelloSphereDual";
	}
	elsif ($option eq 'f')
	{
		system "PointManipulation/PointManipulation";
	}
	elsif ($option eq 'g')
	{
		system "ShapeManipulation/ShapeManipulation";
	}
	elsif ($option eq 'h')
	{
		system "SimpleDeformableSurface/SimpleDeformableSurface";
	}
	elsif ($option eq 'i')
	{
		system "SimpleRigidBodyDynamics/SimpleRigidBodyDynamics";
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

