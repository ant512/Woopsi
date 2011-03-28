@echo off

call hg archive -tfiles C:\Woopsi

rem Tidy up directory structure
cd C:\Woopsi\
del .hg_archival.txt
del .hgignore
del .hgtags
del Woopsi.sln
del ToDo.txt
rmdir Woopsi\images /s /q
rmdir Woopsi\Release /s /q
del Woopsi\Woopsi.vcproj
move README.txt Woopsi\

rem Build library
cd Woopsi\libwoopsi
make
rmdir build /s /q

rem Finishing touch
cd ..\..
del MakeRelease.bat