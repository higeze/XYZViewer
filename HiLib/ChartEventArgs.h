#pragma once
#include "MyDC.h"

struct CPaintChartEventArgs
{
	CDC* DCPtr;
	CPaintChartEventArgs(CDC* pDC)
		:DCPtr(pDC){}
	virtual ~CPaintChartEventArgs(){}
};