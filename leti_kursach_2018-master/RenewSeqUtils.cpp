#ifndef RENEW_SEQ_UTILS_CPP
#define RENEW_SEQ_UTILS_CPP

template<class T>
void Sequence<T>::fixSequenceBySubstOf(
	const Sequence<T>& source1, const Sequence<T>& source2, size_t pos
){
	auto changeTable1 = Sequence<T>::generateChangeTable (*this, source1);
	auto changeTable2 = Sequence<T>::generateChangeTable (*this, source2);

	this->seq.clear();
	auto outIt = std::inserter(this->seq, this->seq.end());

	bool isFirstSeqBeingOperated = true;
	bool end = false;
	auto it1 = source1.seq.begin();
	auto it2 = source2.seq.begin();
	unsigned int curPos = 0;
	while(!end){
		if(curPos == pos){
			isFirstSeqBeingOperated = false;
		}
		if(it2 == source2.seq.end()){
			isFirstSeqBeingOperated = true;
		}
		if(isFirstSeqBeingOperated){
			if(it1 != source1.seq.end()){
				if(changeTable1.find(**it1) != changeTable1.end()){
					auto& tmp = changeTable1.at(**it1);
					tmp->getCountRef() += 1;
					outIt = tmp; //It's writing, not assignment
				}
				++it1;
			} else {
				end = true;
			}
		}
		else{
			if(changeTable2.find(**it2) != changeTable2.end()){
				auto& tmp = changeTable2.at(**it2);
				tmp->getCountRef() += 1;
				outIt = tmp; //It's writing, not assignment
			}
			++it2;
		}
		++curPos;
	}
}


template<class T>
typename Sequence<T>::ChangeTable Sequence<T>::generateChangeTable(
	const Sequence<T>& result, const Sequence<T>& source
){

	Sequence<T>::ChangeTable changeTable;

	auto out_it = result.set.end();
	for(auto it_srs = source.set.begin(), it_res = result.set.begin(); it_srs != source.set.end(); ++it_srs){
		if(it_res == result.set.end()){
			break;
		}
		while(*it_res < *it_srs){
			++it_res;
			if(it_res == result.set.end()){
				break;
			}
		}
		if(*it_res == *it_srs){
			// changeTable.insert(std::pair(*it_srs, it_res)); //C++17, babe ;)
			changeTable.insert(std::make_pair(*it_srs, it_res));
		}
	}

	return changeTable;
}

#endif
