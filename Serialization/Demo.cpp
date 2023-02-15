#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Serialization/ListSerializer.h"
#include "Serialization/Serializable.h"
#include "Serialization/Serializer.h"
#include "Serialization/SerialReader.h"
#include "Serialization/SerialWriter.h"

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
			Serialization::Serializer* ser
					= new Serialization::Serializer(CLL_ID);

			ser->add(&x,sizeof(int));
			ser->add(&y,sizeof(int));

			Serialization::ListSerializer ls;
			ls.setNumberOfItems(list.size());

			for(auto e : list){
				ls.add(&e,sizeof(int));
			}

			ls.marshal();

			ser->add(&ls);
			ser->marshal();
			return ser;
		}
};

Cll* deserializeCll(Serialization::SerialReader& srd){
	srd.readNextId();//read header
	std::ifstream* ifs = srd.stream;//get stream

	int x;
	int y;
	ifs->read((char*)&x, sizeof(int));
	ifs->read((char*)&y, sizeof(int));

	Cll* toReturn = new Cll(x, y);

	srd.readNextId(); // read list header
	int listSize = srd.readListSize(); // read list size

	toReturn->list.resize(listSize);

	for(int i = 0; i < listSize; i++){
		ifs->read((char*)&(toReturn->list.at(i)),
				sizeof(int));
	}
	srd.readNextId(); // read list footer

	srd.readNextId();//read footer
	return toReturn;
}

void Demo() {
	std::vector<Cll> objects{{1,1},{2,1},{3,1}};

	Serialization::SerialWriter sw;
	for(auto&e :objects){
		sw.addNode(e);
	}

	sw.writeToFile("File.txt");

	Serialization::SerialReader sr("File.txt");

	while(sr){
		std::unique_ptr<Cll> ptr{
			deserializeCll(sr)};

		std::cout<< (std::string)(*ptr) <<std::endl;
	}
}
