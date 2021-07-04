# Project: ICP project
# File: Makefile 
# Description: This Makefile generates project
# Authors: David Hudák (xhudak03; Makefile, frontend) and Štefan Mraèna (xmracn00; backend)
# Short description

all: update
	make -f QMAKEFILE 

run: all
	./BlockEditor

update: src/ICP.pro 
	qmake -o QMAKEFILE src/ICP.pro 

doxygen:
	doxygen doc/Doxyfile 

clean:
	rm -f BlockEditor *.o QMAKEFILE moc_* qrc_myRes.cpp ui_mainwindow.h ui_helpwindow.h

pack:
	zip -r 2-xhudak03-xmracn00.zip src examples README.txt README.md Makefile doc/Doxyfile
