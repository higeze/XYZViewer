#include "MyPen.h"

int CPen::GetLeftTopPenWidth()const
{
	return static_cast<int>(floor(m_nWidth*0.5));
}
int CPen::GetRightBottomPenWidth()const
{
	return static_cast<int>(ceil(m_nWidth*0.5));
}