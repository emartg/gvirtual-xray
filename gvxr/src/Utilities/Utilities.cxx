/*

Copyright (c) 2016, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the Bangor University nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


/**
********************************************************************************
*
*   file        Utilities.cxx
*
*  @brief       Some utility functions that do not fit anywhere else.
*
*  @version     1.0
*
*  @date        12/02/2014
*
*  @author      Dr Franck P. Vidal
*
*  @section     License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*  @section     Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <zlib.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************


//----------------------------------------------------
int gVirtualXRay::inflate(const void *src, int srcLen, char** apDst)
//----------------------------------------------------
{
    // See http://stackoverflow.com/questions/4901842/
    //                   in-memory-decompression-with-zlib
    int dstLen(50 * srcLen);
    if (*apDst) delete [] *apDst;
    *apDst = new char[dstLen];
    memset(*apDst, '\0', dstLen * sizeof(char));

    z_stream strm  = {0};
    strm.total_in  = strm.avail_in  = srcLen;
    strm.total_out = strm.avail_out = dstLen;
    strm.next_in   = (Bytef *) src;
    strm.next_out  = (Bytef *) (*apDst);

    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;

    int err = -1;
    int ret = -1;

    //15 window bits, and the +32 tells zlib to to detect if using gzip or zlib
    err = inflateInit2(&strm, (15 + 32));
    if (err == Z_OK)
        {
        err = inflate(&strm, Z_FINISH);

        if (err == Z_STREAM_END)
            {
            ret = strm.total_out;
            }
        else
            {
            inflateEnd(&strm);
            delete [] *apDst;
            *apDst = 0;
            return (err);
            }
        }
    else
        {
        inflateEnd(&strm);
        delete [] *apDst;
        *apDst = 0;
        return (err);
        }

    inflateEnd(&strm);
    return (ret);
}


//------------------------------------------------------------------
int gVirtualXRay::deflate(const void *src, int srcLen, char** apDst)
//------------------------------------------------------------------
{
    int dstLen(srcLen);
    if (*apDst) delete [] *apDst;
    *apDst = new char[dstLen];
    memset(*apDst, '\0', dstLen * sizeof(char));

    // zlib struct
    z_stream strm  = {0};
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    // setup "a" as the input and "b" as the compressed output
    strm.avail_in  = srcLen; // size of input, string + terminator
    strm.next_in   = static_cast<unsigned char*>(const_cast<void*>(src)); // input char array
    strm.avail_out = dstLen; // size of output
    strm.next_out  = (unsigned char*) *apDst; // output char array

    // the actual compression work.
    int err = -1;
    int ret = -1;


    deflateEnd(&strm);

    err = deflateInit(&strm, Z_BEST_COMPRESSION);
    if (err == Z_OK)
        {
        err = deflate(&strm, Z_FINISH);

        if (err == Z_STREAM_END)
            {
            ret = strm.total_out;
            }
        else
            {
            deflateEnd(&strm);
            delete [] *apDst;
            *apDst = 0;
            return (err);
            }
        }
    else
        {
        deflateEnd(&strm);
        delete [] *apDst;
        *apDst = 0;
        return (err);
        }

    deflateEnd(&strm);
    return (ret);
}


//-------------------------------------------------
bool gVirtualXRay::checkExtension(const std::string& aFileName,
                    const std::string& anExtension)
//-------------------------------------------------
{
    unsigned int file_name_size(aFileName.size());
    unsigned int extension_size(anExtension.size());

    if (file_name_size < (extension_size + 2))
        {
        return (false);
        }

    std::string file_extension(aFileName.substr(file_name_size - extension_size - 1,
            extension_size + 1));

    std::string extension_plus_dot(".");
    extension_plus_dot += anExtension;

    std::transform(
    		file_extension.begin(),
			file_extension.end(),
			file_extension.begin(),
			::toupper);

    std::transform(
    		extension_plus_dot.begin(),
    		extension_plus_dot.end(),
			extension_plus_dot.begin(),
			::toupper);

    return (file_extension == extension_plus_dot);
}
