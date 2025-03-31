cd src
make -fmakefile.bcc 
if errorlevel > 0 goto end
cd ..\examples\mswin
make -fmakefile.bcc
pause
if errorlevel > 0 goto end
if "%2" == "nofonts" goto end
cd ..\..\hershey\src
make -fmakefile.bcc
if errorlevel > 0 goto end
md c:\lib\hershey
mkfnts c:\lib\hershey
:end
