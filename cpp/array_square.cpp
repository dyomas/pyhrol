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

// $Date: 2012-10-21 01:22:26 +0400 (Sun, 21 Oct 2012) $
// $Revision: 662 $

#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include "array_square.h"

using namespace std;

const size_t
    ArraySquare::m_index_capacity_initial = 64
  , ArraySquare::m_storage_capacity_initial = 1024
;

void ArraySquare::m_append(const char *data, const size_t length)
{
  size_t tmp = m_storage_capacity;
  void *pstorage_old = NULL;
  char *insertion_pos;

  while (m_storage_size + length + 1 > tmp)
  {
    tmp *= 2;
  }
  //Увеличить размер хранилища строк
  if (tmp > m_storage_capacity)
  {
    pstorage_old = m_storage;
    m_storage = realloc(m_storage, tmp);
    m_storage_capacity = tmp;
  }
  //Скопировать строку и запомнить ее местоположение
  insertion_pos = static_cast<char *>(m_storage) + m_storage_size;
  memcpy(insertion_pos, data, length);
  m_storage_size += length;
  static_cast<char *>(m_storage)[m_storage_size] = '\0';
  m_storage_size += 1;

  //Переместить все указатели в индексе всвязи с переразмещением (reallocate) буфера строк
  if (pstorage_old)
  {
  #ifdef _DEBUG
/*    cout
      << "Shift " << (ssize_t)m_storage - (ssize_t)pstorage_old << " (" << m_index_size << ", " << m_storage_size << ")" << endl
    ;*/
  #endif //_DEBUG
    for (size_t pos = 0; pos < m_index_size; pos ++)
    {
      static_cast<char **>(m_index)[pos] += static_cast<char *>(m_storage) - static_cast<char *>(pstorage_old);
    }
  }

  //Увеличить размер индекса
  if (m_index_size + 1 >= m_index_capacity)
  {
    m_index_capacity *= 2;
    m_index = realloc(m_index, m_index_capacity * sizeof(char *));
  }
  //Разместить новый указатель в индексе
  static_cast<char **>(m_index)[m_index_size] = insertion_pos;
  m_index_size ++;
  //Последний элемент индекса -- всегда нулевой указатель!
  static_cast<char **>(m_index)[m_index_size] = NULL;
}

#ifdef _DEBUG
void ArraySquare::__report(ostream &os, const uint32_t /*level*/)
{
  char const * const * pindex = static_cast<char const * const *>(m_index);
  size_t pos = 0;

  os
    << "Data:" << endl
  ;

  while (*pindex)
  {
    os
      << setw(5) << pos << " \"" << *pindex << "\"" << endl
    ;
    pindex = &pindex[1];
    pos ++;
  }

  os
    << "Positions:" << endl
    << "  m_index_capacity = " << m_index_capacity << endl
    << "  m_index_size = " << m_index_size << endl
    << "  m_storage_capacity = " << m_storage_capacity << endl
    << "  m_storage_size = " << m_storage_size << endl
  ;
}
#endif //_DEBUG

char const * const * ArraySquare::get() const
{
  return static_cast<char const * const *>(m_index);
}

void ArraySquare::linearize(string &res, const char delimiter) const
{
  for (size_t pos = 0; pos < m_index_size; pos ++)
  {
    if (pos)
    {
      res += delimiter;
    }
    res += '\"';
    res += string(static_cast<char const * const *>(m_index)[pos]);
    res += '\"';
  }
}

const string ArraySquare::linearize(const char delimiter) const
{
  string res;

  for (size_t pos = 0; pos < m_index_size; pos ++)
  {
    if (pos)
    {
      res += delimiter;
    }
    res += '\"';
    res += string(static_cast<char const * const *>(m_index)[pos]);
    res += '\"';
  }
  return res;
}

const size_t ArraySquare::size() const
{
  return m_index_size;
}

const char * ArraySquare::operator [](const size_t pos) const
{
  if (pos >= m_index_size)
  {
    ostringstream ostr;
    ostr << "Index " << pos << " out of bound " << m_index_size << " in ArraySquare";
    throw out_of_range(ostr.str());
  }
  return static_cast<char const * const *>(m_index)[pos];
}

ArraySquare::operator char * const * () const
{
  return static_cast<char * const *>(m_index);
}

ArraySquare::operator char const * const * () const
{
  return static_cast<char const * const *>(m_index);
}

void ArraySquare::clear()
{
  m_index_size = m_storage_size = 0;
  /*
  NOTE Массив возвращается неизменным через операторы преобразования типа,
  поэтому при очистке необходимо занулить его первый элемент
  */
  static_cast<char **>(m_index)[0] = NULL;
}

void ArraySquare::append(const char *src)
{
  m_append(src, strlen(src));
}

void ArraySquare::append(const int32_t src)
{
  ostringstream ostr;
  ostr << src;
  m_append(ostr.str().c_str(), ostr.str().length());
}

void ArraySquare::append(const uint32_t src)
{
  ostringstream ostr;
  ostr << src;
  m_append(ostr.str().c_str(), ostr.str().length());
}

void ArraySquare::append(const string &src)
{
  m_append(src.c_str(), src.length());
}

void ArraySquare::append(char const *src, const size_t length)
{
  m_append(src, length);
}

void ArraySquare::append(const vector<string> &src)
{
  for (vector<string>::const_iterator iter = src.begin(); iter != src.end(); iter ++)
  {
    m_append(iter->c_str(), iter->length());
  }
}

void ArraySquare::append(char const * const *src)
{
  while (*src)
  {
    m_append(*src, strlen(*src));
    src = &src[1];
  }
}

void ArraySquare::set(const string &src)
{
  m_index_size = m_storage_size = 0;
  m_append(src.c_str(), src.length());
}

void ArraySquare::set(const vector<string> &src)
{
  m_index_size = m_storage_size = 0;
  for (vector<string>::const_iterator iter = src.begin(); iter != src.end(); iter ++)
  {
    m_append(iter->c_str(), iter->length());
  }
}

void ArraySquare::set(char const * const *src)
{
  m_index_size = m_storage_size = 0;
  while (*src)
  {
    m_append(*src, strlen(*src));
    src = &src[1];
  }
}

ArraySquare::ArraySquare()
  : m_index_capacity(m_index_capacity_initial)
  , m_index_size(0)
  , m_storage_capacity(m_storage_capacity_initial)
  , m_storage_size(0)
  , m_index(malloc(m_index_capacity_initial * sizeof(char *)))
  , m_storage(malloc(m_storage_capacity_initial))
{
  static_cast<char **>(m_index)[0] = NULL;
}

ArraySquare::ArraySquare(const vector<string> &src)
  : m_index_capacity(m_index_capacity_initial)
  , m_index_size(0)
  , m_storage_capacity(m_storage_capacity_initial)
  , m_storage_size(0)
  , m_index(malloc(m_index_capacity_initial * sizeof(char *)))
  , m_storage(malloc(m_storage_capacity_initial))
{
  for (vector<string>::const_iterator iter = src.begin(); iter != src.end(); iter ++)
  {
    m_append(iter->c_str(), iter->length());
  }
}

ArraySquare::ArraySquare(char const * const *src)
  : m_index_capacity(m_index_capacity_initial)
  , m_index_size(0)
  , m_storage_capacity(m_storage_capacity_initial)
  , m_storage_size(0)
  , m_index(malloc(m_index_capacity_initial * sizeof(char *)))
  , m_storage(malloc(m_storage_capacity_initial))
{
  while (*src)
  {
    m_append(*src, strlen(*src));
    src = &src[1];
  }
}

ArraySquare::ArraySquare(const ArraySquare &cp)
  : m_index_capacity(cp.m_index_capacity)
  , m_index_size(cp.m_index_size)
  , m_storage_capacity(cp.m_storage_capacity)
  , m_storage_size(cp.m_storage_size)
  , m_index(malloc(cp.m_index_capacity * sizeof(char *)))
  , m_storage(malloc(cp.m_storage_capacity))
{
  const ssize_t shift = static_cast<char *>(m_storage) - static_cast<char *>(cp.m_storage);
#ifdef _DEBUG
/*  cout
    << "Shift " << shift << endl
  ;*/
#endif //_DEBUG

  memcpy(m_storage, cp.m_storage, m_storage_size);
  for (size_t pos = 0; pos < m_index_size; pos ++)
  {
    static_cast<char **>(m_index)[pos] = static_cast<char **>(cp.m_index)[pos] + shift;
  }
  static_cast<char **>(m_index)[m_index_size] = NULL;
}

ArraySquare::~ArraySquare()
{
  free(m_index);
  free(m_storage);
}
