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

// $Date: 2013-09-18 02:14:07 +0400 (Ср., 18 сент. 2013) $
// $Revision: 785 $

#ifndef __self_addressed_cstring_hpp__
#define __self_addressed_cstring_hpp__

#include <stdlib.h>
#include <string.h>
#include <stdexcept>

#include "self_addressed_cstring.h"


template <typename T> char *SelfAddressedCstringBase<T>::m_allocate(const size_t length, const T *ptr)
{
  m_storage = static_cast<char *>(realloc(m_storage, length + sizeof(T *)));
  if (!m_storage)
  {
    throw std::bad_alloc();
  }
  *reinterpret_cast<size_t *>(m_storage) = reinterpret_cast<size_t>(ptr);
  return m_storage + sizeof(T *);
}

template <typename T> const char *SelfAddressedCstringBase<T>::get() const
{
  return m_storage ? m_storage + sizeof(T *) : NULL;
}

template <typename T> void SelfAddressedCstringBase<T>::get(char *&retval) const
{
  retval = m_storage ? m_storage + sizeof(T *) : NULL;
}

template <typename T> void SelfAddressedCstringBase<T>::get(const char *&retval) const
{
  retval = m_storage ? m_storage + sizeof(T *) : NULL;
}

template <typename T> const T *SelfAddressedCstringBase<T>::construct_const(const char *data)
{
  const T *ret_val = NULL;
  if (data)
  {
    ret_val = reinterpret_cast<const T *>(*reinterpret_cast<const size_t *>(data - sizeof(T *)));
  }
  return ret_val;
}

template <typename T> T *SelfAddressedCstringBase<T>::construct(const char *data)
{
  T *ret_val = NULL;
  if (data)
  {
    ret_val = reinterpret_cast<T *>(*reinterpret_cast<const size_t *>(data - sizeof(T *)));
  }
  return ret_val;
}

template <typename T> SelfAddressedCstringBase<T>::SelfAddressedCstringBase()
  : m_storage(NULL)
{
}

template <typename T> SelfAddressedCstringBase<T>::~SelfAddressedCstringBase()
{
  free(m_storage);
}

#endif //__self_addressed_cstring_hpp__
