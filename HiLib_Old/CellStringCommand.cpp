#include "CellStringCommand.h"
#include "Cell.h"
#include "Column.h"

CCellStringCommand::CCellStringCommand(CCell* pCell, const std::wstring& prevString, const std::wstring& nextString)
	:m_pCell(pCell), m_prevString(prevString), m_nextString(nextString){}

void CCellStringCommand::Do()
{
	m_pCell->SetStringNotify(m_nextString);
}
void CCellStringCommand::UnDo()
{
	m_pCell->SetStringNotify(m_prevString);
}
void CCellStringCommand::ReDo()
{
	m_pCell->SetStringNotify(m_nextString);
}