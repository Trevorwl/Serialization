#ifndef SERIALREADER_H_
#define SERIALREADER_H_

#include "Serializer.h"

#include <fstream>
#include <string>

namespace Serialization{

	/*
	 * A stream that reads serialized data from a file.
	 * This object is passed to deserialization functions
	 * for data extraction. Data in file being read is typically written
	 * by a SerialWriter. See Serializable.h for more implementation details.
	 *
	 * Author: Trevor Lash
	 * Edited 2/14/23
	 */
	class SerialReader{

		private:

			std::string url;

		public:

			std::ifstream* stream;

			SerialReader(std::string _url){
				stream = new std::ifstream(_url);
			}

			~SerialReader(){
				stream->close();
				delete stream;
			}

			/*
			 * Reads next id tag in data.
			 * If you call this before entering
			 * a deserializing function, call rewindId() next.
			 * This is because it is good practice for
			 * a deserializing function to read the Id's
			 * in a file.
			 */
			long readNextId(){
				long id;
				stream->read((char*)&id,
						Serialization::NODE_TAG_SIZE);

				return id;
			}

			/*
			 * Convenience method for
			 * reading a list size for
			 * an object with the tag
			 * Serialization::LIST_ID.
			 *
			 * Call this immediately after
			 * reading a LIST_ID.
			 *
			 * Mainly used so that a
			 * user doesn't become
			 * confused by a call to
			 * readNextId()
			 * and call rewindId()
			 */
			long readListSize(){
				return readNextId();
			}

			/*
			 * Goes back sizeof(long) bytes
			 * so that id can be read in
			 * future reads.
			 */
			void rewindId(){
				stream->seekg((stream->tellg())
				        - Serialization::NODE_TAG_SIZE);
			}

			/*
			 * Does this stream have an error?
			 */
			operator bool(){
				return (bool)(*stream);
			}
	};
}

#endif 
