#!/usr/bin/perl

#############################################
#
# SensAble Technologies, Inc. 2005
#
# run HL console examples
#############################################

sub RunExampleLoop
{
    my($option) = 0;
    while (1)
    {
	print "\n=================================\n";
	print "Choose from the following examples\n";
	print "    (a) CannedForceEffect\n";
	print "    (b) CustomForceEffect\n";
	print "    (c) CustomShape\n";
	print "    (d) Deployment\n";
	print "    (e) EffectAttributes\n";
	print "    (q) Exit\n";
	print "\nChoice: ";
	chomp($option = <STDIN>);
	if ($option eq 'a')
	{
		system "CannedForceEffect/CannedForceEffect";
	}
	elsif ($option eq 'b')
	{
		system "CustomForceEffect/CustomForceEffect";
	}
	elsif ($option eq 'c')
	{
		system "CustomShape/CustomShape";
	}
	elsif ($option eq 'd')
	{
		system "Deployment/Deployment";
	}
	elsif ($option eq 'e')
	{
		system "EffectAttributes/EffectAttributes";
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

