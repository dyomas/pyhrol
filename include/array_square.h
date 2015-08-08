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

// $Date: 2012-10-21 01:22:26 +0400 (Вс., 21 окт. 2012) $
// $Revision: 662 $

#ifndef __array_square_h__
#define __array_square_h__

//Двумерный строковый массив. Применим для таких системных вызовов как exec*

#include <stdint.h>
#include <iostream>
#include <vector>


/**
Классический массив строк char*[], размер которого определяется нулевым указателем.
Размер не ограничен, возможна только запись в конец массива или полная переинициализация.
Данные хранит в двух массивах, из которых один -- собственно строки, другой -- индекс.
*/
class ArraySquare
{
  static const size_t
      m_index_capacity_initial
    , m_storage_capacity_initial
  ;
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
  void m_append(const char *, const size_t);
  public:
  #ifdef _DEBUG
    void __report(std::ostream &, const uint32_t /*level*/ = 0);
  #endif //_DEBUG
    const size_t size() const;
    char const * const *get() const;
    void linearize(std::string &, const char /*delimiter*/ = ' ') const;
    const std::string linearize(const char /*delimiter*/ = ' ') const;
    const char * operator [](const size_t) const;
    /**
    @return ненулевой указатель второго порядка
    */
    operator char * const * () const;
    operator char const * const * () const;

    void clear();
    void append(const char *);
    void append(const int32_t);
    void append(const uint32_t);
    void append(const std::string &);
    void append(char const *, const size_t);
    void append(const std::vector<std::string> &);
    void append(char const * const *);
    void set(const std::string &);
    void set(const std::vector<std::string> &);
    void set(char const * const *);

    ArraySquare();
    ArraySquare(const std::vector<std::string> &);
    ArraySquare(char const * const *);
    ArraySquare(const ArraySquare &);
    ~ArraySquare();
};

#endif //__array_square_h__

