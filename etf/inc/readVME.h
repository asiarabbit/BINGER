/**
	\file readVME.h
	\brief The header file for structs used in class TABUAA, struct for reading binary data
	file from VME data acquisition system, which works coorperatively with PXI Daq to
	provide particle energy loss and tracking information before the reaction target.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2016/11/21 Last revised: 2018/1/9, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/

#ifndef __READVME_H__
#define __READVME_H__

#include <vector>

using std::vector;

/// \struct tVME_event
/// \brief Struct to store a VME event 
struct tVME_event{
	int index; //!< event index in a block
	int length; //!< event length
	int VMEReadCount; // ?
	int adc[32]; //!< MUlti-Sampling Ionization Chamber (music) data
	int tdc[2][32]; //!< MWPC1 and MWDC2 upstream the target and MWPC3 downstream the target.
	int qdc[3][32]; //!< 3 modules * 32 channels.
	int mtdc0[128][10]; // 
	int mtdc1[128][10]; // 
	int hit[32];
	unsigned sca[16];
	unsigned psca[16];
	int dsca[16];
	tVME_event(){ //!< the default constructor
		initialize();
	} // end of the default constructor
	void initialize(){
		index = -1; length = -9999.; VMEReadCount = -9999.;
		for(int i = 0; i < 32; i++){
			tdc[0][i] = -9999.; tdc[1][i] = -9999.; adc[i] = -9999.; hit[i] = 0;
			qdc[0][i] = -9999.; qdc[1][i] = -9999.; qdc[2][i] = -9999.;
		} // end for over i
		for(int i = 0; i < 128; i++) for(int j = 0; j < 10; j++){
			if(mtdc0[i][j] != -9999.) mtdc0[i][j] = -9999.;
			if(mtdc1[i][j] != -9999.) mtdc1[i][j] = -9999.;
		}
		for(int i = 0; i < 16; i++){
			sca[i] = 0; psca[i] = 0; dsca[i] = 0;
		} // end for over i
	} // end of function initialize.
};

/// \struct tVME_block
/// \brief a struct to store a VME data block which encapsulated many events
struct tVME_block{
	int index; //!< block index
	int event_num; //!< number of events contained.
	int lastPos; // ?
	vector<tVME_event> event; //!< events in the block
	/// the default constuctor
	tVME_block(){
		initialize();
	} // end of the default constructor.
	void initialize(){
		index = -1; event_num = 0;
		lastPos = 0;
		event.reserve(200);
	} // end of function initialize.
};
/// read one event from VME binary data file.
/// \param data: one data block - 8192 bytes containing several events
void analyze(const int *data, tVME_event &event);
#endif
