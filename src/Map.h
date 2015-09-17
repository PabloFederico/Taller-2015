/*
 * Map.h
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#ifndef MAP_H_
#define MAP_H_
#include <map>

template <class K,class V>
class Map{
private:
	std::map<K,V>* map;

public:
	Map();

	void put(K, V);

	V get(K);

	virtual ~Map();
};

template <class K, class V>
Map<K,V>::Map(){
   this->map = new std::map<K,V>();
}

template <class K, class V>
void Map<K,V>::put(K key,V value){
   this->map->insert(std::make_pair(key,value));
}

template <class K, class V>
V Map<K,V>::get(K key){
	std::map<K,V>::iterator p = this->map->find(key);
	//if (p != this->map->end()) return p.second;
    return 0;
}

template <class K, class V>
Map<K,V>::~Map(){
   delete this->map;
}



#endif /* MAP_H_ */
