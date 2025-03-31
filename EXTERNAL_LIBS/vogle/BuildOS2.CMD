cd src
make -fmakefile.os2
if errorlevel 1 goto end
cd ..\examples\os2pm
make -fmakefile.os2
if errorlevel 1 goto end
goto end

REM  Borland C++ for OS/2 doesn't compile h2v.c correctly so we can't
REM  make the Hershey fonts through OS/2.
pause
if "%2" == "nofonts" goto end
cd ..\..\hershey\src
make -fmakefile.os2
if errorlevel 1 goto end
md c:\lib\hershey
mkfnts c:\lib\hershey
:end
