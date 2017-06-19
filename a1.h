#include <iomanip>
#pragma once
template <typename T>
class DList {
	struct Node {
		T data_;
		Node* next_;
		Node* prev_;
		Node(const T& data = T{}, Node* next = nullptr, Node* prev = nullptr)
		{
			data_ = data;
			next_ = next;
			prev_ = prev;
		}
	};
	Node* head;
	Node* tail;
	int interSize;
public:
	class const_iterator {
		friend class DList;
	protected:
		Node* constIter;
	public:
		const_iterator()
		{
			DList<T>::const_iterator constIter = nullptr;
		}
		const_iterator(Node* c)
		{
			constIter = c;
		}
		bool operator==(const_iterator rhs)
		{
			if (this->constIter == rhs.constIter)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool operator!=(const_iterator rhs)
		{
			if (this->constIter != rhs.constIter)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		const_iterator operator++()
		{
			constIter = constIter->next_;
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator old = *this;
			this->constIter = this->constIter->next_;
			return old;
		}
		const_iterator operator--()
		{
			this->constIter = this->constIter->prev_;
			return *this;
		}
		const_iterator operator--(int)
		{
			const_iterator old = *this;
			this->constIter = this->constIter->prev_;
			return old;
		}
		const T& operator*() const
		{
			return this->constIter->data_;
		}
	};
	class iterator :public const_iterator {
	public:
		iterator() :const_iterator()
		{}
		iterator(Node * c) :const_iterator(c)
		{}
		T& operator*()
		{
			return this->constIter->data_;
		}
		iterator operator++()
		{
			this->constIter = this->constIter->next_;
			return *this;
		}
		iterator operator++(int)
		{
			iterator old = *this;
			this->constIter = this->constIter->next_;
			return old;
		}
		iterator operator--()
		{
			this->constIter = this->constIter->prev_;
			return *this;
		}
		iterator operator--(int)
		{
			iterator old = *this;
			this->constIter = this->constIter->prev_;
			return old;
		}
		/*
		iterator operator == (T& tmp)
		{
		if(this -> data_ == tmp -> data_)
		{
		return true;
		}
		else
		{
		return false;
		}
		}
		iterator operator == (iterator iter)
		{
		if(this == iter)
		{
		return true;
		}
		else
		{
		return false;
		}
		}

		void setVal(T& a)
		{
		data_ -> value_ = a;
		}
		T& getKey()
		{
		return this -> constIter -> data_.key_;
		}

		*/
	};
	DList()
	{
		head = new Node();
		tail = new Node();
		head->next_ = tail;
		tail->prev_ = head;

	}
	iterator begin()
	{
		iterator currentIter = head->next_;
		return currentIter;
	}
	iterator end()
	{
		iterator currentIter = tail;
		return currentIter;
	}
	const_iterator begin() const
	{
		const_iterator constIter = head->next_;
		return constIter;
	}
	const_iterator end() const
	{
		const_iterator constIter = tail;
		return constIter;
	}
	void push_front(const T& data)
	{
		interSize++;
		Node* first = head->next_;
		Node* temp = new Node(data, first, head);
		head->next_ = temp;
		first->prev_ = temp;

	}
	void push_back(const T& data)
	{
		interSize++;
		Node* temp = new Node(data, tail, tail->prev_);
		Node* last = tail->prev_;
		last->next_ = temp;
		tail->prev_ = temp;
	}
	void pop_front()
	{
		if (head->next_ != tail)
		{
			Node* first = head->next_;
			Node* secondFirst = first->next_;
			delete first;
			secondFirst->prev_ = head;
			head->next_ = secondFirst;
			interSize--;
		}
	}

	void pop_back()
	{
		interSize--;
		Node* last = tail->prev_;
		if (last != head)
		{
			Node* secondLast = last->prev_;
			tail->prev_ = secondLast;
			secondLast->next_ = tail;
			delete last;
		}
	}

	iterator insert(iterator loc, const T& data)
	{
		Node* temp = loc.constIter;
		Node* ins = temp->prev_;
		Node* nex;

		if (loc.constIter != nullptr)
		{
			nex = new Node(data, temp, ins);
			ins->next_ = nex;
			temp->prev_ = nex;
			return --loc;
		}


	}

	void erase(iterator it)
	{
		Node* delte = it.constIter;
		Node* prv = delte->prev_;
		Node* nex = delte->next_;
		delete delte;
		prv->next_ = nex;
		nex->prev_ = prv;
	}
	void erase(iterator first, iterator last)
	{
		Node* temp = first.constIter;
		Node* connct = temp->prev_;
		Node* stp = last.constIter;
		Node* n;
		while (temp != stp)
		{
			n = temp->next_;
			delete temp;
			temp = n;
		}

		if (!connct) {
			head->next_ = stp;
			stp->prev_ = head;
		}
		else {
			connct->next_ = stp;
			stp->prev_ = connct;
		}
	}
	const_iterator search(const T& data) const
	{
		const_iterator front = begin();
		//Node* front = head -> next_;
		if (front == end())
		{
			return end();
		}
		else
		{
			while (front != tail)
			{
				if (*front == data)
				{
					return front;
				}
				else
				{
					++front;
				}
			}
		}
		return front;
	}

	iterator search(const T& data)
	{

		iterator front = begin();

		iterator fin = end();
		//Node* front = head -> next_;
		if (front == fin)
		{
			return end();
		}
		else
		{
			while (front != tail)
			{
				if (*front == data)
				{
					return front;
				}
				else
				{
					++front;
				}
			}
		}
		return fin;
	}


	void swap(iterator n1, iterator n2)
	{
		if (n1.constIter != n2.constIter)
		{
		DList:iterator nxt1 = n1.constIter->next_;
			DList::iterator prv1 = n1.constIter->prev_;

			DList::iterator nxt2 = n2.constIter->next_;
			DList::iterator prv2 = n2.constIter->prev_;

			if (n1.constIter->next_ == n2.constIter) //If node are side by side
			{
				prv1.constIter->next_ = n2.constIter;
				n2.constIter->prev_ = prv1.constIter;

				n2.constIter->next_ = n1.constIter;
				n1.constIter->prev_ = n2.constIter;

				n1.constIter->next_ = nxt2.constIter;
				nxt2.constIter->prev_ = n1.constIter;

			}
			else
			{
				n1.constIter->next_ = nxt2.constIter;
				nxt2.constIter->prev_ = n1.constIter;
				n1.constIter->prev_ = prv2.constIter;
				prv2.constIter->next_ = n1.constIter;

				n2.constIter->next_ = nxt1.constIter;
				nxt1.constIter->prev_ = n2.constIter;
				n2.constIter->prev_ = prv1.constIter;
				prv1.constIter->next_ = n2.constIter;

			}
		}
	}

	void qSort()
	{
		DList<T>::iterator a = head->next_;
		DList<T>::iterator b = tail;
		quickSort(a, b, interSize);
	}

	void sortIterative()
	{
		DList<T>::iterator a = head->next_;
		DList<T>::iterator b = tail;
		quickSort(a, b, interSize);
	}


	void quickSort(iterator a, iterator b, int old)
	{

		iterator left = a;  // left side
		iterator right = b; // right side
		iterator piv = a; 	//pivot
		int swapCount = 0;
		iterator curr = piv; //current
		++curr;
		int checkNo = 0;
		int mid = old / 2;

		if (old != 0) //If the previous number isn't 0
		{
			if (left.constIter->next_ != right.constIter)
			{
				for (int i = 0; i < mid; i++)
				{
					piv = piv.constIter->next_;
				}

				swap(a, piv);
				//Splitting it from the right and left of pivot
				left = curr;
				right = piv;
				right = right.constIter->next_;

				while (curr != b && checkNo != old)
				{
					if (*curr< *piv)
					{
						left = curr;
						swap(right, left);
						curr = right;
						right = left;
						++right;
						swapCount++;
					}
					curr = curr.constIter->next_;
					checkNo++;
					if (checkNo == old)
					{
						break;
					}
				}
				if (swapCount != 0)
				{
					swap(piv, left);
				}

				if (swapCount > 0)
				{
					quickSort(left, piv, swapCount);
				}

				quickSort(++piv, curr, checkNo - swapCount);

			}
		}
	}

	bool empty() const
	{
		if (!head ||!tail)
		{
			return true;
		}

		if (head->next_ != tail)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	int size() const
	{
		return interSize;
	}
	~DList()
	{
		if (head != nullptr)
		{
			erase(begin(), end());
		}
		delete head;
		delete tail;
		head = tail = nullptr;
	}
	DList(const DList& rhs)
	{

		const_iterator iter = rhs.begin();
		const_iterator iterEnd = rhs.end();

		if (!empty())
		{
			erase(begin(), end());
		}
		else
		{
			head = new Node();
			tail = new Node();
			head->next_ = tail;
			tail->prev_ = head;
		}
		if (iter != iterEnd)
		{
			if (&rhs != this)
			{
				while (iter != iterEnd)
				{
					push_back(*iter);
					++iter;
				}
			}
		}

		return *this;


	}
	DList& operator=(const DList& rhs)
	{
		const_iterator iter = rhs.begin();
		const_iterator iterEnd = rhs.end();

		if (!empty())
		{
			erase(begin(), end());
		}
		else
		{
			head = new Node();
			tail = new Node();
			head->next_ = tail;
			tail->prev_ = head;
		}
		if (iter != iterEnd)
		{
			if (&rhs != this)
			{
				while (iter != iterEnd)
				{
					push_back(*iter);
					++iter;
				}
			}
		}

		return *this;
	}
	DList(DList&& rhs)
	{
		head = std::move(rhs.head);
		tail = std::move(rhs.tail);
		interSize = std::move(rhs.interSize);

		rhs.head = nullptr;
		rhs.tail = nullptr;
		rhs.interSize = 0;

		return *this;
	}

	DList& operator=(DList&& rhs)
	{

		head = std::move(rhs.head);
		tail = std::move(rhs.tail);
		interSize = std::move(rhs.interSize);

		rhs.head = nullptr;
		rhs.tail = nullptr;
		rhs.interSize = 0;

		return *this;

	}
};