#pragma once
#include "IDoCommand.h"

class CCell;
class CCellStringCommand:public IDoCommand
{
private:
	CCell* m_pCell;
	std::wstring m_prevString;
	std::wstring m_nextString;

public:
	CCellStringCommand(CCell* pCell, const std::wstring& prevString, const std::wstring& nextString);
	virtual ~CCellStringCommand(){}
	virtual void Do()override;
	virtual void UnDo()override;
	virtual void ReDo()override;
};