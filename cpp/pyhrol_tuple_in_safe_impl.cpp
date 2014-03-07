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

// $Date: 2014-02-06 00:23:12 +0400 (Thu, 06 Feb 2014) $
// $Revision: 886 $

#include <sstream>
#include <typeinfo>
#include <stdexcept>
#include <unistd.h>
#include <pyport.h>
#include <object.h>
#include <complexobject.h>
#include <unicodeobject.h>
#include <stringobject.h>

#include "demangling.h"
#include "pyhrol_tuple_in_safe_impl.h"

namespace pyhrol
{

TupleInSafeImpl::TupleInSafeImpl(const options _opts, const int8_t _required)
  : TupleBaseSafe(_opts, _required)
{
}

TupleInSafeImpl::TupleInSafeImpl(const TupleInSafeImpl &cp)
  : TupleBaseSafe(cp)
{
}

void TupleInSafeImpl::add(char */*value*/, const char *name)
{
  //c (string of length 1) [char]
  m_add_format_unit('c', name);
}

void TupleInSafeImpl::add(const char */*value*/, const char *name)
{
  //e[st] (string, Unicode or character buffer compatible object) [const char *encoding, char **buffer]
  //e[st]# (string, Unicode or character buffer compatible object) [const char *encoding, char **buffer, int *buffer_length]
  m_add_format_unit('e', name);
}

void TupleInSafeImpl::add(char **value, const char *name)
{
/*
  e[st] (string, Unicode or character buffer compatible object) [const char *encoding, char **buffer]
  e[st]# (string, Unicode or character buffer compatible object) [const char *encoding, char **buffer, int *buffer_length]*/
  if (opts & optNotRecode)
  {
    m_add_format_unit('t', name);
  }
  else
  {
    m_add_format_unit('s', name);
  }
}

void TupleInSafeImpl::add(const char **/*value*/, const char *name)
{
/*
  s (string or Unicode) [const char *]
  s# (string, Unicode or any read buffer compatible object) [const char *, int (or Py_ssize_t, see below)]
  z (string, Unicode or None) [const char *]
  z# (string, Unicode, None or any read buffer compatible object) [const char *, int]
  es (string, Unicode or character buffer compatible object) [const char *encoding, char **buffer]
  es# (string, Unicode or character buffer compatible object) [const char *encoding, char **buffer, int *buffer_length]*/
  if (opts & optNoneEnabled)
  {
    m_add_format_unit('z', name);
  }
  else
  {
    m_add_format_unit('s', name);
  }
}

void TupleInSafeImpl::add(uint8_t */*value*/, const char *name)
{
  //b (integer) [unsigned char]
  m_add_format_unit('b', name);
}

void TupleInSafeImpl::add(bool */*value*/, const char *name)
{
  //B (integer) [unsigned char]
  m_add_format_unit('B', name);
}

void TupleInSafeImpl::add(int16_t */*value*/, const char *name)
{
  //h (integer) [short int]
  m_add_format_unit('h', name);
}

void TupleInSafeImpl::add(uint16_t */*value*/, const char *name)
{
  //H (integer) [unsigned short int]
  m_add_format_unit('H', name);
}

void TupleInSafeImpl::add(int32_t */*value*/, const char *name)
{
  //i (integer) [int]
  m_add_format_unit('i', name);
}

void TupleInSafeImpl::add(uint32_t */*value*/, const char *name)
{
  //I (integer) [unsigned int]
  m_add_format_unit('I', name);
}

void TupleInSafeImpl::add(long */*value*/, const char *name)
{
  //l (integer) [long int]
  m_add_format_unit('l', name);
}

void TupleInSafeImpl::add(unsigned long */*value*/, const char *name)
{
  //k (integer) [unsigned long]
  m_add_format_unit('k', name);
}

void TupleInSafeImpl::add(long long int */*value*/, const char *name)
{
  //L (integer) [PY_LONG_LONG]
  m_add_format_unit('L', name);
}

void TupleInSafeImpl::add(unsigned long long int */*value*/, const char *name)
{
  //K (integer) [unsigned PY_LONG_LONG]
  m_add_format_unit('K', name);
}

void TupleInSafeImpl::add(float */*value*/, const char *name)
{
  //f (float) [float]
  m_add_format_unit('f', name);
}

void TupleInSafeImpl::add(double */*value*/, const char *name)
{
  //d (float) [double]
  m_add_format_unit('d', name);
}

void TupleInSafeImpl::add(Py_complex */*value*/, const char *name)
{
  //D (complex) [Py_complex *]
  m_add_format_unit('D', name);
}

void TupleInSafeImpl::add(PyObject **/*value*/, const char *name)
{
  //O (object) [PyObject *]
  m_add_format_unit('O', name);
}

void TupleInSafeImpl::add(PyTypeObject */*value*/, const char *name)
{
  //O! (object) [typeobject, PyObject *]
  m_add_format_unit('!', name);
}

void TupleInSafeImpl::add(PyUnicodeObject **/*value*/, const char *name)
{
  //U (Unicode string) [PyUnicodeObject*]
  m_add_format_unit('U', name);
}

void TupleInSafeImpl::add(PyStringObject **value, const char *name)
{
  //S (string) [PyStringObject *]
  m_add_format_unit('S', name);
}

void TupleInSafeImpl::add(Py_UNICODE **value, const char *name)
{
  //u (Unicode) [Py_UNICODE *]
  //u# (Unicode) [Py_UNICODE *, int]
  m_add_format_unit('u', name);
}

void TupleInSafeImpl::add(Py_buffer *value, const char *name)
{
  //s* (string, Unicode, or any buffer compatible object) [Py_buffer]
  //z* (string, Unicode, None or any buffer compatible object) [Py_buffer]
  if (opts & optNoneEnabled)
  {
    m_add_format_unit('z', name, '*');
  }
  else
  {
    m_add_format_unit('s', name, '*');
  }
}

void TupleInSafeImpl::add(in_conv_t /*converter*/, const char *name)
{
  //O& (object) [converter, anything]
  m_add_format_unit('&', NULL);
}

void TupleInSafeImpl::add(const anything *, const char *name)
{
  //O& (object) [converter, anything]
  m_add_format_unit('O', name);
}

}; //namespace pyhrol

