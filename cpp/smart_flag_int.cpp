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

// $Date: 2015-04-06 21:03:10 +0300 (Пн., 06 апр. 2015) $
// $Revision: 1027 $

#include "smart_flag_int.h"

using namespace std;

template <typename T> SmartFlagNumeric<T>::SmartFlagNumeric(const ftree_t *pftree)
  : m_assigned(false)
  , m_value(0)
  , m_pftree(pftree)
{
}

template <typename T> SmartFlagNumeric<T>::SmartFlagNumeric(const ftree_t *pftree, const T value)
  : m_assigned(false)
  , m_value(value)
  , m_pftree(pftree)
{
}

template <typename T> SmartFlagNumeric<T>::SmartFlagNumeric(const SmartFlagNumeric<T> &cp)
  : m_assigned(cp.m_assigned)
  , m_value(cp.m_value)
  , m_pftree(cp.m_pftree)
{
}

template <typename T> const string SmartFlagNumeric<T>::as_string() const
{
  ostringstream os;
  m_pftree->convert(os, m_value);
  return os.str();
}

template <typename T> void SmartFlagNumeric<T>::convert(ostream &os) const
{
  m_pftree->convert(os, m_value);
}

template <typename T> const typename SmartFlagNumeric<T>::ftree_t *SmartFlagNumeric<T>::operator ->() const
{
  return m_pftree;
}

template <typename T> bool SmartFlagNumeric<T>::assigned() const
{
  return m_assigned;
}

template <typename T> const T SmartFlagNumeric<T>::get() const
{
  return m_value;
}

template <typename T> SmartFlagNumeric<T>::operator const T () const
{
  return m_value;
}

template <typename T> void SmartFlagNumeric<T>::assign(const T &i)
{
  m_pftree->check(i);
  m_value = i;
  m_assigned = true;
}

template <typename T> void SmartFlagNumeric<T>::assign(const string &s)
{
  m_value = m_pftree->convert(s);
  m_assigned = true;
}

template <typename T> SmartFlagNumeric<T> &SmartFlagNumeric<T>::operator = (const T &i)
{
  m_pftree->check(i);
  m_value = i;
  m_assigned = true;
  return *this;
}

template <typename T> SmartFlagNumeric<T> &SmartFlagNumeric<T>::operator = (const string &s)
{
  m_value = m_pftree->convert(s);
  m_assigned = true;
  return *this;
}

template <typename T> void SmartFlagNumeric<T>::clear()
{
  m_assigned = false;
  m_value = 0;
}

ostream &operator << (ostream &os, const SmartFlagInt &f)
{
  f.convert(os);
  return os;
}

ostream &operator << (ostream &os, const SmartFlagUint &f)
{
  f.convert(os);
  return os;
}

template class SmartFlagNumeric<int32_t>;
template class SmartFlagNumeric<uint32_t>;
