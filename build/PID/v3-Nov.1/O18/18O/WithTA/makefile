cs: deconvolution.C \
	/home/asia/pionExp2017/build/PID/errPro/ExpData.h \
	/home/asia/pionExp2017/build/PID/errPro/ExpData.C
	g++ -o $@ $<

csNOPU: deconvolutionNOPU.C \
	/home/asia/pionExp2017/build/PID/errPro/ExpData.h \
	/home/asia/pionExp2017/build/PID/errPro/ExpData.C
	g++ -o $@ $<

clean:
ifeq '$(wildcard cs)' 'cs'
	rm cs
endif
ifeq '$(wildcard csNOPU)' 'csNOPU'
	rm csNOPU
endif

