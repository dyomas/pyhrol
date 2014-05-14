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

// $Date: 2013-12-04 12:48:11 +0400 (Wed, 04 Dec 2013) $
// $Revision: 855 $

#ifndef __pyhrol_ptr_hpp__
#define __pyhrol_ptr_hpp__

#include "pyhrol_ptr.h"
#include "pyhrol_common.h"

namespace pyhrol
{

template <typename T> T *Ptr<T>::operator ->() const
{
  if (!m_obj)
  {
    throw NullPointer();
  }
  return m_obj;
}

template <typename T> Ptr<T>::operator T &() const
{
  if (!m_obj)
  {
    throw NullPointer();
  }
  return *m_obj;
}

template <typename T> Ptr<T>::operator PyObject *() const
{
  if (!m_self)
  {
    throw NullPointer();
  }
  return m_self;
}

template <typename T> T &Ptr<T>::operator *() const
{
  if (!m_obj)
  {
    throw NullPointer();
  }
  return *m_obj;
}

template <typename T> T *Ptr<T>::obj() const
{
  if (!m_obj)
  {
    throw NullPointer();
  }
  return m_obj;
}

template <typename T> PyObject *Ptr<T>::self() const
{
  if (!m_self)
  {
    throw NullPointer();
  }
  return m_self;
}

template <typename T> Ptr<T> &Ptr<T>::operator =(const Ptr<T> &cp)
{
  m_obj = cp.m_obj;
  m_self = cp.m_self;
  return *this;
}

template <typename T> Ptr<T>::Ptr()
  : m_obj(NULL)
  , m_self(NULL)
{
}

template <typename T> Ptr<T>::Ptr(T *obj, PyObject *self)
  : m_obj(obj)
  , m_self(self)
{
}

template <typename T> Ptr<T>::Ptr(const Ptr<T> &cp)
  : m_obj(cp.m_obj)
  , m_self(cp.m_self)
{
}

} //namespace pyhrol

#endif //__pyhrol_ptr_hpp__

