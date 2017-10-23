#pragma once

class IDoCommand
{
public:
	IDoCommand(){}
	virtual ~IDoCommand(){}
	virtual void Do() = 0;
	virtual void UnDo() = 0;
	virtual void ReDo() = 0;
};

