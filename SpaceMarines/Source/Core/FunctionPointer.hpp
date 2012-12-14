#pragma once

namespace SpaceMarines
{

template <typename returnType, typename argType>
class FunctionPointer
{
public:
	FunctionPointer(returnType (*function) (argType))
	{
		this->function = function;
	}

	returnType run(argType arg)
	{
		return function(arg);
	}
private:
	returnType (*function) (argType);

};

template<typename returnType>
class FunctionPointer<returnType, void>
{
public:
	FunctionPointer(returnType (*function) ())
	{
		this->function = function;
	}

	returnType run()
	{
		return function();
	}

private:
	returnType (*function) ();
};

template<>
class FunctionPointer<void, void>
{
public:
	FunctionPointer(void (*function) ())
	{
		this->function = function;
	}

	void run()
	{
		function();
	}

private:
	void (*function) ();
};

template<typename returnType, typename argType>
FunctionPointer<returnType, argType> MakeFunctionPointer(returnType (*function)(argType))
{
	return	FunctionPointer<returnType, argType>(function);
}

template<typename argType>
FunctionPointer<void, argType> MakeFunctionPointer(void (*function)(argType))
{
	return	FunctionPointer<void, argType>(function);
}

template<typename returnType>
FunctionPointer<returnType, void> MakeFunctionPointer(returnType (*function)())
{
	return	FunctionPointer<returnType, void>(function);
}

template<>
FunctionPointer<void, void> MakeFunctionPointer(void (*function)())
{
	return	FunctionPointer<void, void>(function);
}


}
