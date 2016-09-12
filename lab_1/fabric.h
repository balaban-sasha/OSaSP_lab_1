#pragma once
#include <map>

template<class ID, class Base, class ... Args> class GenericObjectFactory {
private:
	typedef Base* (*fInstantiator)(Args ...);
	template<class Derived> static Base* instantiator(Args ... args) {
		return new Derived(args ...);
	}
	std::map<ID, fInstantiator> classes;

public:
	GenericObjectFactory() {}
	template<class Derived> void add(ID id) {
		classes[id] = &instantiator<Derived>;
	}
	fInstantiator get(ID id) {
		return classes[id];
	}

};