# Serialization
Convenient Serialization Library for C++.
Objects can implement class Serializable in Serializable.h,
and be conveniently processed by this library.

Demo.cpp -> A demo of how the library is implemented

Serializable.h -> Interface that contains functions that allows objects to be serialized

Serializer.h -> Handle for serialized data

ListSerializer.h -> Extension of Serializer.h for handling data
from containers such as vectors.

SerialReader.h and SerialWriter.h read and write Serializable objects from/to a file.

***********************************************************
For objects that implement Serializable:

The serialize() function should be written as
```
Serialization::Serializer* serialize(){
  Serialization::Serializer* serializer
          = new Serialization::Serializer(YOUR_OWN_CLASS_ID);

  //add data to serializer

  serializer->marshal(); //Marshal the data. Must do this.

  return serializer;
}
```
Serializable classes should have
a deserializing function in its file
that has the following format:
```
Program::SomeObject* deserializeSomeObject(
        Serialization::SerialReader& reader){

  reader.readNextId(); //consume block header

  //getData

  reader.readNextId(); //consume block footer

  //return pointer to new object
}
```
-Client code that calls SerialReader::readNextId()
before calling a deserialization function
(either directly or indirectly)
should call SerialReader::rewindId() first,
as it is good practice for a deserialization
function to read the id's of its data.

-Note:
rewindId() typically doesn't
need to be called if you are reading
a Serialization::LIST_ID or
Serialization::END_ID.

-If you are checking to see if an id
doesn't match these values,
you might want to call rewindId()

-SerialReader::readListSize() typically
does't require a call to rewindId()
afterwards.

**********************************

-Important: make sure to call Serializer::marshal()
before returning from serialize().
The serializer cannot be written
to a file or copied otherwise.

**********************************
-A polymorphic superclass's file
can include a function that calls
the needed deserializer.
This function should be treated as client code
and should also call SerialReader::rewindId()
```
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
```
