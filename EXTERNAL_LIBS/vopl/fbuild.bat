echo off
if "%1" == "" goto err
echo Making FORTRAN interfaces
cd src\msfort
make F=%1 makefile.msc
cd ..
echo Making VOPL library
make F=%1 fmakefil.msc
cd ..\examples
echo Making VOPL examples
command /c f_2_for
make F=%1 fmakefil.msc
cd ..\gpp
echo Making GPP program
make F=%1 makefile.msc
goto end
:err
echo "usage: fbuild -FPc | -FPi87"
:end
