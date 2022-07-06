@if (@CodeSection == @Batch) @then

@echo off
setlocal

for /L %%I in (63,-1,0) do (
    set /P "=%%I... "<NUL
	D:\Programy\hoot\hootclient volume %%I
    call :pause 140
)

goto :EOF

:pause <ms>
cscript /nologo /e:JScript "%~f0" "%~1"
goto :EOF

@end
WSH.Sleep(WSH.Arguments(0));