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

// $Date: 2013-11-29 03:26:15 +0400 (Fri, 29 Nov 2013) $
// $Revision: 847 $

#include "smart_flag_int.h"

using namespace std;

SmartFlagInt::SmartFlagInt(const ftree_t *pftree)
  : m_assigned(false)
  , m_value(0)
  , m_pftree(pftree)
{
}

SmartFlagInt::SmartFlagInt(const ftree_t *pftree, const int value)
  : m_assigned(false)
  , m_value(value)
  , m_pftree(pftree)
{
}

SmartFlagInt::SmartFlagInt(const SmartFlagInt &cp)
  : m_assigned(cp.m_assigned)
  , m_value(cp.m_value)
  , m_pftree(cp.m_pftree)
{
}

const string SmartFlagInt::as_string() const
{
  ostringstream os;
  m_pftree->convert(os, m_value);
  return os.str();
}

void SmartFlagInt::convert(ostream &os) const
{
  m_pftree->convert(os, m_value);
}

const SmartFlagInt::ftree_t *SmartFlagInt::operator ->() const
{
  return m_pftree;
}

bool SmartFlagInt::assigned() const
{
  return m_assigned;
}

const int SmartFlagInt::get() const
{
  return m_value;
}

SmartFlagInt::operator const int () const
{
  return m_value;
}

void SmartFlagInt::assign(const int &i)
{
  m_pftree->check(i);
  m_value = i;
  m_assigned = true;
}

void SmartFlagInt::assign(const string &s)
{
  m_value = m_pftree->convert(s);
  m_assigned = true;
}

SmartFlagInt &SmartFlagInt::operator = (const int &i)
{
  m_pftree->check(i);
  m_value = i;
  m_assigned = true;
  return *this;
}

SmartFlagInt &SmartFlagInt::operator = (const string &s)
{
  m_value = m_pftree->convert(s);
  m_assigned = true;
  return *this;
}

void SmartFlagInt::clear()
{
  m_assigned = false;
  m_value = 0;
}

ostream &operator << (ostream &os, const SmartFlagInt &f)
{
  f.convert(os);
  return os;
}
