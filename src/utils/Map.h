/*
 * Map.h
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#ifndef UTILS_MAP_H_
#define UTILS_MAP_H_
#include <map>

template <class K,class V>
class Map {
private:
	std::map<K,V> *mapa;

public:
	Map();

	void insert(K,V);

	int size();

	typename std::map<K,V>::iterator find(K);

	typename std::map<K,V>::iterator begin();

	typename std::map<K,V>::iterator end();

	void erase(K);

	virtual ~Map();
};

template <class K, class V>
Map<K,V>::Map(){
   this->mapa = new std::map<K,V>();
}

template <class K, class V>
int Map<K,V>::size(){
   return this->mapa->size();
}

template <class K, class V>
void Map<K,V>::insert(K key,V value){
   this->mapa->insert(std::make_pair(key,value));
}

template <class K, class V>
typename std::map<K,V>::iterator Map<K,V>::find(K key){
	typename std::map<K,V>::iterator p = this->mapa->find(key);
	if (p != this->mapa->end()) return p;
    return this->mapa->end();
}

template <class K, class V>
typename std::map<K,V>::iterator Map<K,V>::begin(){
	return this->mapa->begin();
}

template <class K, class V>
typename std::map<K,V>::iterator Map<K,V>::end(){
	return this->mapa->end();
}

template <class K, class V>
void Map<K,V>::erase(K key){
	this->mapa->erase(key);
}

template <class K, class V>
Map<K,V>::~Map(){
   this->mapa->clear();
   delete this->mapa;
}

#endif /* UTILS_MAP_H_ */
