@echo off

for /d %%x in (.\*) do (
	cd %%x
	make clean
	make
	cd ..
)

pause