@echo off


for %%i in (*.exe) do (
    if exist %%~ni.exe (
        echo %%i
        %%i
    )
)
