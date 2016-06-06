#include "ConvertEndianUtil.h"


ConvertEndianUtil::ConvertEndianUtil(void)
{
}


ConvertEndianUtil::~ConvertEndianUtil(void)
{
}


short ConvertEndianUtil::convertEndianShort(short t){
	short tResult = 0; 
	for (int  I = 0; I < sizeof(short); ++ I) 
	{ 
		tResult <<= 8;    
		tResult |= (t & 0xFF) ; 

		t >>= 8; 
	} 
	return tResult;
}


int  ConvertEndianUtil::convertForInt(int t){
	int tResult = 0; 
	for (int  I = 0; I < sizeof(int); ++ I) 
	{ 
		tResult <<= 8;    
		tResult |= (t & 0xFF) ; 

		t >>= 8; 
	} 
	return tResult;
}
