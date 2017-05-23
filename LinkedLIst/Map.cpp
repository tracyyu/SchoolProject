#include "Map.h"

#include <iostream>
#include <string>
using namespace std;


// creates an empty map with dummy node
Map::Map()
	:head(new Node), m_pairs(0)		// head points to a dummy Node
{
	head->prev = head;			// pointer points to itself
	head->next = head;			// pointer points to itself
}

// create a new copy of map
Map::Map( const Map& copy)
	:m_pairs(copy.m_pairs), head(new Node)
{
	head->prev = head;						//creates a dummy node 
	head->next = head;	

	if(!copy.empty())
	{	for( Node* p = copy.head->next; p != copy.head; p = p->next)		// insert rest of nodes that does not include the dummy node
		{	
			insert(p->key, p->value);
		}
	}
	else
		;
}

// create a copy of map
Map Map::operator=(const Map& other)
{
    if (this != &other)					// checks if both maps are the same 
    {
        Map temp( other);
        swap(temp);
    }
    return *this;
}

// destructor for the map
Map::~Map()
{
	if(empty())                                                                                                                                                                                                
		delete head;			// delete dummy node
	else{
		Node* d = head->next;
		while(d != head)				// delete the node after the dummy node, until the pointer points to dummy node
		{
			Node* temp = d;
			(d->prev)->next = d->next;			
			(d->next)->prev = d->prev;
						d = d->next;
			delete temp;
		}
		delete head;
	}
}

// returns whether the map is empty
bool Map::empty() const
{
	if(head->prev == head && head->next == head)			// if it points to iself, then it is empty
		return true;
	else 
		return false;
}

// return the size of the map or number of pairs of keys and values
int Map::size() const
{
	return m_pairs;
}


// Insert a key/ value pair at the top of the list
bool Map::insert(const KeyType& key, const ValueType& value)
{
	for( Node* check = head->next; check != head; check = check->next)
	{if( check->key == key) return false;}

	Node* nwptr;
	nwptr = new Node;
	nwptr->key = key;
	nwptr->value = value;

	if(empty())
	{
		head->prev = nwptr;				// dummy node prev points to current new node
		nwptr->prev = head;				// current prev points to dummy node
		head->next = nwptr;				// dummy node next points to current new node
		nwptr->next = head;				// current next points to dummy node
		m_pairs++;
		return true;
	}
	else
	{
		Node* p = head->next;							// points to item after dummy item
		nwptr->next = p;								// new node next points to current pointer
		nwptr->prev = p->prev;							// new node prev points to current pointer prev
		p->prev = nwptr;								// current prev points to new node 
		(nwptr->prev)->next = nwptr;							// new node prev now points to new node
		m_pairs++;
		return true;
	}
	return true;
}

// changes the value of the key/value pair in the map
bool Map::update(const KeyType& key, const ValueType& value)
{
	if(empty())
		return false;
	Node*  k;
	for( k = head->next; k != head && k->key != key; k = k->next)	// loop through until points to key == key or dummy node
		;
	if( k->key == key)
	{	
		k->value = value;			// sets to value at which k points to value 
		return true;
	}
	else 
		return false;
}

// it either update or insert a key depending whether or not it exist in the map
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	Node*  k;
	if( empty())
		insert(key,value);
	for( k = head->next; k != head && k->key != key; k = k->next)
		;
	if( k->key == key)
		return update(key, value);
	else
		return insert(key,value);
	return true;
}

// delete one of item in the list
bool Map:: erase(const KeyType& key)
{
	if(!empty())
	{
		Node* e;
		for( e = head->next; e != head && e->key != key; e = e->next)
			;
		if( e->key == key)
		{										// find key in the list and deletes it
			Node* temp  = e;
			(e->prev)->next = e->next;			// set prev node's next to e's next
			(e->next)->prev = e->prev;			// set next node's prev to e's prev
			delete temp;						// delete the node
			m_pairs--;
			return true;						// subtract size of pairs
		}
		else
			return false;
	}
	else
		return false;
}

// checks whether to key exist in the Map
bool Map::contains(const KeyType& key) const
{
	if( empty())
		return false;
	for( Node* p = head->next; p != head; p = p->next)		// loops thoguth to see if the key is contained in map
	{
		if( p->key == key)
			return true;					// if contains the key, return true
	}
	return false;
}

// get the pair if they key is in the map and set value to value in the map
bool Map::get(const KeyType& key, ValueType& value) const
{
	if( empty())
		return false;
	Node*  k;
	for( k = head->next; k != head && k->key != key; k = k->next)		// looop through the map to find the key pair
		;
	if( k->key == key)
	{	
		value = k->value;					// set the value entered in the function to the value in the map for that pair
		return true;
	}
	else 
		return false;
}

// loops through the map depending on the i, and set key and value to one of the pairs in the map, if any
bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if(empty())
		return false;
	if (i < 0  ||  i >= m_pairs)
        return false;
	Node* p = head->next;
	for(int j = i; p !=  head && j < m_pairs; j++)
	{
		key = p->key;
		value = p->value;
		p = p->next;
	}
	return true;
}

// change to list between both the maps
void Map::swap(Map& other)
{   
	Node* temp = head;
    head = other.head;
    other.head = temp;

	int t = m_pairs;
   m_pairs = other.m_pairs;
   other.m_pairs = t;
}

// combines m1 and m2 pairs based upon if only one pair exist in one Map and identical pairs exist in both maps
bool combine(const Map& m1, const Map& m2, Map& result)
{
	result = m1;							// assignment operator on map result
	KeyType k1 = "";
	ValueType v1 = 0;
	ValueType v2;
	for(int i = 0; i < m2.size(); i++)		// loops through size of second map
	{	
		m2.get(i, k1, v1 );				// compare the key and its value for each 
		if( result.contains(k1))
		{	
			result.get(k1, v2);
			if( v1 == v2)
			{
				result.insert(k1, v1);
				return true;				// return either true if pair match in m1 and m2
			}
			else
				return false;			// or false if value does not match
		}							
		else
			result.insert(k1, v1);		// insert those pairs from m2
	}															
	return true;
}

// erases any pairs that have identical keys in m1 and m2
void subtract(const Map& m1, const Map& m2, Map& result)
{
	Map c = m1;
	KeyType k1 = "";
	ValueType v1 = 0;

	for(int i = 0; i < m2.size(); i++)			// loops through size of second map
	{	
		m2.get(i, k1, v1 );						// get a key from map2
		if( c.contains(k1))			// if this is true, must erase that pair from result, else nothing
			c.erase(k1);						// erase pair from c
	}
		result = c;
}
