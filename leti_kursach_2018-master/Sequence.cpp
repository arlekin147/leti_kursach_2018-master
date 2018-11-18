#ifndef SEQUENCE_CPP
#define SEQUENCE_CPP

#include <set>
#include <list>
#include <unordered_map>
#include <stack>

#include <utility> //for pairs
#include <iterator> //for std::insert_iterator

#include "SequenceIterator.cpp"
#include "ElementWrapper.cpp"

template<class T>
class Sequence{
public:
	using wrapper = ElementWrapper<T>;
	using wrapper_hash = ElementWrapperHash<T>;
	using TreeClass = typename std::set<wrapper>;
	using TreeIterator = typename TreeClass::iterator;
	using ListClass = typename std::list<TreeIterator>;
	using ChangeTable = std::unordered_map<wrapper, TreeIterator, wrapper_hash>;

	Sequence():
	set(),
	seq(),
	unusedLocalHashes()
	{
	}

	Sequence(const Sequence<T>& other){
		*this = other;
	}

	Sequence(Sequence<T>&& other){
		*this = std::move(other);
	}

	Sequence<T>& operator=(const Sequence<T>& other){
		this->set = other.set;
		this->seq = other.seq;
		this->unusedLocalHashes = other.unusedLocalHashes;

		//Now this->seq elements point to other.set
		//Let's fix it

		ChangeTable renew_table;

		for(auto it = this->set.begin(); it != this->set.end(); ++it){
			//renew_table.insert(std::pair(*it, it)); //C++17 features, babe
			renew_table.insert(std::make_pair(*it, it));
		}

		for(auto it = this->seq.begin(); it != this->seq.end(); ++it){
			*it = renew_table.at(**it);
			//@it is iterator of list, C++ standart guarantee correctness after change
			//of underlying element (and list at all, except deleting underlying element)
		}

		return *this;
	}

	Sequence<T>& operator=(Sequence<T>&& other){
		this->set = std::move(other.set);
		this->seq = std::move(other.seq);
		this->unusedLocalHashes = std::move(other.unusedLocalHashes);

		return *this;
	}

	SequenceIterator<typename ListClass::const_iterator> begin() const{
		return SequenceIterator<typename ListClass::const_iterator>(this->seq.begin());
	}

	SequenceIterator<typename ListClass::const_iterator> end() const{
		return SequenceIterator<typename ListClass::const_iterator>(this->seq.end());
	}

	void insert(T el){
		auto iter = this->safeAddToTree(el);
		this->seq.push_back(iter); //Iterator on inserted element or equal element in set
	}

	size_t size() const {
		return this->seq.size();
	}

	size_t getSetSize() const {
		return this->set.size();
	}

	template <class Generator>
	void getRandomSequence(size_t might, Generator gen){
		Sequence<T> result;
		for(int i = 0; i < might; ++i){
			result.insert(gen());
		}
		std::swap(*this, result);
	}


	/* Operations on sequence*/
	static Sequence<T> erase(const Sequence<T>& d1, size_t pos1 , size_t pos2);
	static Sequence<T> merge(const Sequence<T>& d1, const Sequence<T>& d2);
	static Sequence<T> exclude(const Sequence<T>& d1, const Sequence<T>& d2);
	static Sequence<T> concat(const Sequence<T>& d1, const Sequence<T>& d2);
	static Sequence<T> mul(const Sequence<T>& d1, size_t times);
	static Sequence<T> subst(const Sequence<T>& d1, const Sequence<T>& d2, size_t pos);
	static Sequence<T> change(const Sequence<T>& d1, const Sequence<T>& d2, size_t pos);

	/*Operations on set (we saving order of keeped elements of sequence)*/
	Sequence<T> operator|(const Sequence<T>& other) const;
	Sequence<T> operator&(const Sequence<T>& other) const;
	Sequence<T> operator^(const Sequence<T>& other) const;
	Sequence<T> operator/(const Sequence<T>& other) const;

private:
	 TreeClass set;
	 ListClass seq;
	 std::stack<size_t> unusedLocalHashes;

	 bool safeDeleteFromTree(TreeIterator it){
		 --( it->getCountRef() );
 		if(it->getCountRef() <= 0){
			this->saveUnusedLocalHash(it->getLocalHash());
 			this->set.erase(it);
			return true;
 		}

		return false;
 	}

	TreeIterator safeAddToTree(T el){
		return safeAddToTreeByIterator(this->set.begin(), el);
	}

	TreeIterator safeAddToTreeByIterator(TreeIterator out_it, T el){
		return safeAddToTreeByIterator(out_it, el, 1);
	}

	size_t getNewLocalHash(){
		if(this->unusedLocalHashes.empty()){
			return this->getSetSize();
		}else{
			auto newLocalHash = this->unusedLocalHashes.top();
			this->unusedLocalHashes.pop();
			return newLocalHash;
		}
	}

	void saveUnusedLocalHash(size_t hash){
		this->unusedLocalHashes.push(hash);
	}

	TreeIterator safeAddToTreeByIterator(TreeIterator out_it, T el, size_t count){
		size_t newLocalHash = this->getNewLocalHash();
		auto iterator = this->set.insert(out_it, ElementWrapper(el, 0, newLocalHash));

		if(iterator->getCountRef() != 0){
			this->saveUnusedLocalHash(newLocalHash);
		}

		(iterator->getCountRef()) += count;

		return iterator;
	}

	void flushTreeMeta(){
		size_t newLocalHash = 0;
		for(auto& wrap : this->set){
			//We will set all @countRef's to zero and increase them later
			//While iterating over sequences
			wrap.getCountRef() = 0;

			wrap.getLocalHash() = newLocalHash++;
		}
		while(!this->unusedLocalHashes.empty()){
			this->unusedLocalHashes.pop();
		}
	}

	static ChangeTable generateChangeTable(const Sequence<T>& result, const Sequence<T>& source);

	void fixSequenceBySubstOf(
		const Sequence<T>& source1, const Sequence<T>& source2, size_t pos
	);

	void fixSequenceByConcatOf(
		const Sequence<T>& source1, const Sequence<T>& source2
	){
		this->fixSequenceBySubstOf(
			source1, source2, source1.seq.size()
		);
	}

};

#include "RenewSeqUtils.cpp"
#include "SequenceOperations.cpp"
#include "SetOperations.cpp"

#endif
