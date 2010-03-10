@echo off

rmdir C:\devkitpro\libwoopsi /s /q
mkdir C:\devkitpro\libwoopsi
xcopy icon C:\devkitpro\libwoopsi\icon\
xcopy include C:\devkitpro\libwoopsi\include\
xcopy include\fonts C:\devkitpro\libwoopsi\include\fonts\
xcopy lib C:\devkitpro\libwoopsi\lib\