#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "ListSerializer.h"
#include "Serializable.h"
#include "Serializer.h"
#include "SerialReader.h"
#include "SerialWriter.h"

class Cll : public Serialization::Serializable{

	public:

		int x;
		int y;
		std::vector<int>list{1,2,3};

		const long CLL_ID = 1;

		Cll(int _x = 0,int _y = 0):
		        x{_x},y{_y}{}

		operator std::string(){
			char data[64];
			sprintf(data,"x: %d -- y:%d -- list:%d",
					x, y, list.size());

			return {data};
		}

		Serialization::Serializer* serialize(){
			Serialization::Serializer* serializer
					= new Serialization::Serializer(CLL_ID);

			serializer->add(&x,sizeof(int));
			serializer->add(&y,sizeof(int));

			Serialization::ListSerializer listSerializer(
			        list.size());

			for(auto e : list){
				listSerializer.add(&e, sizeof(int));
			}

			listSerializer.marshal();

			serializer->add(&listSerializer);

			serializer->marshal();
			return serializer;
		}
};

Cll* deserializeCll(Serialization::SerialReader&
        serialReader){
	serialReader.readNextId();//read header

	int x;
	int y;
	serialReader.readData((char*)&x, sizeof(int));
	serialReader.readData((char*)&y, sizeof(int));

	Cll* toReturn = new Cll(x, y);

	serialReader.readNextId(); // read list header
	int length = serialReader.readListSize(); // read list size

	toReturn->list.resize(length);

	for(int i = 0; i < length; i++){
		serialReader.readData(&(toReturn->list)[i],
		        sizeof(int));
	}

	serialReader.readNextId(); // read list footer

	serialReader.readNextId();//read footer
	return toReturn;
}

void demo() {
	std::vector<Cll> objects{{1,1},{2,1},{3,1}};

	Serialization::SerialWriter serialWriter;
	for(auto& e : objects){
		serialWriter.addNode(e);
	}

	serialWriter.writeToFile("File.txt");

	Serialization::SerialReader serialReader("File.txt");

	while(serialReader){
		std::unique_ptr<Cll> data{
			deserializeCll(serialReader)};

		std::cout << (std::string)(*data) << std::endl;
	}
}
