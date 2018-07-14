######################################
#  makefile for Ribll-Online         #
#                                    #
######################################


CC=gcc
CF=ifort
FF=gfortran
FFLAG= -w -o2 -fpp 

CPP=g++
CPPFLAG = -O2 -D TH_HBOOK -lrt
#ROOTLIB = $(shell root-config --cflags --glibs)
ROOTLIB = $(shell root-config --glibs)
SYSLIB= -lstdc++
#ROOTINC = -I$(shell root-config --incdir)
CRFLAG = $(shell root-config --cflags)
ROOTBIN = $(shell root-config --bindir)

PROG = online

OBJS = OnlineFrame.o Online.o TH_analyze.o TH_fill.o TH_init.o UserFunc.o

$(PROG):$(OBJS)
	$(CPP) -o $(PROG) $(OBJS) $(CPPFLAG) $(ROOTLIB) $(SYSLIB)
	
%.o: %.cpp
	$(CPP) -c $(CPPFLAG) $(CRFLAG) -o $@ $<

	
clean:
	-rm $(OBJS) $(PROG)

