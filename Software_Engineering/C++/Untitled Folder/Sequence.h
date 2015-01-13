#include <string> // IS IT OKAY TO ADD LIBRARY FILES TO A HEADER FILE
	//NULL WAS UNDEFINED IF I DIDN'T INCLUDE ANY LIBRARY FILES.  DO YOU NEED A LIBRARY FILE FOR 'USING NAMESPACE STD;?
using namespace std;

#ifndef SequenceClass
#define SequenceClass

typedef unsigned long ItemType; //INSIDE CLASS?
//typedef std::string ItemType;


class Sequence
{
  public:
	 Sequence()// Create an empty sequence.
		: head(NULL), tail(NULL), m_size(0)
	 {}
	~Sequence() //destructor SHOULD RETURN EMPTY SEQUENCE?
	{
		Node* p;
		while(head != NULL)
		{
			p = head;
			head = p->m_next;
			delete p;
		}
		tail = NULL;
		m_size = 0;
	}
	Sequence(const Sequence& other); //copy constructor, other is the existing sequence
	Sequence& operator=(const Sequence& rhs) 
	{
		if (this != &rhs) //only an optimization
		{
			Sequence temp(rhs);
			swap(temp);
		}

		return *this; //WHAT SHOULD BE RETURNED
	}
	bool empty() const;  // Return true if the sequence is empty, otherwise false.
	int size() const;    // Return the number of items in the sequence.
	bool insert(int pos, const ItemType& value);
      // Insert value into the sequence so that it becomes the item at
      // position pos.  The original item at position pos and those that
      // follow it end up at positions one higher than they were at before.
      // Return true if 0 <= pos <= size() and the value could be
      // inserted.  (It might not be, if the sequence has a fixed capacity,
      // e.g., because it's implemented using a fixed-size array.)  Otherwise,
      // leave the sequence unchanged and return false.  Notice that
      // if pos is equal to size(), the value is inserted at the end.

    bool insert(const ItemType& value);
      // et p be the smallest integer such that value <= the item at
      // position p in the sequence; if no such item exists (i.e.,
      // value > all items in the sequence), let p be size().  Insert
      // value into the sequence so that it becomes the item at position
      // p.  The original item at position p and those that follow it end
      // up at positions one higher than before.  Return true if the value
      // was actually inserted.  Return false if the value was not inserted
      // (perhaps because the sequence has a fixed capacity and is full).
     
    bool erase(int pos);
      // If 0 <= pos < size(), remove the item at position pos from
      // the sequence (so that all items that followed this item end up at
      // positions one lower than they were at before), and return true.
      // Otherwise, leave the sequence unchanged and return false.
     
    int remove(const ItemType& value);
      // Erase all items from the sequence that == value.  Return the
      // number of items removed (which will be 0 if no item == value).

    bool get(int pos, ItemType& value) const;
      // If 0 <= pos < size(), copy into value the item at position pos
      // in the sequence and return true.  Otherwise, leave value unchanged
      // and return false.

    bool set(int pos, const ItemType& value);
      // If 0 <= pos < size(), replace the item at position pos in the
      // sequence with value and return true.  Otherwise, leave the sequence
      // unchanged and return false.

    int find(const ItemType& value);
      // Let p be the smallest integer such that value == the item at
      // position p in the sequence; if no such item exists, let p be -1.
      // Return p.

    void swap(Sequence& other);
      // Exchange the contents of this sequence with the other one.
private:
	struct Node//WHAT ABOUT INITIALIZATION OF THIS?
	{
		ItemType m_value;
		Node* m_next;
		Node* m_prev;
	};
	Node* head;
	Node* tail;
	int m_size;
	Node* findpointer(Node* p, int pos) const; //returns pointer to node in linked list at pos
};

inline
bool Sequence::empty() const  // Return true if the sequence is empty, otherwise false.
{
	return size() == 0;
}

inline
int Sequence::size() const  // Returns size of Sequence
{
	return m_size;
}

int subsequence(const Sequence& seq1, const Sequence& seq2);//returns the location (if it exists) in seq1 that begins a
	//copy of the values in seq2, i.e., if seq2 is a subsequence of seq1

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result); //mixes two Sequences to make new Sequence
	//eg: result,0 = seq1,0; result,1 = seq2, 0; result,2 = seq1,1, etc.

#endif