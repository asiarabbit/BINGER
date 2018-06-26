##################################################################################################
# Data Analysis Code Project for the External Target Facility, CSR-HIRFL, @IMP
#
# Building code for GUI system for demonstration purposes to plain users.
#
# Created Time: 2018/6/24
# Author: SUN Yazhou, asia.rabbit@163.com
# Last Modified: 2018/6/24, SUN Yazhou
##################################################################################################

CXX			:= g++ -O0 -Wall -ggdb
ROOTINCDIR	:= $(shell root-config --incdir)
ROOTGLIBS	:= $(shell root-config --glibs)
ROOTCFLAGS	:= $(shell root-config --cflags)
RO			:= $(CXX) $(ROOTCFLAGS) $(ROOTGLIBS)


gui: gui.C MyMainFrameDict.C MyMainFrame.o
	$(RO) $^ -o $@

MyMainFrame.o: MyMainFrame.C custom.C MyMainFrame.h
	$(CXX) -I$(ROOTINCDIR) -c $< -o $@

MyMainFrameDict.C: MyMainFrame.h MyMainFrameLinkDef.h
	rootcling -f $@ -c $^

clean:
ifeq '$(wildcard gui)' 'gui'
	rm gui
endif
ifeq '$(wildcard MyMainFrame.o)' 'MyMainFrame.o'
	rm MyMainFrame.o
endif
ifeq '$(wildcard MyMainFrameDict.C)' 'MyMainFrameDict.C'
	rm MyMainFrameDict.C
endif
ifeq '$(wildcard MyMainFrameDict_rdict.pcm)' 'MyMainFrameDict_rdict.pcm'
	rm MyMainFrameDict_rdict.pcm
endif


