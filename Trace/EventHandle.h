#pragma once
#include <list>
using namespace std;
enum class EventType
{
	OnUpdata,
	OnPresent,
	OnWndProc
};
template<EventType Type>
class EventHandle
{
	list<void*> EventCallbacks;
public:
	static auto GetIns()
	{
		static auto _Event = new EventHandle();
		return _Event;
	}
	void Add(void* callback)
	{
		if (callback != nullptr)
		{
			EventCallbacks.push_back(callback);

		}
	}
	void Remove(void* listener)
	{
		if (listener == nullptr)
			return;
		EventCallbacks.remove(listener);
	}
	template<typename  ...Args>
	void Invoke(Args ... args)
	{
		for (auto CallBack : EventCallbacks)
		{
			reinterpret_cast<void(_cdecl*)(Args ...)>(CallBack)(args ...);
		}
	}
};

