#ifndef COMPRESS_H
#define COMPRESS_H

using namespace std;
#include <string>

class Compressor
{
public:
    virtual int compress(const string& sourceFile, const string& compressFile) = 0;
    virtual~Compressor() = default;
};

#endif // COMPRESS_H
