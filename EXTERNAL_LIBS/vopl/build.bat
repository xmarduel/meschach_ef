echo off
if "%1" == "" goto err
cd src
echo Making VOPL library
make F=%1 makefile.msc
cd ..\examples
echo Making C examples
make F=%1 makefile.msc
cd ..\gpp
echo Making the GPP program
make F=%1 makefile.msc
goto end
:err
echo "usage: build -FPc | -FPi87"
:end
