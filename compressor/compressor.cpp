#include "compressor.h"

Compressor::Compressor()
{
    rnx2Crx = new Rnx2Crx;
    crx2rnx = new Crx2Rnx;
}

Compressor::~Compressor()
{
    delete rnx2Crx;
    delete crx2rnx;
}

bool Compressor::compress(std::string str){
    if(rnx2Crx != nullptr)
        delete rnx2Crx;
    rnx2Crx = new Rnx2Crx;
    return rnx2Crx->compr(str);
}

bool Compressor::unCompress(std::string str){
    if(crx2rnx != nullptr)
        delete crx2rnx;
    crx2rnx = new Crx2Rnx;
    return crx2rnx->unCompr(str);

}
