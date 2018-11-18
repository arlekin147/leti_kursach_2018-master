#ifndef SEQUENCE_OPS_CPP
#define SEQUENCE_OPS_CPP

#include <algorithm>
#include <unordered_map>
#include <list>

#include "Sequence.cpp"
#include "algorithm.cpp"


template<class T>
Sequence<T> Sequence<T>::erase(const Sequence<T>& d1, size_t pos1 , size_t pos2){
	Sequence<T> result = d1;

	size_t curPos = 0;
	for(auto it = result.seq.begin(); it != result.seq.end(); ){
		if(pos1 <= curPos && curPos <= pos2){
			result.safeDeleteFromTree(*it);
			it = result.seq.erase(it);
		}else{
			++it;
		}
		++curPos;
	}

	return result;
}

template<class T>
Sequence<T> Sequence<T>::exclude(const Sequence<T>& d1, const Sequence<T>& d2){
	Sequence<T> result = d1;

	auto positions_of_needles = find_all_needles(d1.begin(), d1.end(), d2.begin(), d2.end());

	size_t index = 0;
	bool wasAnythingDeleted = false;
	size_t positionOfLastDelete = 0;
	auto positions_it = positions_of_needles.begin();
	for(auto it = result.seq.begin(); it != result.seq.end(); ){
		if(positions_it == positions_of_needles.end()){
			break;
		}
		if(*positions_it == index){
			if(!wasAnythingDeleted || index - positionOfLastDelete >= d2.size()){
				for(int i = 0; i < d2.size(); ++i){
					if(*positions_it == index){
						++positions_it;
					}
					result.safeDeleteFromTree(*it);
					it = result.seq.erase(it);
					++index;
				}
				wasAnythingDeleted = true;
				continue;
			}
		}

		++it;
		++index;
	}

	return result;
}

template<class T>
Sequence<T> Sequence<T>::subst(const Sequence<T>& d1, const Sequence<T>& d2, size_t pos){
	Sequence<T> result = Sequence<T>();

	std::set_union(	d1.set.begin(), d1.set.end(),
					d2.set.begin(), d2.set.end(),
					std::inserter(result.set, result.set.end()));

	result.flushTreeMeta();
	result.fixSequenceBySubstOf(d1, d2, pos);

	return result;
}

template<class T>
Sequence<T> Sequence<T>::concat(const Sequence<T>& d1, const Sequence<T>& d2){
	return Sequence<T>::subst(d1, d2, d1.seq.size());
}

template<class T>
Sequence<T> Sequence<T>::mul(const Sequence<T>& d1, size_t times){
	Sequence<T> result = d1;
	for(auto& el : result.set){
		(el.getCountRef())*=times;
	}

	auto tmp_seq = result.seq;
	for(size_t i = 0; i < times; ++i ){
		result.seq.insert(result.seq.end(), tmp_seq.begin(), tmp_seq.end());
	}

	return result;
}

template<class T>
Sequence<T> Sequence<T>::merge(const Sequence<T>& d1, const Sequence<T>& d2){
	Sequence<T> result = Sequence<T>();

	TreeIterator it1 = d1.set.begin();
	TreeIterator it2 = d2.set.begin();
	std::list<ElementWrapper<T>> cache;

	while(it1 != d1.set.end() || it2 != d2.set.end()){
		TreeIterator* max = nullptr;
		if(it1 == d1.set.end()){
			max = &it2;
		}else if(it2 == d2.set.end()){
			max = &it1;
		}else {
			max = &(*it1 < *it2 ? it1 : it2);
		}

		if(!cache.empty() && static_cast<const T&>(cache.back()) == T(**max)){
				cache.back().getCountRef()+=(*max)->getCountRef();
		}else{
			cache.push_back(ElementWrapper<T>(T(**max), (*max)->getCountRef(), cache.size()));
		}

		++*max;
	}

	result.set = Sequence<T>::TreeClass(cache.begin(), cache.end());

	//Here result.set is formed, and result.seq is empty

	for(auto it = result.set.begin(); it != result.set.end(); ++it){
		for(int i = 0; i < (it)->getCountRef(); ++i){
			result.seq.push_back(it);
		}
	}

	return result;
}

template<class T>
Sequence<T> Sequence<T>::change(const Sequence<T>& d1, const Sequence<T>& d2, size_t pos){
	Sequence<T> result = d1;


	result = Sequence<T>::erase(result, pos, pos+d2.seq.size()-1);
	result = Sequence<T>::subst(result, d2, pos);

	return result;
}


#endif
