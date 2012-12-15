#pragma once
#include "Delegate.hpp"
#include <list>

namespace SpaceMarines
{

namespace EventType
{ enum List {
	Start = 0,
	Count
};}

template<class ...A>
class Event
{
private:
	typedef void* InstancePointer;
	typedef void (*InternalFunc)(InstancePointer, A...);

	struct Handler
	{
		Handler()
		{
			instance = nullptr;
			func = nullptr;
		}
		InstancePointer instance;
		InternalFunc func;
	};

	template <void (*Func)(A...)>
	static inline void FunctionStub(InstancePointer, A... args)
	{
		return (Func)(args...);
	}

	template <class C, void (C::*Function)(A...)>
	static inline void MethodStub(InstancePointer instance, A... args)
	{
	    return (static_cast<C*>(instance)->*Function)(args...);
	}

	///////////////////////////////////////////////////////////////////

	std::list<Handler> handlers;
	typedef typename std::list<Handler>::iterator Iter;
public:
	Event()
	{
	}

	~Event()
	{
		handlers.clear();
	}

	void clearHandlers()
	{
		handlers.clear();
	}

	template <class C, void (C::*Method)(A...)>
	void addListener(C* obj)
	{
		Handler h;
		h.instance = obj;
		h.func = &MethodStub<C, Method>;
		handlers.push_back(h);
	}

	void fire(A... args)
	{
		for (Iter it = handlers.begin(); it != handlers.end(); it++)
		{
			it->func(it->instance, args...);
		}
	}
};

}
