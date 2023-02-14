#ifndef SERIALWRITER_H_
#define SERIALWRITER_H_

#include "SerialException.h"
#include "Serializable.h"
#include "Serializer.h"

#include <fstream>
#include <string>
#include <vector>
#include <memory>

namespace Serialization {

	/*
	 * Used to write serialized data to a file.
	 * Objects implementing Serializable.h
	 * are passed to addNode(), where their
	 * Serialize() function is called to extract data.
	 */
	class SerialWriter{

		private:

			/*
			 * Where serializers are stored before writing
			 */
			std::vector<Serialization::Serializer*>allNodes;

			/*
			 * Size of all serializers' data
			 */
			long dataSize = 0;

		public:

			~SerialWriter(){
				for(auto& e : allNodes){
					delete e;
				}
			}

			void addNode(Serialization::Serializable& _node){
				Serialization::Serializer* serialNode
				        = _node.serialize();

				if(!(serialNode->isMarshalled())){
					delete serialNode;

					throw Serialization::SerialException(
							"Serializer::AddNode(): "
							"Object's serializer "
							"not marshalled");
				}

				allNodes.push_back(serialNode);

				dataSize += (serialNode->getLength());
			}

			long getDataSize(){
				return dataSize;
			}

			void writeToFile(std::string url){
				if(!allNodes.size()){
					throw SerialException("SerialWriter::write(): "
							"No data to write");
				}

				std::ofstream os(url);

				for(auto& node: allNodes){
					std::unique_ptr<char>copy{node->getData()};

					os.write(copy.get(), node->getLength());
				}

				os.close();
			}
	};
}


#endif 
