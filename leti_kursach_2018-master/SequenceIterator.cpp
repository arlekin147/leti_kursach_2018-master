#ifndef SEQUENCE_TREE_ITERATOR_CPP
#define SEQUENCE_TREE_ITERATOR_CPP

#include <set>
#include <utility>
#include <iterator>

template<class InIter>
class SequenceIterator{
/**
*	InIter class must be an iterator over iterators, because
*	Sequence class stores sequence as list of iterators to tree
*	with actaual elements.
*/
public:
	using T_wrapper =
		typename std::iterator_traits<typename std::iterator_traits<InIter>::value_type>::value_type;
	using T = typename T_wrapper::stored_type;

	//For std::iterator_traits to refer
	//using value_type = std::remove_cv_t<T>; //C++20 ;)
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using pointer = T*;
	using reference = T&;
	using iterator_category = std::input_iterator_tag;

	SequenceIterator(const InIter _iterator):
	it(_iterator)
	{
	}

	const T& operator*() const {
		return static_cast<const T&>(**it);
	}

	void operator++(){
		++it;
	}

	bool operator!=(const SequenceIterator& other) const {
		return this->it != other.it;
	}

	bool operator==(const SequenceIterator& other) const {
		return this->it == other.it;
	}

private:
	 InIter it;
};

#endif
