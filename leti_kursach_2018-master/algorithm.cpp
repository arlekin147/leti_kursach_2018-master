#ifndef ALGORITHM_CPP
#define ALGORITHM_CPP

#include <vector>
#include <cstddef>
#include <memory>

template<class T>
std::vector<unsigned int> prefix_function(const std::vector<T>& str){
	if(str.size() <= 0){
		return std::vector<unsigned int>(0);
	}
	auto p = std::vector<unsigned int>(str.size(), 0);
	for(size_t index = 1; index < str.size(); ++index){
		unsigned int p_new = p.at(index-1);
		while(p_new > 0 && str.at(index) != str.at(p_new)){
			p_new = p.at(p_new - 1);
		}

		if(str.at(index) == str.at(p_new)){
			++p_new;
		}

		p[index] = p_new;
	}

	return p;
}

template <class T>
class KmpWrapper{
public:
	KmpWrapper(T n):
	isFake(false),
	val(std::make_unique<T>(n))
	{
	}

	KmpWrapper(std::nullptr_t n):
	isFake(true),
	val(n)
	{
	}

	bool operator==(const KmpWrapper& other) const{
		if(this->isFake){
			return false;
		}
		if(other.isFake){
			return false;
		}
		return *(this->val) == *(other.val);
	}

	bool operator!=(const KmpWrapper& other) const{
		return !(*this == other);
	}

private:
	bool isFake;
	std::unique_ptr<T> val;
};

template<class InIter>
std::vector<size_t> find_all_needles(
	InIter haystak_begin, InIter haystack_end,
	InIter needle_begin, InIter needle_end
){
	using T = typename std::iterator_traits<InIter>::value_type;
	std::vector<KmpWrapper<T>> concat;
	for(auto it = needle_begin; it != needle_end; ++it){
		concat.push_back(KmpWrapper<T>(*it));
	}
	concat.push_back(KmpWrapper<T>(nullptr));
	for(auto it = haystak_begin; it != haystack_end; ++it){
		concat.push_back(KmpWrapper<T>(*it));
	}

	auto prefixes = prefix_function(concat);

	std::vector<size_t> needle_positions;

	size_t needle_size = static_cast<size_t>(std::distance(needle_begin, needle_end));
	size_t index = 0;
	for(auto& pr : prefixes){
		if(pr == needle_size){
			needle_positions.push_back(index - needle_size - 2);
		}
		++index;
	}

	return needle_positions;
}

#endif
