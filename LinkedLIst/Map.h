#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>

typedef std::string KeyType;
typedef double ValueType;

class Map
{
  public:
    Map();
	Map( const Map& copy);
	Map operator=(const Map& other);
	~Map();
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);

  private:
	struct Node{
		Node* prev;
		Node* next;
		KeyType key;
		ValueType value;
	};
	Node* head;
	int m_pairs;
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif
