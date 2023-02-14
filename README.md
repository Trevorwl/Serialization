# Serialization
Convenient Serialization Library for C++.
Objects can implement class Serializable in Serializable.h,
and be conveniently processed by this library.

Serializable.h -> Interface that contains functions that allows objects to be serialized

Serializer.h -> Handle for serialized data

ListSerializer.h -> Extension of Serializer.h for handling data
from containers such as vectors.

SerialReader.h and SerialWriter.h read and write Serializable objects from/to a file.

See Serializable.h for implementation details
