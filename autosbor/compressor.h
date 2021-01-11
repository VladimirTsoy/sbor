#ifndef COMPRESSOR_H
#define COMPRESSOR_H
#include "crx2rnx.h"
#include "rnx2crx.h"
#include <iostream>

class Compressor
{
public:
    Compressor();
    ~Compressor();

    bool compress(std::string str);
    bool unCompress(std::string str);

private:
    Rnx2Crx *rnx2Crx;
    Crx2Rnx *crx2rnx;
};

#endif // COMPRESSOR_H
