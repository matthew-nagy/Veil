#ifndef BINARY_LIST_H

#define BINARY_LIST_H 0
#ifndef _VECTOR_
#include <vector>
#endif


template <class T>
class Binary_List{
public:

	inline short unsigned size(void)const;		//Returns variable size

	inline bool contains(T t);					//Returns true if t is in the list, otherwise false

	short getIndex(T t);						//Returns the index of an element, or -1 if it doesn't exist

	void push_back(T t);						//Adds t to the list

	inline void reserve(size_t space);			//Reserves the space in list

	inline void shrink_to_fit(void);			//Calls vector::shrink_to_fit

	T pop_front(void);							//Returns the first member of the list

	T pop_back(void);							//Returns the last member of list

	T pop_element(short unsigned index);		//Returns the T at that point in list

	inline T Binary_List<T>::operator[](short unsigned index);			//Returns the index'th element of list

	T Binary_List<T>::operator()(short unsigned index, bool& allowed);	//Returns the index'th element, or the first and makes allowed false

private:
	short unsigned _size = 0;					//Stores the current size of the list

	std::vector<T> list;						//Stores all of the variables in size order
};



template<class T>
inline short unsigned Binary_List<T>::size(void)const{
	return _size;
}

template<class T>
inline bool Binary_List<T>::contains(T t){
	return getIndex(t) != -1;
}

template<class T>
short Binary_List<T>::getIndex(T t){
	if (_size == 0){
		return -1;
	}

	size_t position = _size / 2;
	short unsigned step = position / 2;
	while (step > 0){

		if (list[position] == t){
			step = 0;
		}
		else if (list[position] > t){
			position -= step;
		}
		else{
			position += step;
		}

		step /= 2;
	}

	if (list[position] == t){
		return position;
	}

	if (list[position] > t){
		do{
			if (t == list[position]){
				return position;
			}
			else if (t > list[position]){//it can't exist in the list if it hasn't found it yet
				return -1;
			}
			position -= 1;

		} while (position > 0);
	}
	else{
		while (position < _size){
			if (t == list[position]){
				return position;
			}
			else if (list[position] > t){
				return -1;
			}
			position += 1;
		}
	}

	//It will only get here if the counter gets out of bounds
	return -1;
}

template <class T>
void Binary_List<T>::push_back(T t){
	if (_size == 0){
		list.push_back(t);
		_size += 1;
		return;
	}
	size_t position = _size / 2;
	short unsigned step = position / 2;
	while (step > 0){

		if (list[position] == t){
			step = 0;
		}
		else if (list[position] > t){
			position -= step;
		}
		else{
			position += step;
		}

		step /= 2;
	}

	if (list[position] > t){
		while (list[position] > t){
			position -= 1;
			if (position == -1){
				break;
			}
		}
		position += 1;
		list.insert(list.begin() + position, t);
	}
	else{
		if (position + 1 == _size){
			list.push_back(t);
		}
		else{
			while (position != _size){
				if (list[position] >= t){ break; }
				position += 1;
			}
			if (position == _size){
				list.push_back(t);
			}
			else{
				list.insert(list.begin() + position, t);
			}
		}
	}

	_size += 1;

}

template<class T>
inline void Binary_List<T>::reserve(size_t space){
	list.reserve(space);
}

template<class T>
inline void Binary_List<T>::shrink_to_fit(void){
	list.shrink_to_fit();
}

template<class T>
T Binary_List<T>::pop_front(void){
	T toret = list[0];
	list.erase(list.begin());
	_size -= 1;
	return toret;
}

template<class T>
T Binary_List<T>::pop_back(void){
	T toret = list[_size - 1];
	list.pop_back();
	_size -= 1;
	return toret;
}

template<class T>
T Binary_List<T>::pop_element(short unsigned index){
	T toret = list[index];
	list.erase(index);
	_size -= 1;
	return toret;
}

template<class T>
inline T Binary_List<T>::operator[](short unsigned index){
	return list[index];
}

template <class T>
T Binary_List<T>::operator()(short unsigned index, bool& allowed){
	if (index < 0 || index >= _size){
		allowed = false;
		return list[0];
	}
	allowed = true;
	return list[index];
}

#endif