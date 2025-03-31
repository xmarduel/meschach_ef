echo off
cd src
echo Making VOPL library
make -fmakefile.tc
cd ..\examples
echo Making C examples
make -fmakefile.tc
cd ..\gpp
echo Making the GPP program
make -fmakefile.tc
