#ifndef LZ4COMPRESSOR_H
#define LZ4COMPRESSOR_H
#include "compressor.h"

class Lz4Compressor : public Compressor
{
public:
    int compress(const string& sourceFile, const string& compressFile) override;
    ~Lz4Compressor(){}
};

#endif // LZ4COMPRESSOR_H
