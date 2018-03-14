#makefile for TAMatrix package
CXX			= g++
#using ROOT library
ROOTINC		:= $(shell root-config --incdir)
ROOTGLIBS	:= $(shell root-config --glibs)
ROOTCFLAGS	:= $(shell root-config --cflags)
RO		:= $(ROOTCFLAGS) $(ROOTGLIBS)
#local directories
INC		:= ./inc
SRC		:= ./src
OBJ		:= ./obj
LIB		:= ./lib
#files
SRCS		:= $(wildcard $(SRC)/*.C)
OBJS		:= $(addprefix $(OBJ)/, $(patsubst %.C,%.o,$(notdir $(SRCS))))
MIL		:= -I$(INC) -L$(LIB) -lmil
#compile static library.
AR		:= ar rcs

$(LIB)/libmil.a: $(OBJS)
	$(AR) $@ $^
#	rm $(OBJ)/*.o

$(OBJ)/TAMatrix.o: $(SRC)/TAMatrix.C $(INC)/TAMatrix.h
	$(CXX) -I$(INC) -c $< -o $@

$(OBJ)/TAMatrixSym.o: $(SRC)/TAMatrixSym.C $(INC)/TAMatrix.h $(INC)/TAMatrixSym.h
	$(CXX) -I$(INC) -c $< -o $@

$(OBJ)/TAFun.o: $(SRC)/TAFun.C $(INC)/TAFun.h
	$(CXX) -I$(INC) -c $< -o $@

clean:
ifeq '$(wildcard $(OBJ)/TAMatrix.o)' '$(OBJ)/TAMatrix.o'
	rm $(OBJ)/*.o
endif
ifeq '$(wildcard $(LIB)/libmil.a)' '$(LIB)/libmil.a'
	rm $(LIB)/libmil.a
endif

