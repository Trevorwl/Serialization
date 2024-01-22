#ifndef LISTSERIALIZER_H_
#define LISTSERIALIZER_H_

#include <iostream>
#include <stdexcept>

#include "SerialException.h"
#include "Serializer.h"

namespace Serialization{

	/*
	  Use this class for serializing lists.

	  Note: Remember to call setNumberOfItems()
	  before marshaling. Otherwise an exception will be
	  thrown.

	  Author: Trevor Lash
	  Edited 2/14/23
	*/
	class ListSerializer :
	        public Serialization::Serializer {

		private:

			/*
			 * Needs to be set later by user to actual
			 * value before calling marshal()
			 */
			long numberOfItems = -1;

		public:
			/*arbitrary change*/

			/*
			 *  Note: Remember to set the numberOfItems
				before marshaling. Otherwise an exception will be
				thrown.
			 */
			ListSerializer() : Serializer(LIST_ID){
				marshalStream.write((char*)&numberOfItems,
				        NODE_TAG_SIZE);
			}

			/*
			 * Note: Only makes a copy if the argument is
			 * marshaled. See Serializer's copy
			 * constructor for more implementation
			 * details.
			 */
			ListSerializer(const ListSerializer& o) : Serializer(o){
				numberOfItems = o.numberOfItems;
			}


			/*
			 * The argument does not restrict the number
			 * of items you can add. Call setNumberOfItems()
			 * later if the amount of items in list changes.
			 */
			ListSerializer(long _numberOfItems)
			        : Serializer(LIST_ID){
			    setNumberOfItems(_numberOfItems);
			}

			/*
			 * Returns -1 if number of items
			 * not set.
			 */
			long getNumberOfItems(){
				return numberOfItems;
			}

			/*
			 * For a ListSerializer, sizeof(long)
			 * bytes are saved after the id to
			 * record the number of elements in the list.
			 */
			void setNumberOfItems(long _numberOfItems) {
				if(_numberOfItems < 0){
					throw std::invalid_argument(
							"Serialization::ListSerializer::"
							"setNumberofItems():"
							"number of items must be >= 0");
				}

				long currp = ((long)marshalStream.tellp())
				        == NODE_TAG_SIZE ? NODE_TAG_SIZE * 2 :
				        ((long)marshalStream.tellp());

				marshalStream.seekp(NODE_TAG_SIZE);
				marshalStream.write((char*)&_numberOfItems, NODE_TAG_SIZE);
				marshalStream.seekp(currp);

				numberOfItems = _numberOfItems;
			}

			void marshal(){
				if(numberOfItems == -1){
					throw new Serialization::SerialException(
						"ListSerializer::marshal(): "
						"number of items not set");
				}

				Serializer::marshal();
			}
    };
}

#endif 
