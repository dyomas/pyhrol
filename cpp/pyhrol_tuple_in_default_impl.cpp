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

// $Date: 2014-04-04 16:35:38 +0400 (Fri, 04 Apr 2014) $
// $Revision: 906 $

#include <sstream>
#include <typeinfo>
#include <stdexcept>
#include <sys/types.h>
#include <pyport.h>
#include <object.h>
#include <complexobject.h>
#include <unicodeobject.h>
#include <stringobject.h>

#include "demangling.h"
#include "pyhrol_tuple_in_default_impl.h"

namespace pyhrol
{

TupleInDefaultImpl::TupleInDefaultImpl(const char *_description, const options _opts, const int8_t _required)
  : TupleBase(_description, _opts, _required)
{
}

TupleInDefaultImpl::TupleInDefaultImpl(const TupleInDefaultImpl &cp)
  : TupleBase(cp)
{
}

void TupleInDefaultImpl::add(char *value, const char *name)
{
  //c (string of length 1) [char]
  m_add_format_unit('c', "char", name, value, m_arg_is_optional() ? sizeof(char) : 0);
}

void TupleInDefaultImpl::add(const char *value, const char *name)
{
  //e[st] (string, Unicode or character buffer compatible object) [const char *encoding, char **buffer]
  //e[st]# (string, Unicode or character buffer compatible object) [const char *encoding, char **buffer, int *buffer_length]
  m_add_format_unit('e', "const char *", name, value, value ? strlen(value) : 0);
}

void TupleInDefaultImpl::add(char **value, const char *name)
{
  const size_t length = (m_arg_is_optional() && *value ? strlen(*value) : 0);
/*
  e[st] (string, Unicode or character buffer compatible object) [const char *encoding, char **buffer]
  e[st]# (string, Unicode or character buffer compatible object) [const char *encoding, char **buffer, int *buffer_length]*/
  if (opts & optNotRecode)
  {
    m_add_format_unit('t', "char *", name, *value, length);
  }
  else
  {
    m_add_format_unit('s', "char */*not null*/", name, *value, length);
  }
}

void TupleInDefaultImpl::add(const char **value, const char *name)
{
  const size_t length = (m_arg_is_optional() && *value ? strlen(*value) : 0);
/*
  s (string or Unicode) [const char *]
  s# (string, Unicode or any read buffer compatible object) [const char *, int (or Py_ssize_t, see below)]
  z (string, Unicode or None) [const char *]
  z# (string, Unicode, None or any read buffer compatible object) [const char *, int]
  es (string, Unicode or character buffer compatible object) [const char *encoding, char **buffer]
  es# (string, Unicode or character buffer compatible object) [const char *encoding, char **buffer, int *buffer_length]*/
  if (opts & optNoneEnabled)
  {
    m_add_format_unit('z', "char */*nullable*/", name, *value, length);
  }
  else
  {
    m_add_format_unit('s', "char */*not null*/", name, *value, length);
  }
}

void TupleInDefaultImpl::add(uint8_t *value, const char *name)
{
  //b (integer) [unsigned char]
  m_add_format_unit('b', "uint8_t", name, value, m_arg_is_optional() ? sizeof(uint8_t) : 0);
}

void TupleInDefaultImpl::add(bool *value, const char *name)
{
  //B (integer) [unsigned char]
  m_add_format_unit('B', "bool", name, value, m_arg_is_optional() ? sizeof(bool) : 0);
}

void TupleInDefaultImpl::add(int16_t *value, const char *name)
{
  //h (integer) [short int]
  m_add_format_unit('h', "int16_t", name, value, m_arg_is_optional() ? sizeof(int16_t) : 0);
}

void TupleInDefaultImpl::add(uint16_t *value, const char *name)
{
  //H (integer) [unsigned short int]
  m_add_format_unit('H', "uint16_t", name, value, m_arg_is_optional() ? sizeof(uint16_t) : 0);
}

void TupleInDefaultImpl::add(int32_t *value, const char *name)
{
  //i (integer) [int]
  m_add_format_unit('i', "int32_t", name, value, m_arg_is_optional() ? sizeof(int32_t) : 0);
}

void TupleInDefaultImpl::add(uint32_t *value, const char *name)
{
  //I (integer) [unsigned int]
  m_add_format_unit('I', "uint32_t", name, value, m_arg_is_optional() ? sizeof(uint32_t) : 0);
}

void TupleInDefaultImpl::add(long *value, const char *name)
{
  //l (integer) [long int]
  m_add_format_unit('l', "long_t", name, value, m_arg_is_optional() ? sizeof(long) : 0);
}

void TupleInDefaultImpl::add(unsigned long *value, const char *name)
{
  //k (integer) [unsigned long]
  m_add_format_unit('k', "ulong_t", name, value, m_arg_is_optional() ? sizeof(unsigned long) : 0);
}

void TupleInDefaultImpl::add(long long int *value, const char *name)
{
  //L (integer) [PY_LONG_LONG]
  m_add_format_unit('L', "int64_t", name, value, m_arg_is_optional() ? sizeof(int64_t) : 0);
}

void TupleInDefaultImpl::add(unsigned long long int *value, const char *name)
{
  //K (integer) [unsigned PY_LONG_LONG]
  m_add_format_unit('K', "uint64_t", name, value, m_arg_is_optional() ? sizeof(uint64_t) : 0);
}

void TupleInDefaultImpl::add(float *value, const char *name)
{
  //f (float) [float]
  m_add_format_unit('f', "float", name, value, m_arg_is_optional() ? sizeof(float) : 0);
}

void TupleInDefaultImpl::add(double *value, const char *name)
{
  //d (float) [double]
  m_add_format_unit('d', "double", name, value, m_arg_is_optional() ? sizeof(double) : 0);
}

void TupleInDefaultImpl::add(Py_complex *value, const char *name)
{
  //D (complex) [Py_complex *]
  m_add_format_unit('D', "Py_complex", name, value, 0);
}

void TupleInDefaultImpl::add(PyObject **value, const char *name)
{
  //O (object) [PyObject *]
  m_add_format_unit('O', "PyObject", name, *value, 0);
}

void TupleInDefaultImpl::add(PyTypeObject *value, const char *name)
{
  //O! (object) [typeobject, PyObject *]
  m_add_format_unit('!', isolate_type_name(value->tp_name), name, value, sizeof(PyTypeObject));
}

void TupleInDefaultImpl::add(PyUnicodeObject **value, const char *name)
{
  //U (Unicode string) [PyUnicodeObject*]
  m_add_format_unit('U', "PyUnicodeObject", name, *value, 0);
}

void TupleInDefaultImpl::add(PyStringObject **value, const char *name)
{
  //S (string) [PyStringObject *]
  m_add_format_unit('S', "PyStringObject", name, *value, 0);
}

void TupleInDefaultImpl::add(Py_UNICODE **value, const char *name)
{
  //u (Unicode) [Py_UNICODE *]
  //u# (Unicode) [Py_UNICODE *, int]
  m_add_format_unit('u', "Py_UNICODE", name, *value, 0);
}

void TupleInDefaultImpl::add(Py_buffer *value, const char *name)
{
  //s* (string, Unicode, or any buffer compatible object) [Py_buffer]
  //z* (string, Unicode, None or any buffer compatible object) [Py_buffer]
  if (opts & optNoneEnabled)
  {
    m_add_format_unit('z', "Py_buffer", name, value, 0, '*');
  }
  else
  {
    m_add_format_unit('s', "Py_buffer", name, value, 0, '*');
  }
}

void TupleInDefaultImpl::add(in_conv_t converter, const char *name)
{
  //O& (object) [converter, anything]
  m_add_format_unit('&', "in_conv_t", name, NULL, 0);
}

void TupleInDefaultImpl::add(const anything *p, const char *name)
{
  /* NOTE TODO
  If this argument used unpaired, there is no error in signature,
  but guaranteed SIGSEGV because of unsafe conversion without in_conv_t
  Possible workaround is using temporary specifier '@', which replaced
  with 'O' in case of paired converter shown up or causes tuple invalidation
  otherwise
  */
  //O& (object) [converter, anything]
  m_add_format_unit('O', "void *", name, p, 0);
}

/* NOTE Following datatypes unsupported:
n (integer) [Py_ssize_t]
t# (read-only character buffer) [char *, int]
w (read-write character buffer) [char *]
w# (read-write character buffer) [char *, Py_ssize_t]
w* (read-write byte-oriented buffer) [Py_buffer]
*/

} //namespace pyhrol

