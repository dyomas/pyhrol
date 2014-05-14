/*
 *   Copyright (c) 2013, 2014, Pyhrol, pyhrol@rambler.ru
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

// $Date: 2013-05-21 13:23:01 +0400 (Tue, 21 May 2013) $
// $Revision: 718 $

#ifndef __array_void_h__
#define __array_void_h__


#include <string.h>
#ifdef _DEBUG_UTILS
#include <stdint.h>
#include <ostream>
#endif //_DEBUG_UTILS
//#include ""

//#define
/**
Массив абстрактных указателей void * с индексом
Размер не ограничен, возможна только запись в конец массива или полная переинициализация.
Данные хранит в двух массивах, из которых один -- собственно данные, другой -- индекс.
*/
class ArrayVoid
{
  public:
    static const size_t
        index_capacity_default
      , storage_capacity_default
    ;
    ArrayVoid(const size_t index_capacity = index_capacity_default, const size_t storage_capacity = storage_capacity_default);
    ArrayVoid(const ArrayVoid &);
    ~ArrayVoid();

  #ifdef _DEBUG_UTILS
    void __report(std::ostream &, const uint32_t /*level*/ = 0);
  #endif //_DEBUG_UTILS
    const size_t size() const;
    void const *get(size_t &length, const size_t pos) const;

    void clear();
    void append(void const *, const size_t);
    void set(void const *, const size_t);

  private:
    size_t
        m_index_capacity
      , m_index_size
      , m_storage_capacity
      , m_storage_size
    ;
    void
        *m_index
      , *m_storage
    ;
    void m_append(void const *, const size_t);
};

#endif //__array_void_h__
