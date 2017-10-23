#include "UnDoReDoManager.h"
#include "IDoCommand.h"

void CUnDoReDoManager::Do(std::shared_ptr<IDoCommand> command)
{
	m_undos.push_back(command);
	if(m_undos.size() > kMaxsize){
		m_undos.pop_front();
	}

	command->Do();

	m_redos.clear();

	SetCanUnDo(!m_undos.empty() && m_undos.size()>0);
	SetCanReDo(!m_redos.empty() && m_redos.size()>0);
}
void CUnDoReDoManager::UnDo()
{
	if(GetCanUnDo()){
		auto command = m_undos.back();
		m_undos.pop_back();

		command->UnDo();

		m_redos.push_back(command);
		if(m_redos.size() > kMaxsize){
			m_redos.pop_front();
		}
	}

	SetCanUnDo(!m_undos.empty() && m_undos.size()>0);
	SetCanReDo(!m_redos.empty() && m_redos.size()>0);
}
void CUnDoReDoManager::ReDo()
{
	if(GetCanReDo()){
		auto command = m_redos.back();
		m_redos.pop_back();

		command->ReDo();

		m_undos.push_back(command);
		if(m_undos.size() > kMaxsize){
			m_undos.pop_front();
		}
	}

	SetCanUnDo(!m_undos.empty() && m_undos.size()>0);
	SetCanReDo(!m_redos.empty() && m_redos.size()>0);
}
bool CUnDoReDoManager::GetCanUnDo()const
{
	return m_canUnDo;//!m_undos.empty() && m_undos.size()>0;
}
bool CUnDoReDoManager::GetCanReDo()const
{
	return m_canReDo;//!m_redos.empty() && m_redos.size()>0;
}

void CUnDoReDoManager::SetCanUnDo(bool canUnDo)
{
	if(m_canUnDo!=canUnDo){
		m_canUnDo = canUnDo;
		PropertyChanged(L"CanUnDo");
	}
}
void CUnDoReDoManager::SetCanReDo(bool canReDo)
{
	if(m_canReDo!=canReDo){
		m_canReDo = canReDo;
		PropertyChanged(L"CanReDo");
	}
}

void CUnDoReDoManager::Clear()
{
	m_undos.clear();
	m_redos.clear();
	SetCanUnDo(!m_undos.empty() && m_undos.size()>0);
	SetCanReDo(!m_redos.empty() && m_redos.size()>0);
}