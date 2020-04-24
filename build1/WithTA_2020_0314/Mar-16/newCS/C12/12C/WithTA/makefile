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

ccl: B.C \
	/home/asia/pionExp2017/build/PID/errPro/ExpData.h \
	/home/asia/pionExp2017/build/PID/errPro/ExpData.C
	g++ -o $@ $< -g

clean:
ifeq '$(wildcard csOri)' 'csOri'
	rm csOri
endif

ifeq '$(wildcard cs)' 'cs'
	rm cs
endif

ifeq '$(wildcard csInter)' 'csInter'
	rm csInter
endif

ifeq '$(wildcard ccl)' 'ccl'
	rm ccl
endif

