#ifndef MY_PAIR_CPP
#define MY_PAIR_CPP

#include <utility>
#include <memory>

template<class T>
class ElementWrapper{
/**
*	We need @countRef to be stored by uniquePtr, cuz
*	some STL collections require elements to be immutable,
*	for order to be persistent.
*	@countRef is not used for ordering, but, anyway, we can't change it
*	from const ElementWrapper. But we can change it, if it's stored in
*	dynamic memory! So, we store it there.
*
*	"Local hash" means, T1==T2 => hash(T1)==hash(T2)
*	only if T1 and T2 are both stored in same collection (which must manage
*	such property).
*
*/
public:
	using stored_type = T;

	ElementWrapper(T el, size_t _countRef, size_t _localHash):
	element(el),
	countRef(std::make_unique<size_t>(_countRef)),
	localHash(std::make_unique<size_t>(_localHash))
	{
	}

	ElementWrapper(const ElementWrapper& other)
	{
		this->element = other.element;
		this->countRef = std::make_unique<size_t>(*(other.countRef));
		this->localHash = std::make_unique<size_t>(*(other.localHash));
	}

	ElementWrapper(ElementWrapper&& other)
	{
		this->element = std::move(other.element);
		this->countRef = std::move(other.countRef);
		this->localHash = std::move(other.localHash);
	}

	bool operator==(const ElementWrapper& other) const {
		return this->element == other.element;
	}

	bool operator!=(const ElementWrapper& other) const {
		return this->element != other.element;
	}

	bool operator>(const ElementWrapper& other) const {
		return this->element > other.element;
	}

	bool operator<(const ElementWrapper& other) const {
		return this->element < other.element;
	}

	bool operator>=(const ElementWrapper& other) const {
		return this->element >= other.element;
	}

	bool operator<=(const ElementWrapper& other) const {
		return this->element <= other.element;
	}

	size_t& getCountRef() const{
		return *(this->countRef);
	}

	size_t& getLocalHash() const{
		return *(this->localHash);
	}

	operator const T&() const{
		return this->element;
	}

	operator T&(){
		return this->element;
	}

	operator T(){
		return this->element;
	}

private:
	T element;
	std::unique_ptr<size_t> countRef;
	std::unique_ptr<size_t> localHash;
};

template<class T>
struct ElementWrapperHash {
   size_t operator() (const ElementWrapper<T> &element) const {
     return element.getLocalHash();
   }
};

#endif
