#include "compressfactory.h"

//std::unordered_map<std::string, std::function<Compressor*()>> compressFactory::compressorCreators;

//static std::unordered_map<std::string, std::pair<CreatorFunction, ReleaseFunction>> compressorCreators;

//Compressor *compressFactory::compressorCreate(const std::string &type)
//{
//    auto& creators = getCreators();

//    auto it = creators.find(type);

//    if (it != creators.end())
//    {
//        return （it->second.first)();
//    }
//    else
//    {
//        return nullptr;
//    }
//}

//void compressFactory::compressorRegister(const std::string &type, std::function<Compressor *()> creator)
//{
//    auto& creators = getCreators();
//    creators[type] = creator;
//}
