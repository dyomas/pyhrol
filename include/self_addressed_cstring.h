/*
 *   Copyright (c) 2013 Pyhrol, pyhrol@rambler.ru
 *   GEO: N55.703431,E37.623324 .. N48.742359,E44.536997
 * 
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   4. Neither the name of the Pyhrol nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 * 
 *   THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 *   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *   ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 *   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *   OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *   SUCH DAMAGE.
 */

// $Date: 2013-09-18 02:14:07 +0400 (Wed, 18 Sep 2013) $
// $Revision: 785 $

#ifndef __self_addressed_cstring_h__
#define __self_addressed_cstring_h__


#include <sys/types.h>
//#include ""


//#define

/**
SelfAddressedCstringBase хранит собственный адрес в строке (слева от ее начала),
поэтому может быть восстановлен из такой строки.
*/
template <typename T> class SelfAddressedCstringBase
{
  char *m_storage;
  SelfAddressedCstringBase(const SelfAddressedCstringBase<T> &);
  SelfAddressedCstringBase &operator= (const SelfAddressedCstringBase<T> &);

  protected:
    char *m_allocate(const size_t, const T *);

  public:
    const char *get() const;
    void get(char *&) const;
    void get(const char *&) const;
    static const T *construct_const(const char *data);
    static T *construct(const char *data);

    SelfAddressedCstringBase();
    ~SelfAddressedCstringBase();
};

class SelfAddressedCstring: public SelfAddressedCstringBase<SelfAddressedCstring>
{
  public:
    char *allocate(const size_t);
};

#endif //__self_addressed_cstring_h__