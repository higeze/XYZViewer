#pragma once

namespace NotifyContainerChangedAction{
	enum Type{
		Add,
		Clear,
		//Move,
		//Remove
		//Replace,
		//Reset,

	};
}

template<class T>
struct NotifyContainerChangedEventArgs
{
	NotifyContainerChangedAction::Type Action;
	std::vector<T> NewItems;
	std::vector<T> OldItems;

	NotifyContainerChangedEventArgs(NotifyContainerChangedAction::Type action, std::vector<T> newItems, std::vector<T> oldItems)
		:Action(action), NewItems(newItems), OldItems(oldItems){}
	virtual ~NotifyContainerChangedEventArgs(){}
};