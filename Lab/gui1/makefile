##################################################################################################
# Data Analysis Code Project for the External Target Facility, CSR-HIRFL, @IMP
#
# Building code for GUI system for demonstration purposes to plain users.
#
# Created Time: 2018/6/24
# Author: SUN Yazhou, asia.rabbit@163.com
# Last Modified: 2018/6/24, SUN Yazhou
##################################################################################################


ROOTINCDIR	:= $(shell root-config --incdir)
ROOTGLIBS	:= $(shell root-config --glibs)
ROOTCFLAGS	:= $(shell root-config --cflags)
RO			:= $(shell root-config --cxx) $(ROOTCFLAGS) $(ROOTGLIBS)


gui: test.C MyMainFrameDict.C MyMainFrame.o
	$(RO) $^ -o $@

MyMainFrame.o: MyMainFrame.C MyMainFrame.h
	g++ -I$(ROOTINCDIR) -c $< -o $@

MyMainFrameDict.C: MyMainFrame.h MyMainFrameLinkDef.h
	rootcling -f $@ -c $^

clean:
ifeq '$(wildcard gui)' 'gui'
	rm main
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


