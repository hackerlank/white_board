#pragma once
class ConvertEndianUtil
{
public:
	ConvertEndianUtil(void);
	~ConvertEndianUtil(void);
	short static convertEndianShort(short t);      

	int  static  convertForInt(int t);
};

