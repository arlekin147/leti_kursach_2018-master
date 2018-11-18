#ifndef SET_OPS_CPP
#define SET_OPS_CPP

#include <list>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <iterator>

template<class T>
Sequence<T> Sequence<T>::operator|(const Sequence<T>& other) const{
	return Sequence<T>::concat(*this, other);
}

template<class T>
Sequence<T> Sequence<T>::operator&(const Sequence<T>& other) const{
	Sequence<T> result = Sequence<T>();

	std::set_intersection(	this->set.begin(), this->set.end(),
							other.set.begin(), other.set.end(),
							std::inserter(result.set, result.set.end()));
	result.flushTreeMeta();
	result.fixSequenceByConcatOf(*this, other);

	return result;
}

template<class T>
Sequence<T> Sequence<T>::operator^(const Sequence<T>& other) const{
	Sequence<T> result = Sequence<T>();

	std::set_symmetric_difference(this->set.begin(), this->set.end(), other.set.begin(), other.set.end(),
						std::inserter(result.set, result.set.end()));
	result.flushTreeMeta();
	result.fixSequenceByConcatOf(*this, other);

	return result;
}

template<class T>
Sequence<T> Sequence<T>::operator/(const Sequence<T>& other) const{
	Sequence<T> result;

	std::set_difference(this->set.begin(), this->set.end(), other.set.begin(), other.set.end(),
						std::inserter(result.set, result.set.end()));
	result.flushTreeMeta();
	result.fixSequenceByConcatOf(*this, other);


	return result;
}

#endif
