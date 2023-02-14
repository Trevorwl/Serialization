#ifndef SERIALIZATION_SERIALIZABLE_H_
#define SERIALIZATION_SERIALIZABLE_H_

#include "Serializer.h"

namespace Serialization{

	/*
	   An interface for classes to make them serializable
	   for this library.

	   Author: Trevor Lash
	   Edited 2/14/23

	   Serializable classes should have
	   a deserializing function in its file
	   that has the following format:

		   Program::SomeObject* deserializeSomeObject(
		           Serialization::SerialReader& reader){

				std::ifstream* st = reader.stream;  //get data stream


				reader.readNextId();  //consume block header



				//getData



				reader.readNextId(); 	//consume block footer

				//return pointer to new object
		  }

	   Client code that calls SerialReader::readNextId()
	   before calling a deserialization function
	   (either directly or indirectly)
	   should call SerialReader::rewindId() first,
	   as it is good practice for a deserialization
	   function to read the id's of its data.

	   Note:
	   rewindId() typically doesn't
	   need to be called if you are reading
	   a Serialization::LIST_ID or
	   Serialization::END_ID.

	   If you are checking to see if an id
	   doesn't match these values,
	   you might want to call rewindId()

	   SerialReader::readListSize() typically
	   does't require a call to rewindId()
	   afterwards.

	   **********************************

	   Important: make sure to call Serializer::marshal()
	   before returning from serialize().
	   The serializer cannot be written
	   to a file or copied otherwise.

	   **********************************
	   A polymorphic superclass's file
	   can include a function that calls
	   the needed deserializer.
	   This function should be treated as client code
	   and should also call SerialReader::rewindId()

		   Program::SuperClass* deserializeSuper(
				   Serialization::SerialReader& reader){

				long nextId = reader.readNextId();
				reader.rewindId();

				switch(nextId){
					case id1:
						return Program::deserializeId1(reader);
					case id2:
						return Program::deserializeId2(reader);
					//etc
				}

				return nullptr;
		  }
	 */
	class Serializable {

		public:

			/*
			   Note: make sure to call Serializer::marshal()
			   before returning from serialize().
			   The serializer cannot be written
			   to a file or copied otherwise.
			 */
			virtual Serialization::Serializer* serialize() = 0;
			virtual ~Serializable(){}
	};

}

#endif 
