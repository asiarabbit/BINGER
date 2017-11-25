OBJDIR = $(GARFIELD_HOME)/Object
SRCDIR = $(GARFIELD_HOME)/Source
INCDIR = $(GARFIELD_HOME)/Include
HEEDDIR = $(GARFIELD_HOME)/Heed
LIBDIR = $(GARFIELD_HOME)/Library

# Compiler flags
CFLAGS = -Wall -Wextra -Wno-long-long \
	`root-config --cflags` \
	-O3 -fno-common \
	-I$(INCDIR) -I$(HEEDDIR)

# Debug flags
# CFLAGS += -g

LDFLAGS = `root-config --glibs` -lGeom -lgfortran -lm
LDFLAGS += -L$(LIBDIR) -lGarfield
# LDFLAGS += -g

all: DC

DC: DC.C 
	$(CXX) $(CFLAGS) $< -o $@ $(LDFLAGS) 
test: test.C 
	$(CXX) $(CFLAGS) $< -o $@ $(LDFLAGS) 

tube: tube.C 
	$(CXX) $(CFLAGS) $< -o $@ $(LDFLAGS) 

clean:
ifeq '$(wildcard DC)' 'DC'
	rm DC
endif
ifeq '$(wildcard test)' 'test'
	rm test
endif
ifeq '$(wildcard tube)' 'tube'
	rm tube
endif

