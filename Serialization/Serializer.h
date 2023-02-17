#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "SerialException.h"

namespace Serialization {

	/*
	* Default id. Serializer must have
	* different id to be marshaled.
	*/
	const long NO_ID = std::numeric_limits<long>::max();

	/*
	 * Used in ListSerializer as an id.
	 */
	const long LIST_ID = std::numeric_limits<long>::min();

	/*
	 * Automatically added to data when this object
	 * is marshaled. Needed for deserialization.
	 */
	const long END_ID = NO_ID / 2;

	/*
	 * The size of the tags used to mark the beginning and end
	 * of each serialized object.
	 */
	const long NODE_TAG_SIZE = sizeof(long);

	/*
	 * Handle used to hold serialized data.
	 * Can be used directly. However,
	 * it is recommended to have an
	 * object implement class Serializable
	 * and return a Serializer* that way.
	 * The Serializable object is later passed
	 * to SerialWriter.addNode(), where its
	 * Serializer* is extracted.
	 *
	 * Author: Trevor Lash
	 * Edited 2/14/23
	 */
	class Serializer{

		private:

			long id = NO_ID;
			bool marshalled = false;

			/*
			 * Is filled and
			 * length set when marshal() is
			 * called
			 */
			char* data = nullptr;
			long length = 0;

		protected:

			/*
			 * Underlying stream for
			 * this object
			 */
			std::stringstream marshalStream;

		public:

			Serializer(){
				writeId(id);
			};

			/*
			 * Copy constructor. Makes exact
			 * copy. Only executes if argument
			 * is marshaled.
			 */
			Serializer(const Serialization::Serializer& o){
				if(!o.isMarshalled()){
					throw new Serialization::SerialException(
							"Serializer(const Serializer&): "
							"cannot copy a serializer"
							"without marshalling");
				}

				id = o.getId();

				if(marshalled){
					delete data;
				} else {
					marshalled = true;
				}

			    data = o.deepCopy(),
			    length = o.length;

				marshalStream.seekp(std::ios::beg);
				marshalStream.write(data, length - NODE_TAG_SIZE);
			}

			Serializer(long _id){
				switch(_id){
					case NO_ID:
						throw std::invalid_argument(
							"Serializer::setId(): "
							"id cannot be Serializer::NO_ID");

					case END_ID:
						throw std::invalid_argument(
							"Serializer::setId(): "
							"id cannot be Serializer::END_ID");

					default:
						id = _id;

						writeId(id);
				}
			}

			virtual ~Serializer(){
				if(marshalled){
					delete[]data;
				}
			}

			/*
			 * Returns Serializer::NO_ID if no id
			 */
			long getId() const{
				return id;
			}

			void setId(long _id){
				switch(_id){
					case NO_ID:
						throw std::invalid_argument(
							"Serializer::setId(): "
							"id cannot be Serializer::NO_ID");

					case END_ID:
						throw std::invalid_argument(
							"Serializer::setId(): "
							"id cannot be Serializer::END_ID");

					default:
						id = _id;

						writeId(id);
				}
			}

		private:

			/*
			 * sizeof(long) bytes if reserved at
			 * beginning of data to write the id tag.
			 * This function sets the cursor at beginning of data to
			 * write the id, and then sets it
			 * back at the end of the stream.
			 */
			void writeId(long _id){
				long currp = ((long)marshalStream.tellp()) ?
						((long)marshalStream.tellp()) : NODE_TAG_SIZE;

				marshalStream.seekp(std::ios::beg);
				marshalStream.write((char*)&_id, NODE_TAG_SIZE);
				marshalStream.seekp(currp);
			}

		public:

			/*
			 * Returns 0 if not marshaled
			 */
			long getLength() const{
				return length;
			}

			const char* shallowCopy() const{
				return data;
			}

			char* deepCopy() const {
				if(!marshalled){
					return nullptr;
				}

				char* cpy = new char[length];

				memcpy(cpy, data, length);

				return cpy;
			}

			bool isMarshalled() const{
				return marshalled;
			}

			void add(const void* data, int _size){
				marshalStream.write((const char*) data, _size);
			}

			/*
			 * Used to copy other serializers into this serializer.
			 */
			void add(Serialization::Serializer* serializer){
				if(!serializer){
					throw std::invalid_argument("Serializer::add(): "
							"null argument");

				} if(!(serializer->isMarshalled())){
					throw std::invalid_argument(
							"Serializer::add(): "
							"argument not marshalled");
				}

				marshalStream.write(
						(const char*) serializer->data,
						serializer->length);
			}

			/*
			 * Copies the stream into a
			 * retrievable array. Adds tag END_ID
			 * at the end for later deserialization.
			 * marshal() can be called as many
			 * times as needed to re-update the array.
			 */
			virtual void marshal(){
				if(id == NO_ID){
					throw new Serialization::SerialException(
							"Serializer::marshal():"
							"node has no id");

				} else if(marshalled){
					delete[]data;

				} else {
					marshalled = true;
				}

				long bytesInStream = marshalStream.tellp();

				data = new char[(length =
				        bytesInStream + NODE_TAG_SIZE)];

				marshalStream.seekg(std::ios::beg);
				marshalStream.read(data, bytesInStream);

				long* endIdPtr = ((long*)(data + bytesInStream));
				*endIdPtr = END_ID;
			}
	};
}

#endif 
