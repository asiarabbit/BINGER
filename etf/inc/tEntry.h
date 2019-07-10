/**
	\file tEntry.h
	\struct tEntry
	\brief For holding raw data information of a data section defined in GDAQ
	system. This struct is used in binary data reading and data distribution, and holds
	one channel from an event in the binary file.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/10 Last revised: 2018/5/26, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _tENTRY_H_
#define _tENTRY_H_


struct tEntry{ // struct of reading one channel from binary files
	char name[256]; ///< entry name, showing the identity of the entry
	int index;
	int channelId;
	int nl; ///< count of leading edges in one data section
	int nt; ///< count of trailing edges in one dat section
	static const int NMAX = 10; ///< edge number limit
	double leadingTime[NMAX];
	double trailingTime[NMAX];
	bool is_V;
	int bunchId; ///< trigger time in one time cycle

	tEntry();
	void initialize();
	void show() const;
	double TOT() const;
	double TOT1() const;
};

#endif
