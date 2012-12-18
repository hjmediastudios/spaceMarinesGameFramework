#pragma once
#include <cstddef>
#include <algorithm>
#include <utility>

namespace SpaceMarines
{

template<class T> class Delegate {};

template <class R, class ...A>
class Delegate<R (A...)>
{
public:
    Delegate()
        : object_ptr(0)
        , stub_ptr(0)
    {}


	template <R (*TMethod)(A...)>
	static Delegate FromFunction()
	{
		return FromStub(0, &FunctionStub<TMethod>);
	}

	template <class T, R (T::*TMethod)(A...)>
	static Delegate FromMethod(T* object_ptr)
	{
		return FromStub(object_ptr, &MethodStub<T, TMethod>);
	}

    R invoke(A... args) const
    {
        return (*stub_ptr)(object_ptr, args...);
    }

    operator bool() const
	{
    	return stub_ptr != nullptr;
	}

    bool operator !() const
    {
    	return !(operator bool());
    }

    Delegate& operator=(const Delegate &other)
    {
    	object_ptr = other.object_ptr;
    	stub_ptr = other.stub_ptr;
    	return *this;
    }




private:
    typedef R (*stub_type)(void* object_ptr, A... args);

    void* object_ptr;
    stub_type stub_ptr;

    static Delegate FromStub(void* object_ptr, stub_type stub_ptr)
	{
		Delegate d;
		d.object_ptr = object_ptr;
		d.stub_ptr = stub_ptr;
		return d;
	}

	template <R (*TMethod)(A...)>
	static R FunctionStub(void* obj, A... args)
	{
		return (TMethod)(args...);
	}

	template <class T, R (T::*TMethod)(A...)>
	static R MethodStub(void* object_ptr, A... args)
	{
		T* p = static_cast<T*>(object_ptr);
		return (p->*TMethod)(args...);
	}

	template <class T, R (T::*TMethod)(A...) const>
	static R ConstMethodStub(void* object_ptr, A... args)
	{
		T const* p = static_cast<T*>(object_ptr);
		return (p->*TMethod)(args...);
	}

};

}
