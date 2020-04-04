cs: deconvolution.C \
	/home/asia/pionExp2017/build/PID/errPro/ExpData.h \
	/home/asia/pionExp2017/build/PID/errPro/ExpData.C
	g++ -o $@ $< -g

csInter: interactionCS.C \
	/home/asia/pionExp2017/build/PID/errPro/ExpData.h \
	/home/asia/pionExp2017/build/PID/errPro/ExpData.C
	g++ -o $@ $< -g

csOri: deconvolutionOri.C \
	/home/asia/pionExp2017/build/PID/errPro/ExpData.h \
	/home/asia/pionExp2017/build/PID/errPro/ExpData.C
	g++ -o $@ $< -g

clean:
ifeq '$(wildcard cs)' 'cs'
	rm cs
endif

ifeq '$(wildcard csOri)' 'csOri'
	rm csOri
endif

