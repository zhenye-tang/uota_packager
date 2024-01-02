#ifndef ZLIBCOMPRESSOR_H
#define ZLIBCOMPRESSOR_H

#include "compressor.h"

class zlibCompressor : public Compressor
{
    int compress(const string& sourceFile, const string& compressFile) override;
    ~zlibCompressor(){}
};



#endif // ZLIBCOMPRESSOR_H
