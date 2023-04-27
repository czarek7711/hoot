@if (@CodeSection == @Batch) @then

@echo off
setlocal

for /L %%I in (255,-1,192) do (
	set /P "=%%I... "<NUL
	D:\Programy\hoot\hootclient volume %%I
	call :pause 5
)

for /L %%I in (191,-2,128) do (
	set /P "=%%I... "<NUL
	D:\Programy\hoot\hootclient volume %%I
	call :pause 8
)

for /L %%I in (127,-2,64) do (
	set /P "=%%I... "<NUL
	D:\Programy\hoot\hootclient volume %%I
	call :pause 8
)

for /L %%I in (63,-1,0) do (
	set /P "=%%I... "<NUL
	D:\Programy\hoot\hootclient volume %%I
	call :pause 10
)

goto :EOF

:pause <ms>
cscript /nologo /e:JScript "%~f0" "%~1"
goto :EOF

@end
WSH.Sleep(WSH.Arguments(0));