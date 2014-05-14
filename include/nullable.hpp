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

// $Date: 2013-10-11 14:23:34 +0400 (Fri, 11 Oct 2013) $
// $Revision: 804 $

#ifndef _nullable_hpp_
#define _nullable_hpp_

#include "nullable.h"

template <class T> Nullable<T>::Nullable()
  : m_is_null(true)
{
}

template <class T> Nullable<T>::Nullable(const T &v)
  : m_value(v)
  , m_is_null(false)
{
}

template <class T> Nullable<T>::Nullable(const T *pv)
  : m_is_null(!pv)
{
  if (pv)
  {
    m_value = *pv;
  }
}

template <class T> Nullable<T>::Nullable(const Nullable<T> &cp)
  : m_value(cp.m_value)
  , m_is_null(cp.m_is_null)
{
}

template <class T> const T &Nullable<T>::getValue() const
{
  if (m_is_null)
  {
    throw std::logic_error("Null pointer dereferencing attempt");
  }
  return m_value;
}

template <class T> const T *Nullable<T>::getPointer() const
{
  return m_is_null ? NULL : &m_value;
}

template <class T> Nullable<T>::operator const T &() const
{
  if (m_is_null)
  {
    throw std::logic_error("Null pointer dereferencing attempt");
  }
  return m_value;
}

template <class T> Nullable<T>::operator const T *() const
{
  return m_is_null ? NULL : &m_value;
}

template <class T> const T &Nullable<T>::nullIf(const T &v) const
{
  if (m_is_null)
  {
    return v;
  }
  else
  {
    return m_value;
  }
}

template <class T> bool Nullable<T>::isNull() const
{
  return m_is_null;
}

template <class T> bool Nullable<T>::assigned() const
{
  return !m_is_null;
}

template <class T> Nullable<T> &Nullable<T>::operator = (const T &v)
{
  m_value = v;
  m_is_null = false;
  return *this;
}

template <class T> Nullable<T> &Nullable<T>::operator = (const T *pv)
{
  if (pv)
  {
    m_value = *pv;
  }
  m_is_null = !pv;
  return *this;
}

template <class T> void Nullable<T>::clear()
{
  m_is_null = true;
}

template <class T> void Nullable<T>::setValue(const T &v)
{
  m_value = v;
  m_is_null = false;
}

template <class T> void Nullable<T>::setPointer(const T *pv)
{
  if (pv)
  {
    m_value = *pv;
  }
  m_is_null = !pv;
}

#endif //_nullable_hpp_
