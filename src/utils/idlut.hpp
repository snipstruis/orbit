#pragma once

#include "minimal_unsigned_type.hpp"

#include <cstdint>
#include <algorithm>
#include <iostream>

/**
Idlut (ID Look Up Table)

A container that has its elements continguous and local without allocation.

**/

template<typename element_t, uint64_t max_elements>
class OrderedIdlut{
public:
	using index_t = minimal_unsigned_type<max_elements>;
private:
	element_t elements  [max_elements];
	index_t   indices   [max_elements]; // lookup table: id -> index of elements
	index_t   revIndices[max_elements]; // lookup table: index of elements -> id
	index_t   nextFreeId;
	index_t   nrOfElements;
public:
	OrderedIdlut()
		: nextFreeId(0),
		  nrOfElements(0){
		for(index_t i=0; i<max_elements; i++){
			indices[i]=i+1;
		}
	}
	// guaranteed to append element at the end
	index_t push_back(element_t t){
		elements[nrOfElements] = t;      // copy element to final position
		index_t idOfT = nextFreeId;      // save the ID of the new element
		nextFreeId = indices[idOfT];     // update nextFreeIndex
		indices[idOfT] = nrOfElements;   // update id_to_index
		revIndices[nrOfElements] = idOfT;// update index_to_id
		nrOfElements++;                  // update nrOfElements
		return idOfT;
	}
	// guaranteed to preserve element order
	index_t insert_before(index_t id, element_t t){
		const index_t ret = nextFreeId;
		const index_t indexOfT = indices[id];
		std::copy_backward(&elements[indexOfT],
						   &elements[nrOfElements],
						   &elements[nrOfElements+1]);
		std::copy_backward(&revIndices[indexOfT],
						   &revIndices[nrOfElements],
						   &revIndices[nrOfElements+1]);
		for(index_t i=indexOfT; i<=nrOfElements; i++){indices[revIndices[i]]++;}
		elements[indexOfT] = t;
		revIndices[indexOfT] = nextFreeId;
		nextFreeId = indices[nextFreeId];
		indices[revIndices[indexOfT]] = indexOfT;
		return ret;
	}
	// guaranteed to preserve element order
	void ordered_erase(const index_t id, const unsigned n=1){
		index_t indexOfId = revIndices[id];

		// manage arrays
		std::copy(&revIndices[indices[id]+n],
				  &revIndices[nrOfElements],
				  &revIndices[indices[id]]);
		for(index_t i=id; i<id+n; i++){
			indices[revIndices[i]] -= n;
		}
		std::copy(&elements[indices[id]+n],
				  &elements[nrOfElements],
				  &elements[indices[id]]);
		nrOfElements-=n;

		// manage freelist
		for(index_t i=0; i<n-1; i++)
			indices[indexOfId+i] = indexOfId+i+1;
		indices[indexOfId+n-1] = nextFreeId;
		nextFreeId = indexOfId;
	}
	index_t    size()  const {return nrOfElements;}
	element_t* begin() {return &elements[0]; };
	element_t* end()   {return &elements[nrOfElements];}
	element_t& operator[](index_t id){return elements[indices[id]];}
};
