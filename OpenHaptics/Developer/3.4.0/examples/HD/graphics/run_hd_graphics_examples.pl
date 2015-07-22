#!/usr/bin/perl

#############################################
#
# SensAble Technologies, Inc. 2005
#
# run HD graphics examples
#############################################

sub RunExampleLoop
{
    my($option) = 0;
    while (1)
    {
	print "\n=================================\n";
	print "Choose from the following examples\n";
	print "    (a) CoulombField\n";
	print "    (b) CoulombForceDual\n";
	print "    (c) ParticleWaltz\n";
	print "    (d) PointManipulation\n";
	print "    (e) PointSnapping\n";
	print "    (f) SimpleHapticScene\n";
	print "    (g) SlidingContact\n";
	print "    (q) Exit\n";
	print "\nChoice: ";
	chomp($option = <STDIN>);
	if ($option eq 'a')
	{
		system "CoulombField/CoulombField";
	}
	elsif ($option eq 'b')
	{
		system "CoulombForceDual/CoulombForceDual";
	}
	elsif ($option eq 'c')
	{
		system "ParticleWaltz/ParticleWaltz";
	}
	elsif ($option eq 'd')
	{
		system "PointManipulation/PointManipulation";
	}
	elsif ($option eq 'e')
	{
		system "PointSnapping/PointSnapping";
	}
	elsif ($option eq 'f')
	{
		system "SimpleHapticScene/SimpleHapticScene";
	}
	elsif ($option eq 'g')
	{
		system "SlidingContact/SlidingContact";
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

