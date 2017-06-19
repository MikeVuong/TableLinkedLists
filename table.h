#include <string>
#include <utility>
#include "a1.h"
using namespace std;

template <class TYPE>
class Table {
public:
	Table() {}
	virtual bool update(const string& key, const TYPE& value) = 0;
	virtual bool remove(const string& key) = 0;
	virtual bool find(const string& key, TYPE& value) = 0;
	virtual ~Table() {}
};

template <class TYPE>
class SimpleTable :public Table<TYPE> {

	struct Record {
		TYPE data_;
		string key_;
		Record(const string& key, const TYPE& data) {
			key_ = key;
			data_ = data;
		}

	};

	Record** records_;   //the table
	int max_;           //capacity of the array
	int size_;          //current number of records held
	int search(const string& key);
	void sort();
	void grow();
public:
	SimpleTable(int maxExpected);
	SimpleTable(const SimpleTable& other);
	SimpleTable(SimpleTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const SimpleTable& operator=(const SimpleTable& other);
	virtual const SimpleTable& operator=(SimpleTable&& other);
	virtual ~SimpleTable();
};


//returns index of where key is found.
template <class TYPE>
int SimpleTable<TYPE>::search(const string& key) {
	
	int rc = -1;
	//int i = 0;
	
	/*
	while (rc = -1 || i < size_)
	{
		if (records_[i]->key_ == key) {
			rc = i;
		}
		i++;
	}
	*/
	/*
	for (int i = 0; i<size_; i++) {
		if (records_[i]->key_ == key) {
			rc = i;
			return rc;
		}
	}
	
	return rc;
	*/

	int low = 0;
	int high = size_ - 1;
	int mid;
	while (low <= high && rc == -1)
	{
		mid = (low + high) / 2;
		if (records_[mid]->key_ > key)
			high = mid - 1;
		else if (records_[mid]->key_ < key)
			low = mid + 1;
		else
			rc = mid;
	}/*while*/
	return rc;
	
}
//sort the according to key in table
template <class TYPE>
void SimpleTable<TYPE>::sort() {
	int minIdx = 0;
	for (int i = 0; i<size_; i++) {
		minIdx = i;
		for (int j = i + 1; j<size_; j++) {
			if (records_[j]->key_ < records_[minIdx]->key_) {
				minIdx = j;
			}
		}
		Record* tmp = records_[i];
		records_[i] = records_[minIdx];
		records_[minIdx] = tmp;
	}
}

//grow the array by one element
template <class TYPE>
void SimpleTable<TYPE>::grow() {
	Record** newArray = new Record*[max_ + 1];
	max_ = max_ + 1;
	for (int i = 0; i<size_; i++) {
		newArray[i] = records_[i];
	}
	delete[] records_;
	records_ = newArray;
}

/* none of the code in the function definitions below are correct.  You can replace what you need
*/
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(int maxExpected) : Table<TYPE>() {
	records_ = new Record*[maxExpected];
	max_ = maxExpected;
	size_ = 0;
}

template <class TYPE>
SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& other) {
	records_ = new Record*[other.max_];
	max_ = other.max_;
	size_ = 0;
	for (int i = 0; i<other.size_; i++) {
		update(other.records_[i]->key_, other.records_[i]->data_);
	}
}
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& other) {
	size_ = other.size_;
	max_ = other.max_;
	records_ = other.records_;
	other.records_ = nullptr;
	other.size_ = 0;
	other.max_ = 0;
}

template <class TYPE>
bool SimpleTable<TYPE>::update(const string& key, const TYPE& value) {
	int idx = search(key);
	if (idx == -1) {
		if (size_ == max_) {
			grow();
		}
		
		records_[size_++] = new Record(key, value);
		
		sort();
	}
	else {
		records_[idx]->data_ = value;
	}
	return true;
}

template <class TYPE>
bool SimpleTable<TYPE>::remove(const string& key) {
	int idx = search(key);
	if (idx != -1) {
		delete records_[idx];
		for (int i = idx; i<size_ - 1; i++) {
			records_[i] = records_[i + 1];
		}
		size_--;
		return true;
	}
	else {
		return false;
	}
}

template <class TYPE>
bool SimpleTable<TYPE>::find(const string& key, TYPE& value) {
	int idx = search(key);
	if (idx == -1)
		return false;
	else {
		value = records_[idx]->data_;
		return true;
	}
}

template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& other) {
	if (this != &other) {
		if (records_) {
			int sz = size_;
			for (int i = 0; i<sz; i++) {
				remove(records_[0]->key_);
			}
			delete[] records_;
		}
		records_ = new Record*[other.max_];
		max_ = other.max_;
		size_ = 0;
		for (int i = 0; i<other.size_; i++) {
			update(other.records_[i]->key_, other.records_[i]->data_);
		}

	}
	return *this;
}
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& other) {
	swap(records_, other.records_);
	swap(size_, other.size_);
	swap(max_, other.max_);
	return *this;
}
template <class TYPE>
SimpleTable<TYPE>::~SimpleTable() {
	if (records_) {
		int sz = size_;
		for (int i = 0; i<sz; i++) {
			remove(records_[0]->key_);
		}
		delete[] records_;
	}
}

template <class TYPE>
class HashTable :public Table<TYPE> {

	struct HashRecord
	{
		string key_;
		TYPE value_;

		HashRecord(const string& k = "", const TYPE& t = {})
		{
			key_ = k;
			value_ = t;
		}
		bool operator == (const HashRecord& tmp)
		{
			if (this->key_ == tmp.key_)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

	};

	/*Chaining / Array of linked lists */
	DList<HashRecord>* table_;
	HashRecord* rec;
	int tableSize_;



	/*Bucketing*/
	//	Record** table_;
	//	int* bucketsUsed_;

	/*Linear Probing*/
	//Method 1:
	//	Record* table;
	//	bool isempty_;


public:
	/*Chaining*/
	//	table_ = new DList<Record>[maxExpected];

	/*Bucketing*/
	//	table_ = new Record*[maxExpected];
	//	bucketsUsed_ = new int[maxExpected];
	//	for(int i = 0; i < maxExpected; i++)
	//	{
	//		table_[i] = new Record[10];
	//		bucketsUsed_[i] = 0;
	//	}
	HashTable(int maxExpected);
	HashTable(const HashTable& other);
	HashTable(HashTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const HashTable& operator=(const HashTable& other);
	virtual const HashTable& operator=(HashTable&& other);
	virtual ~HashTable();

};
/* none of the code in the function definitions below are correct.  You can replace what you need
*/


template <class TYPE>
HashTable<TYPE>::HashTable(int maxExpected) : Table<TYPE>() {
	table_ = new DList<HashRecord>[maxExpected];
	tableSize_ = maxExpected;
}

template <class TYPE>
HashTable<TYPE>::HashTable(const HashTable<TYPE>& other) {
	
	/*
	table_ = new DList<HashRecord>[other.tableSize_];
	for (int i = 0; i < other.tableSize_; i++)
	{
		table_[i] = other.table_[i];
	}
	*/
	table_ = new DList<HashRecord>[other.tableSize_];
	tableSize_ = other.tableSize_;
	
	for (int i = 0; i < other.tableSize_; i++)
	{
		if (!other.table_[i].empty())
		{
			table_[i] = other.table_[i];
		}
	}
}

template <class TYPE>
bool HashTable<TYPE>::update(const string& key, const TYPE& value) {

	/*
	size_t coll;
	std::hash<std::string> myhash;
	

	HashRecord rec(key);
	coll = myhash(key) % tableSize_;
	auto end = table_[coll].end();
	typename DList<HashRecord>::iterator iter = table_[coll].search(rec);
	if (iter != end)
	{
		table_[coll].erase(iter);
		table_[coll].push_back(rec);
		return true;
	}
	

	*/

	size_t coll;
	std::hash<std::string> myhash;

	HashRecord rec(key, value);
	coll = myhash(key) % tableSize_;

	typename DList<HashRecord>::iterator iter = table_[coll].search(rec);
	HashRecord temp = *iter;
	if (table_[coll].empty())
	{
		table_[coll].push_back(rec);
	}
	else
	{
		if (temp.key_ == key)
		{
			table_[coll].erase(iter);
			table_[coll].push_back(rec);
		}
		else
		{
			table_[coll].push_back(rec);
		}
	}
	
	return true;
	
//	HashRecord temp = *iter;
	

}

template <class TYPE>
bool HashTable<TYPE>::remove(const string& key) {

	size_t coll;
	std::hash<std::string> myhash;

	coll = myhash(key) % tableSize_;
	//auto iter = table_[coll].begin();
	auto end = table_[coll].end();
	//auto iter = table_[coll].search(rec);
	HashRecord rec(key);
	auto iter = table_[coll].search(rec);
	/*
	if (!table_[coll].key.empty())
	{

		table_[coll].erase(iter);
		return true;
	}
	
	return false;
	*/
	/*
	if (!iter!= nullptr)
	{
		table_[coll].erase(iter);
		return true;
	}
	else
	{
		return false;
	}
	*/

	if (iter != end)
	{
		table_[coll].erase(iter);
		return true;
	}
	else
	{
		return false;
	}


}

template <class TYPE>
bool HashTable<TYPE>::find(const string& key, TYPE& value) {

	size_t coll;
	std::hash<std::string> myhash;
	coll = myhash(key) % tableSize_;
	auto iter = table_[coll].begin();
	auto end = table_[coll].end();
	HashRecord rec(key);
	iter = table_[coll].search(rec);
	rec = *iter;

	if (iter != end)
	{
		//*iter = value
		value = rec.value_;
		return true;
	}
	else
	{
		return false;
	}
}


template <class TYPE>
const HashTable<TYPE>& HashTable<TYPE>::operator=(const HashTable<TYPE>& other)
{
	
	if (this != &other)
	{
		delete[] table_;
	}

	table_ = new DList<HashRecord>[other.tableSize_];
	tableSize_ = other.tableSize_;

	for (int i = 0; i < other.tableSize_; i++)
	{
		if (!other.table_[i].empty())
		{
			table_[i] = other.table_[i];
		}
	}

	return *this;
}



template <class TYPE>
const HashTable<TYPE>& HashTable<TYPE>::operator=(HashTable<TYPE>&& other) {

	if (this != &other)
	{
		delete[] table_;
	}
	table_ = std::move(other.table_);
	rec = std::move(other.rec);
    tableSize_ = std::move(other.tableSize_);

	other.table_ = nullptr;
	other.rec = nullptr;
	other.tableSize_ = 0;
	//table_ = new DList<HashRecord>[other.tableSize_];

	return *this;

}
template <class TYPE>
HashTable<TYPE>::~HashTable() {
	delete[] table_;
}