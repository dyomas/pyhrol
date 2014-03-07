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

// $Date: 2013-05-21 13:23:01 +0400 (Tue, 21 May 2013) $
// $Revision: 718 $

#include <stdlib.h>
#include "array_void.h"
#ifdef _DEBUG_UTILS
#include <iostream>
#include <iomanip>
#include "silma.h"

using namespace std;
#endif //_DEBUG_UTILS


const size_t
    ArrayVoid::index_capacity_default = 64
  , ArrayVoid::storage_capacity_default = 1024
;

ArrayVoid::ArrayVoid(const size_t index_capacity, const size_t storage_capacity)
  : m_index_capacity(index_capacity)
  , m_index_size(0)
  , m_storage_capacity(storage_capacity)
  , m_storage_size(0)
  , m_index(malloc(m_index_capacity * sizeof(size_t)))
  , m_storage(malloc(m_storage_capacity))
{
}

ArrayVoid::ArrayVoid(const ArrayVoid &cp)
  : m_index_capacity(cp.m_index_capacity)
  , m_index_size(cp.m_index_size)
  , m_storage_capacity(cp.m_storage_capacity)
  , m_storage_size(cp.m_storage_size)
  , m_index(malloc(cp.m_index_capacity * sizeof(size_t)))
  , m_storage(malloc(cp.m_storage_capacity))
{
  memcpy(m_index, cp.m_index, m_index_size * sizeof(size_t));
  memcpy(m_storage, cp.m_storage, m_storage_size);
}

ArrayVoid::~ArrayVoid()
{
  free(m_index);
  free(m_storage);
}

#ifdef _DEBUG_UTILS
void ArrayVoid::__report(std::ostream &os, const uint32_t /*level*/)
{
  size_t const *pindex = static_cast<size_t const *>(m_index);
  char const *pdata = static_cast<char const *>(m_storage);

  Silma::silmaParams sp;
  sp.escapes = true;

  os
    << "Offsets:" << endl
  ;

  for (size_t pos = 0; pos < m_index_size; pos ++)
  {
    os
      << setw(5) << pos << setw(5) << pindex[pos] << endl
    ;
  }

  os
    << "Data:" << endl
  ;

  size_t offset_prev = 0;
  for (size_t pos = 0; pos < m_index_size; pos ++)
  {
    const size_t offset = pindex[pos];
    os
      << setw(5) << pos << setw(5) << offset_prev << setw(5) << offset << ' ' << Silma(&pdata[offset_prev], offset - offset_prev, &sp) << endl
    ;
    offset_prev = offset;
  }

  os
    << "Storage:" << endl
    << "  " << Silma(m_storage, m_storage_size, &sp) << endl
    << "Positions:" << endl
    << "  m_index_capacity = " << m_index_capacity << endl
    << "  m_index_size = " << m_index_size << endl
    << "  m_storage_capacity = " << m_storage_capacity << endl
    << "  m_storage_size = " << m_storage_size << endl
  ;
}
#endif //_DEBUG_UTILS

const size_t ArrayVoid::size() const
{
  return m_index_size;
}

void const *ArrayVoid::get(size_t &length, const size_t pos) const
{
  void const *ret_val = NULL;

  if (pos < m_index_size)
  {
    size_t const *pindex = static_cast<size_t const *>(m_index);
    const size_t offset = pos ? pindex[pos - 1] : 0;
    length = pindex[pos] - offset;
    ret_val = &static_cast<char const *>(m_storage)[offset];
  }
  return ret_val;
}

void ArrayVoid::clear()
{
  m_index_size = m_storage_size = 0;
}

void ArrayVoid::append(void const *src, const size_t length)
{
  m_append(src, length);
}

void ArrayVoid::set(void const *src, const size_t length)
{
  m_index_size = m_storage_size = 0;
  m_append(src, length);
}

void ArrayVoid::m_append(void const *data, const size_t length)
{
  size_t tmp = m_storage_capacity;

  while (m_storage_size + length > tmp)
  {
    tmp *= 2;
  }
  //Увеличить размер хранилища строк
  if (tmp > m_storage_capacity)
  {
    m_storage = realloc(m_storage, tmp);
    m_storage_capacity = tmp;
  }
  //Скопировать данные
  if (length)
  {
    memcpy(&static_cast<char *>(m_storage)[m_storage_size], data, length);
    m_storage_size += length;
  }

  //Увеличить размер индекса
  if (m_index_size + 1 >= m_index_capacity)
  {
    m_index_capacity *= 2;
    m_index = realloc(m_index, m_index_capacity * sizeof(size_t));
  }
  //Разместить новый указатель в индексе
  (static_cast<size_t *>(m_index))[m_index_size ++] = m_storage_size;
}
