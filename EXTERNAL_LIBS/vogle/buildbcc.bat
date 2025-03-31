cd src
make -fmakefile.tc
if errorlevel > 0 goto end
cd ..\examples
make -fmakefile.tc
if errorlevel > 0 goto end
if "%2" == "nofonts" goto end
cd ..\hershey\src
make -fmakefile.tc
if errorlevel > 0 goto end
md c:\lib\hershey
mkfnts c:\lib\hershey
:end
