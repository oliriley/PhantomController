@echo off
rem SetPhantomConfigDirectory.bat
rem For Vista Only!
rem Set the PHANTOM_CONFIG_DIRECTORY to an alternative location, 
rem so users don’t have to run elevated to access the configuration files.

mkdir %PUBLIC%\Documents\Sensable 
setx PHANTOM_CONFIG_DIRECTORY %PUBLIC%\Documents\Sensable /m
echo. This is a token to flag that PHANTOM device driver installation is in progress. > %PHANTOM_CONFIG_DIRECTORY%\installing.

@echo on

 

