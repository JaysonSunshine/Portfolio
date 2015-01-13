#include <string>
#include <iostream>
#include "Sequence.h"
using namespace std;

Sequence::Sequence(const Sequence& other)//copy constructor (initialization
								   //of new Sequence from existing Sequence
{
		m_size = other.m_size;

		if(other.size() != 0) //if rhs isn't empty
		{
			Node* p;
			p = other.head; //p points to first node in rhs
			Node* newNode; //pointer of new nodes added to lhs
			newNode = new Node; //first node of lhs
			newNode->m_value = p->m_value; //fird node of lhs gets its value
			head = newNode; //head of lhs is assined
			newNode->m_prev = NULL;
			int counter = 1;
			Node* q;
			q = newNode; //q keeps track of last node added, so new node can
						 //be connected to growing linked list
			while (counter < other.size()) //loops through rest of rhs
			{
				newNode = new Node;
				p = p->m_next; //moves to next value of rhs
				newNode->m_value = p->m_value;
				q->m_next = newNode; //previous node is linked to newest node
				newNode->m_prev = q;
				q = q->m_next; //q moves forward rhs' growing linked list
				counter ++;
			}
			q->m_next = NULL; //final values of new Sequence object are created
			tail = q;
		}
		else //if rhs is empty, left becomes emtpy
		{
			head = NULL;
			tail = NULL;
		}
	}

bool Sequence::insert(int pos, const ItemType& value) //inserts new node into
	//linked list at pos with value
{
	if(pos < 0 || pos > size()) //won't insert if out of bounds of linked list
		return false;

	Node* newNode;
	newNode = new Node;
	newNode->m_value = value;

	if(head == NULL) //empty list
	{
		head = newNode;
		tail = newNode;
		newNode->m_next = NULL;
		newNode->m_prev = NULL;

		m_size ++;
		return true;
	}
	if(pos == 0)//insertion at beginning of nonempy list
	{
		newNode->m_next = head;
		newNode->m_prev = NULL;
		head->m_prev = newNode;
		head = newNode;

		m_size ++;
		return true;
	}
	if(pos == size())//insertion at end of nonempty list
	{
		newNode->m_prev = tail;
		newNode->m_next = NULL;
		tail->m_next = newNode;
		tail = newNode;

		m_size ++;
		return true;
	}
	else//insertion in middle of nonempty list
	{
		Node* p;
		p = findpointer(head, pos - 1); //provides a pointer to node before where new node
										//will be added
		
		newNode->m_next = p->m_next;
		newNode->m_prev = p;
		newNode->m_next->m_prev = newNode;
		p->m_next = newNode;

		m_size ++;
		return true;
	}
}
      // Insert value into the sequence so that it becomes the item at
      // position pos.  The original item at position pos and those that
      // follow it end up at positions one higher than they were at before.
      // Return true if 0 <= pos <= size() and the value could be
      // inserted.  (It might not be, if the sequence has a fixed capacity,
      // e.g., because it's implemented using a fixed-size array.)  Otherwise,
      // leave the sequence unchanged and return false.  Notice that
      // if pos is equal to size(), the value is inserted at the end.

bool Sequence::insert(const ItemType& value) //insert at location in list based off value
{
	Node* p;
	p = head;
	int counter = 0;

		while(p != NULL && value > p->m_value)
		{
			p = p->m_next;
			counter ++;
		}
		insert(counter, value);

	return true;
}
      // Let p be the smallest integer such that value <= the item at
      // position p in the sequence; if no such item exists (i.e.,
      // value > all items in the sequence), let p be size().  Insert
      // value into the sequence so that it becomes the item at position
      // p.  The original item at position p and those that follow it end
      // up at positions one higher than before.  Return true if the value
      // was actually inserted.  Return false if the value was not inserted
      // (perhaps because the sequence has a fixed capacity and is full).
     
bool Sequence::erase(int pos) //deletes a node at pos
{
	if(head == NULL || pos < 0 || pos >= size())//empty list
		return false;
	if(pos == 0)//deleting from head
	{
		head = head->m_next;
		delete head->m_prev;
		head->m_prev = NULL;
	}
	if(pos == size() - 1)//deleting last entry
	{
		tail = tail->m_prev;
		delete tail->m_next;
		tail->m_next = NULL;
	}
	else//deleting from middle of nonempty list
	{
		Node* p;
		p = findpointer(head, pos);
		p->m_prev->m_next = p->m_next;
		p->m_next->m_prev = p->m_prev;
		delete p;
	}

	m_size --;
	return false;
}
      // If 0 <= pos < size(), remove the item at position pos from
      // the sequence (so that all items that followed this item end up at
      // positions one lower than they were at before), and return true.
      // Otherwise, leave the sequence unchanged and return false.
     
int Sequence::remove(const ItemType& value)
{
	int i = 0;
	int p = find(value);
	
	while(p != -1) //searches through linked list until all nodes containing value are erased
	{
		erase(p);
		i++;
		p = find(value);
	}

	return i;
}
      // Erase all items from the sequence that == value.  Return the
      // number of items removed (which will be 0 if no item == value).

bool Sequence::get(int pos, ItemType& value) const
{
	if(0 <= pos && pos < size())
	{
		Node* p;
		p = findpointer(head, pos);
		value = p->m_value;
		return true;
	}
	return false;
}
      // If 0 <= pos < size(), copy into value the item at position pos
      // in the sequence and return true.  Otherwise, leave value unchanged
      // and return false.

bool Sequence::set(int pos, const ItemType& value)
{
	if(0 <= pos && pos < size())
	{
		//Node* p;
		//p = findpointer(head, pos); WON'T WORK SINCE FINDPOINTER IS CONST. WHY IMPORTANT?
		int counter = 0;
		Node* p;
		p = head;
		while(counter < pos)
		{
			p = p->m_next;
			counter ++;
		}
		p->m_value = value;
		return true;
	}
	return false;
}
      // If 0 <= pos < size(), replace the item at position pos in the
      // sequence with value and return true.  Otherwise, leave the sequence
      // unchanged and return false.

int Sequence::find(const ItemType& value)
{
	int counter = 0;
	Node* p = head;

	while(p != NULL)
	{
		if(p->m_value == value)
			return counter;
		counter++;
		p = p->m_next;
	} 

	return -1;
}
      // Let p be the smallest integer such that value == the item at
      // position p in the sequence; if no such item exists, let p be -1.
      // Return p.

void Sequence::swap(Sequence& other)
{
	if( this != &other)
	{
		int buffer;
		buffer = m_size;//m_size is swapped
		m_size = other.m_size;
		other.m_size = buffer;

		Node* bufferpoint;
		bufferpoint = head;//head is swapped
		head = other.head;
		other.head = bufferpoint;

		bufferpoint = tail;//tail is swapped
		tail = other.tail;
		other.tail = bufferpoint;
	}
}
      // Exchange the contents of this sequence with the other one.

Sequence::Node* Sequence::findpointer(Node* head, int pos) const//returns a pointer to node in link list at
	//pos.  WAY TO NOT PASS HEAD, EG VIA THIS?
{
	int counter = 0;
	Node* p;
	p = head;
	while(counter < pos)
		{
			p = p->m_next;
			counter ++;
		}

	return p;
}

int subsequence(const Sequence& seq1, const Sequence& seq2)//returns the location (if it exists) in seq1 that begins a
	//copy of the values in seq2, i.e., if seq2 is a subsequence of seq1
{
	if(seq2.empty() || seq1.empty() || seq1.size() < seq2.size()) //boundary conditions
		return -1;
	
	Sequence buffer = seq1;
	
	int k;
	int counter = 0;
	ItemType value, value1, value2;
	seq2.get(0, value);
	k = buffer.find(value);
	while(k + seq2.size() <= buffer.size()) //only runs while se2 could fit in seq1 at value in seq1
		//that matches first value of seq2
	{
		if(k == -1)
			return -1;
		int i = 1;
		buffer.get(k + i, value1);
		seq2.get(i, value2);
		while(i < seq2.size() && value1 == value2)
		{
			i++;
			if(i < seq2.size())//prevens out of bounds error
			{
				buffer.get(k + i, value1);
				seq2.get(i, value2);
			}
		}
		
		if(i == seq2.size())
			return k + counter;
		
		counter += k + 1; //keeps track of how many nodes from buffer we've erased from buffer
		for(i = 0; i <= k; i++)
			buffer.erase(0); //truncates buffer to allow for new search

		k = buffer.find(value);
	}
	return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)//mixes two Sequences to make new Sequence
	//eg: result,0 = seq1,0; result,1 = seq2, 0; result,2 = seq1,1, etc.
{
	Sequence temp; //use of temp avoids aliasing problem

	if(seq1.empty())//cases when at least one of the Sequences is empty
	{	
		if(seq2.empty())
			result.swap(temp);
			return;
		temp = seq2;
		result.swap(temp);
		return;
	}
	if(seq2.empty())
	{	
		if(seq1.empty())
			result.swap(temp);
			return;
		temp = seq1;
		result.swap(temp);
		return;
	}

	ItemType value;
	int sizeCounter = 0;
	int insertCounter = 0;
	if(seq1.size() >= seq2.size())  //creation of interleaved Sequence from two non-empty Sequences
	{			
		while(sizeCounter < seq2.size())
		{
			seq1.get(sizeCounter, value);
			temp.insert(insertCounter, value);
			insertCounter++;
			seq2.get(sizeCounter, value);
			temp.insert(insertCounter, value);
			insertCounter++;
			sizeCounter ++;
		}
		while(sizeCounter <seq1.size())
		{
			seq1.get(sizeCounter, value);
			temp.insert(insertCounter, value);
			insertCounter++;
			sizeCounter++;
		}
	}
	if(seq1.size() < seq2.size())
	{			
		while(sizeCounter < seq1.size())
		{
			seq1.get(sizeCounter, value);
			temp.insert(insertCounter, value);
			insertCounter++;
			seq2.get(sizeCounter, value);
			temp.insert(insertCounter, value);
			insertCounter++;
			sizeCounter ++;
		}
		while(sizeCounter <seq2.size())
		{
			seq2.get(sizeCounter, value);
			temp.insert(insertCounter, value);
			insertCounter++;
			sizeCounter++;
		}
	}
	result.swap(temp);
	return;
}