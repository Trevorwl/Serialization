#ifndef LISTSERIALIZER_H_
#define LISTSERIALIZER_H_

#include "Serializer.h"

namespace Serialization{

	/*
	  Use this class for serializing lists.
	  Call setNumberOfItems() before adding any data,
	  as deserialization functions need to read the list size
	  after checking the id tag.

	  Author: Trevor Lash
	  Edited 2/14/23
	*/
	class ListSerializer :
	        public Serialization::Serializer {

		private:

			long numberOfItems = 0;

		public:

			ListSerializer() : Serializer(LIST_ID){}

			long getNumberOfItems(){
				return numberOfItems;
			}

			void setNumberOfItems(long _numberOfItems) {
				if(_numberOfItems < 0){
					throw std::invalid_argument(
							"Serialization::ListSerializer::"
							"setNumberofItems():"
							"number of items must be >= 0");
				}

				numberOfItems = _numberOfItems;

				add(&_numberOfItems, sizeof(long));
			}
    };
}

#endif 
