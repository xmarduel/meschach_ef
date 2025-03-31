echo off
rem
rem  Copys vogle directories onto floppy disk
rem
rem  Usage: cd to vogle directory, then cpfd A:
rem
echo MAKING DIRECTORIES
md %1\vogle
md %1\vogle\src
md %1\vogle\src\msfort
md %1\vogle\src\sunfort
md %1\vogle\src\sunpas
md %1\vogle\drivers
md %1\vogle\drivers\ibmpc
md %1\vogle\examples
md %1\vogle\examples\mswin
md %1\vogle\examples\xview
md %1\vogle\examples\sunview
md %1\vogle\examples\xt
md %1\vogle\hershey
md %1\vogle\hershey\fonts
md %1\vogle\hershey\src
md %1\vogle\hershey\data
md %1\vogle\docs
echo "DONE MAKING DIRECTORIES"
rem
rem  Do the copying
rem
echo BEGIN COPYING
copy *.* %1\vogle
copy src\*.* %1\vogle\src
copy src\msfort\*.* %1\vogle\src\msfort
copy src\sunfort\*.* %1\vogle\src\sunfort
copy src\sunpas\*.* %1\vogle\src\sunpas
rem
copy drivers\*.* %1\vogle\drivers
copy drivers\ibmpc\*.* %1\vogle\drivers\ibmpc
rem
copy examples\*.* %1\vogle\examples
copy examples\mswin\*.* %1\vogle\examples\mswin
copy examples\xview\*.* %1\vogle\examples\xview
copy examples\sunview\*.* %1\vogle\examples\sunview
copy examples\xt\*.* %1\vogle\examples\xt
rem
copy hershey\*.* %1\vogle\hershey
copy hershey\src\*.* %1\vogle\hershey\src
copy hershey\data\*.* %1\vogle\hershey\data
copy hershey\fonts\*.* %1\vogle\hershey\fonts
copy docs\*.* %1\vogle\docs
echo FINISHED COPYING
