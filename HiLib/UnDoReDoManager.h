#pragma once

class IDoCommand;

class CUnDoReDoManager
{
private:
	static const int kMaxsize = 10;
	bool m_canUnDo;
	bool m_canReDo;

	std::deque<std::shared_ptr<IDoCommand>> m_undos;
	std::deque<std::shared_ptr<IDoCommand>> m_redos;
public:
	boost::signals2::signal<void(LPCTSTR)> PropertyChanged;
public:
	/**
	 *  Constructor
	 */
	CUnDoReDoManager():m_canUnDo(false),m_canReDo(false){}
	/**
	 *  Destructor
	 */
	virtual ~CUnDoReDoManager(){}

	void Do(std::shared_ptr<IDoCommand> command);
	void UnDo();
	void ReDo();
	bool GetCanUnDo()const;
	bool GetCanReDo()const;
	void SetCanUnDo(bool canUnDo);
	void SetCanReDo(bool canReDo);

	void Clear();

};
