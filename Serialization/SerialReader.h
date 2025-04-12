#ifndef SERIALREADER_H_
#define SERIALREADER_H_

#include <fstream>
#include <string>

#include "Serializer.h"
#include "SerialWriter.h"

namespace Serialization{


	/*
	 * A stream that reads serialized data from a file.
	 * This object is passed to deserialization functions
	 * for data extraction. Data in file being read is typically written
	 * by a SerialWriter.
	 *
	 * Author: Trevor Lash
	 * Edited 2/16/23
	 */
	class SerialReader{

		private:

			std::string url;

		public:

			/*
			 * Underlying file stream.
			 */
			std::ifstream* stream;

			SerialReader(std::string _url){
				stream = new std::ifstream(_url);
			}

			~SerialReader(){
				stream->close();
				delete stream;
			}

			void readData(const void* dest, long size){
				stream->read((char*)dest, size);
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
			 * Call this immediately after
			 * reading a LIST_ID.
			 *
			 * Convenience method for
			 * reading a list size for
			 * an object with the tag
			 * Serialization::LIST_ID.
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
			 * so that an id tag can be read in
			 * future reads.
			 */
			void rewindId(){
				stream->seekg((stream->tellg())
						- Serialization::NODE_TAG_SIZE);
			}


			/*
			 * Do we still have more to read and
			 * still have no errors present?
			 */
			operator bool(){
				long nextTag = readNextId();
				rewindId();

				return (nextTag != Serialization::EOF_ID)
						&& ((bool)(*stream));
			}
	};
}

#endif 
