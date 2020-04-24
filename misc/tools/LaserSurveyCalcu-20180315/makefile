CXX := g++ -g

all: dc0 dc1 dc2 tofw

dc0: DCPos-DC0.C
	$(CXX) $< -o $@
dc1: DCPos-DC1.C
	$(CXX) $< -o $@
dc2: DCPos-DC2.C
	$(CXX) $< -o $@
tofw: TOFWPos.C
	$(CXX) $< -o $@

clean:
ifeq '$(wildcard dc0)' 'dc0'
	rm dc0
endif
ifeq '$(wildcard dc1)' 'dc1'
	rm dc1
endif
ifeq '$(wildcard dc2)' 'dc2'
	rm dc2
endif
ifeq '$(wildcard tofw)' 'tofw'
	rm tofw
endif

