:: simple build script
@set MATLABROOT=C:\apps\MATLAB\R2024b\extern
@set CLOPTS=/nologo
@set CLOPTS=%CLOPTS% /I%MATLABROOT%\include
@set CLOPTS=%CLOPTS% /c

@set LINKOPTS=/nologo
@set LINKOPTS=%LINKOPTS% /DLL
@set LINKOPTS=%LINKOPTS% /OUT:mexListDlls.mexw64
@set LINKOPTS=%LINKOPTS% /EXPORT:mexFunction

@set LINKLIBS=
@set LINKLIBS=%LINKLIBS% %MATLABROOT%\lib\win64\microsoft\libmex.lib
@set LINKLIBS=%LINKLIBS% %MATLABROOT%\lib\win64\microsoft\libmx.lib

cl %CLOPTS% mexListDlls.c

link %LINKOPTS% mexListDlls.obj %LINKLIBS%
