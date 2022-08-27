#Makefile

SHELL=cmd

all: run

run:
	cd build && cd Debug && ./calculatorGui.exe

profile:
	cd build && cd Debug && ./profiling.exe

help:
	cat help.txt

clean:
	rmdir /s build

pack: 
	cd .. && set foo=%CD% && cd .. && xcopy /S "./doc" "./xkment06_xmracn00_xpopel22_xhudak03/doc" && xcopy /S "./install" "./xkment06_xmracn00_xpopel22_xhudak03/install" && xcopy /S %foo% "./xkment06_xmracn00_xpopel22_xhudak03/repo" && powershell "compress-archive xkment06_xmracn00_xpopel22_xhudak03 xkment06_xmracn00_xpopel22_xhudak03.zip"