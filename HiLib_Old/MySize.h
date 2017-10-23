#pragma once

class CSize:public tagSIZE
{
public:
	CSize(){cx=0;cy=0;}
	CSize(LONG lCx,LONG lCy){cx=lCx;cy=lCy;}
	//virtual ~CSize(){}//Because of Struct
};