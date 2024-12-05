#pragma once
template<class T>
class Singleton
{
protected:
	Singleton() {};
	virtual ~Singleton() {};

public:
	static T& Instance()
	{
		static T instance;
		return instance;
	}

protected:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
};