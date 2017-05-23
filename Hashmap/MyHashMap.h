#ifndef MYHASHMAP_INCLUDED
#define MYHASHMAP_INCLUDED

const int DEFAULT_NUM_BUCKETS = 1000000;

#include <string>

// In accordance with the spec, YOU MUST RE-IMPLEMENT THIS CLASS, since
// you are not allowed to use an STL map or unordered_map or hash_map.

template <typename ValueType>
class MyHashMap
{
public:
 MyHashMap(int numBuckets = DEFAULT_NUM_BUCKETS)
     : hashsize(numBuckets), count(0), getNextWord(false), prev(NULL)
    {
		hashtable = new Node*[hashsize]; 	// create an array of Node pointers
		for( int k = 0; k < hashsize; k++)
			(hashtable[k]) = NULL;
	}

    ~MyHashMap()
    {
		Node* temp = prev;
		while( temp != NULL)
		{
			Node* NextOne = temp->second_next;
			delete temp;
			temp = NextOne;
		}
		for( int k=0; k < hashsize; k++)
			delete hashtable[k];
		delete [] hashtable;
    }

    void associate(std::string key, const ValueType& value)
    {
		if( find(key) == NULL)	// first find if the key exist in the table
		{	
			int pos = hash(key)%hashsize;	// insert the new node, a the specific address
			Node* p = new Node;
			p->value = value;
			p->key = key;
			p->second_next = prev;			// the second linked list next points to previous Node, if exist
			p->next = hashtable[pos];
			hashtable[pos] = p;
			prev = p;						// set prev pointer to key/pair just created
			count++;
		}
		else
		{
			*(find(key)) = value;
		}
		getNextWord = false;	
    }

    const ValueType* find(std::string key) const
    {
		string word = key;
		unsigned int pos = hash(word)%hashsize;
		if( hashtable[pos] == NULL)/*if the element is pointing to NULL*/
			return NULL;
		else
		{						// if there node does not point to NULL then traverse through that position and find the key
			Node* ptr;
			for(ptr = hashtable[pos]; ptr != NULL; ptr = ptr->next)
			{
				if( key == ptr->key )
					return &ptr->value;
			}
			return NULL;
		}
    }

    ValueType* find(std::string key)
    {
          // Do not change the implementation of this overload of find
        const MyHashMap<ValueType>* constThis = this;
        return const_cast<ValueType*>(constThis->find(key));
    }

    ValueType* getFirst(std::string& key)
    {
		curr = prev;
		if(count < 1)
			return NULL;
		else
		{
			if(curr == NULL)
				return NULL;
			key = curr->key;
			return &curr->value;
			getNextWord = true;
		}
    }

    ValueType* getNext(std::string& key)
    {
		if( getNextWord == false || getFirst(key) == NULL)
			return NULL;
		curr = curr->second_next;
		if( curr == NULL) 
			return NULL;

		key = curr->key;
		return &curr->value;
    }

    int numItems() const
    {
        return count;
    }

private:

	const unsigned int hash(std::string s) const
	{
		unsigned int h = 2166136261;
		for(int k = 0; k != s.size(); k++)
		{	
			h+=s[k];
			h*= 16777619;
		}
		return h;
	}

	struct Node				//creates a node for singly linked list 
	{
		std::string key;
		ValueType value;
		Node* next;
		Node* second_next;	// another pointer that directly links every key added 
	};
	Node** hashtable;
	Node* prev;			// another pointer that points to the previous key/pair added
	Node* curr;			// points to current node
	int count;				// count the number of association created
	int hashsize;			// bucket size
	bool getNextWord;
};

#endif // MYHASHMAP_INCLUDED