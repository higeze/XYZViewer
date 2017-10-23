#pragma once

class CGridView;
class CSheetState;
struct KeyEventArgs;

class IKeyObserver
{
public:
	IKeyObserver(void){}
	virtual ~IKeyObserver(void){}
	virtual CSheetState* OnKeyDown(CGridView* pSheet, KeyEventArgs& e)=0;
};

