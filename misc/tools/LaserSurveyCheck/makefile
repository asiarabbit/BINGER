traceL0: trace_DCVersion/LDC0/trace.cxx inputLocal.C inputGlobal.C math.C header.C
	g++ -o $@ $< -I ./
traceL1: trace_DCVersion/LDC1/trace.cxx inputLocal.C inputGlobal.C math.C header.C
	g++ -o $@ $< -I ./
traceL2: trace_DCVersion/LDC2/trace.cxx inputLocal.C inputGlobal.C math.C header.C
	g++ -o $@ $< -I ./
check: check.cxx inputLocal.C inputGlobal.C math.C header.C
	g++ -o $@ $<
traceR0: trace_DCVersion/RDC0/trace.cxx inputLocal.C inputGlobal.C math.C header.C
	g++ -o $@ $< -I ./
traceR1: trace_DCVersion/RDC1/trace.cxx inputLocal.C inputGlobal.C math.C header.C
	g++ -o $@ $< -I ./
traceR2: trace_DCVersion/RDC2/trace.cxx inputLocal.C inputGlobal.C math.C header.C
	g++ -o $@ $< -I ./

.PHONY: clean

clean:
ifeq '$(wildcard traceL0)' 'traceL0'
	rm traceL0
endif
ifeq '$(wildcard traceL1)' 'traceL1'
	rm traceL1
endif
ifeq '$(wildcard traceL2)' 'traceL2'
	rm traceL2
endif
ifeq '$(wildcard traceR0)' 'traceR0'
	rm traceR0
endif
ifeq '$(wildcard traceR1)' 'traceR1'
	rm traceR1
endif
ifeq '$(wildcard traceR2)' 'traceR2'
	rm traceR2
endif
clean1:
ifeq '$(wildcard detPos/detpos.001)' 'detPos/detpos.001'
	rm detPos/detpos.001
endif

