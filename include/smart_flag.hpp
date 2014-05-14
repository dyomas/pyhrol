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

// $Date: 2014-04-30 17:24:23 +0400 (Wed, 30 Apr 2014) $
// $Revision: 914 $

#ifndef _smart_flag_hpp_
#define _smart_flag_hpp_

#include "smart_flag.h"
#include "enum_shell.hpp"

template <class T> void SmartFlag<T>::convert(std::ostream &os) const
{
  m_get_tree().convert(os, m_assigned ? m_value : m_value_default);
}

template <class T> const std::string SmartFlag<T>::to_string() const
{
  std::ostringstream ostr;
  m_get_tree().convert(ostr, m_assigned ? m_value : m_value_default);
  return ostr.str();
}

template <class T> const typename SmartFlag<T>::ttree_t *SmartFlag<T>::operator ->() const
{
  return &m_get_tree();
}

template <class T> SmartFlag<T>::operator const T () const
{
  return m_assigned ? m_value : m_value_default;
}

template <class T> bool SmartFlag<T>::assigned() const
{
  return m_assigned;
}

template <class T> const T SmartFlag<T>::get() const
{
  return m_assigned ? m_value : m_value_default;
}

template <class T> SmartFlag<T> &SmartFlag<T>::operator = (const std::string &s)
{
  m_value = m_get_tree().convert(s);
  m_assigned = true;
  return *this;
}

template <class T> SmartFlag<T> &SmartFlag<T>::operator = (const T value)
{
  m_value = value;
  m_assigned = true;
  return *this;
}

template <class T> void SmartFlag<T>::clear()
{
  m_assigned = false;
  m_value = m_value_default;
}

template <class T> SmartFlag<T>::SmartFlag()
  : m_assigned(false)
  , m_value_default(static_cast<T>(0))
{
}

template <class T> SmartFlag<T>::SmartFlag(const char *_value)
  : m_assigned(true)
  , m_value(m_get_tree().convert(_value))
  , m_value_default(static_cast<T>(0))
{
}

template <class T> SmartFlag<T>::SmartFlag(const T _value)
  : m_assigned(false)
  , m_value_default(_value)
{
}

/* NOTE
const ttree_t &m_get_tree() -- реализуется конечным пользователем
{
  static ttree_t __t;
  if (!t.filled())
  {
    t.add(...);
    ...
    t.free_index();
  }
  return t;
}
*/

template <class T> std::ostream &operator << (std::ostream &os, const SmartFlag<T> &sf)
{
  sf.convert(os);
  return os;
}

#define SMART_FLAG_INSTANTIATION_BEGIN(type, var)\
  template <> const SmartFlag<type>::ttree_t &SmartFlag<type>::m_get_tree()\
  {\
    static ttree_t var;\
    if (!var.filled())\
    {

#define SMART_FLAG_INSTANTIATION_END(type, var)\
      var.free_index();\
    }\
    return var;\
  }\
  template std::ostream &operator << (std::ostream &/*os*/, const SmartFlag<type> &/*sf*/);\
  template class enum_2_string_stree<type>;\
  template class SmartFlag<type>;\

#endif //_smart_flag_hpp_
