echo off
echo Making directories
md %1\vopl
md %1\vopl\src
md %1\vopl\src\msfort
md %1\vopl\src\sunfort
md %1\vopl\examples
md %1\vopl\gpp
echo Copying
copy *.* %1\vopl
copy src\*.* %1\vopl\src
copy src\msfort\*.* %1\vopl\src\msfort
copy src\sunfort\*.* %1\vopl\src\sunfort
copy examples\*.* %1\vopl\examples
copy gpp\*.* %1\vopl\gpp
