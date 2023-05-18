@echo off


for %%i in (*.cpp) do (
    cl.exe /EHsc /W4 /nologo  pdh.lib *.cpp
)