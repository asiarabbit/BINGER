/**
	\file TAUIDParser.h
	\class TAUIDParser
	\brief Resolve and interpret uid to comprehensible results, used for
	objects identification.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/16 Last revised: 2017/10/16, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAUIDPARSER_H_
#define _TAUIDPARSER_H_

class TAUIDParser{
public:
	TAUIDParser();
	virtual ~TAUIDParser();
	
	static void DNS(int *result, unsigned uid);
	static unsigned UIDGenerator(const int *type);
};

#endif
