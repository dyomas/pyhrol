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

// $Date: 2014-01-09 22:53:51 +0400 (Thu, 09 Jan 2014) $
// $Revision: 869 $

#ifndef __pyhrol_tuple_out_safe_impl_h__
#define __pyhrol_tuple_out_safe_impl_h__

#include "pyhrol_tuple_out.h"
#include "pyhrol_tuple_base_safe.h"

namespace pyhrol
{

class TupleOutSafeImpl: public TupleBaseSafe, public TupleOut
{
  public:
    TupleOutSafeImpl(const options /*_opts*/);
    TupleOutSafeImpl(const TupleOutSafeImpl &);

    virtual void add(char, const char *name);
    virtual void add(int8_t, const char *name);
    virtual void add(uint8_t, const char *name);
    virtual void add(bool, const char *name);
    virtual void add(const char *, const char *name);
    virtual void add(int16_t, const char *name);
    virtual void add(uint16_t, const char *name);
    virtual void add(int32_t, const char *name);
    virtual void add(uint32_t, const char *name);
    virtual void add(long, const char *name);
    virtual void add(unsigned long, const char *name);
    virtual void add(long long, const char *name);
    virtual void add(unsigned long long, const char *name);
    virtual void add(float, const char *name);
    virtual void add(double, const char *name);
    virtual void add(const Py_complex *, const char *name);
    virtual void add(const PyObject *, const char *name);
    virtual void add(const Py_UNICODE *, const char *name);
    virtual void add(out_cconv_t, const char *name);
    virtual void add(const anything *, const char *name);
};

}; //namespace pyhrol

#endif //__pyhrol_tuple_out_safe_impl_h__
