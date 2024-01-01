#ifndef COMPRESSFACTORY_H
#define COMPRESSFACTORY_H

#include <functional>
#include <unordered_map>

class Compressor;

class compressFactory
{
public:

    using CreatorFunction = Compressor* (*)();
    using ReleaseFunction = void (*)(Compressor*);

    static void compressorRegister(const std::string& type, CreatorFunction creator, ReleaseFunction release)
    {
        getCreators()[type] = std::make_pair(creator, release);
    }

    static Compressor *compressorCreate(const std::string &type)
    {
        auto& creators = getCreators();
        auto it = creators.find(type);
        if (it != creators.end())
        {
            return (it->second.first)();
        }
        else
        {
            return nullptr;
        }
    }

    static  void compressorDestory(const std::string &type, Compressor *compressor)
    {
        auto& creators = getCreators();
        auto it = creators.find(type);
        if (it != creators.end())
        {
            (it->second.second)(compressor);
        }
    }

private:
    static std::unordered_map<std::string, std::pair<CreatorFunction, ReleaseFunction>>& getCreators()
    {
            static std::unordered_map<std::string, std::pair<CreatorFunction, ReleaseFunction>> creators;
            return creators;
    }
};


#endif // COMPRESSFACTORY_H
