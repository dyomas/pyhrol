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

// $Date: 2015-07-20 23:24:41 +0300 (Пн., 20 июля 2015) $
// $Revision: 1047 $

#include <stdio.h>
#include <pyport.h>
#include <object.h>
#include <complexobject.h>
#include <unicodeobject.h>

#include "pyhrol_tuple_out_default_impl.h"

namespace pyhrol
{

TupleOutDefaultImpl::TupleOutDefaultImpl(const char *_description, const options _opts)
  : TupleBase(_description, _opts)
{
}

TupleOutDefaultImpl::TupleOutDefaultImpl(const TupleOutDefaultImpl &cp)
  : TupleBase(cp)
{
}

void TupleOutDefaultImpl::add(char value, const char *name)
{
  //c (string of length 1) [char]
  m_add_format_unit('c', "char", "str[0]", name, &value, sizeof(char));
}

void TupleOutDefaultImpl::add(int8_t value, const char *name)
{
  //b (integer) [char]
  m_add_format_unit('b', "int8_t", "int", name, &value, sizeof(int8_t));
}

void TupleOutDefaultImpl::add(uint8_t value, const char *name)
{
  //b (integer) [char]
  m_add_format_unit('b', "uint8_t", "int", name, &value, sizeof(uint8_t));
}

void TupleOutDefaultImpl::add(bool value, const char *name)
{
  //B (integer) [unsigned char]
  m_add_format_unit('B', "bool", "int", name, &value, sizeof(bool));
}

void TupleOutDefaultImpl::add(const char *value, const char *name)
{/*
  s (string) [char *]
  s# (string) [char *, int]
  z (string or None) [char *]
  z# (string or None) [char *, int]*/
  m_add_format_unit('s', "char */*nullable*/", "str", name, value, 0);
}

void TupleOutDefaultImpl::add(int16_t value, const char *name)
{
  //h (integer) [short int]
  m_add_format_unit('h', "int16_t", "int", name, &value, sizeof(int16_t));
}

void TupleOutDefaultImpl::add(uint16_t value, const char *name)
{
  //H (integer) [unsigned short int]
  m_add_format_unit('H', "uint16_t", "int", name, &value, sizeof(uint16_t));
}

void TupleOutDefaultImpl::add(int32_t value, const char *name)
{
  //i (integer) [int]
  m_add_format_unit('i', "int32_t", "int", name, &value, sizeof(int32_t));
}

void TupleOutDefaultImpl::add(uint32_t value, const char *name)
{
  //I (integer/long) [unsigned int]
  m_add_format_unit('I', "uint32_t", "int", name, &value, sizeof(uint32_t));
}

void TupleOutDefaultImpl::add(long value, const char *name)
{
  //l (integer) [long int]
  m_add_format_unit('l', "long_t", "int", name, &value, sizeof(long));
}

void TupleOutDefaultImpl::add(unsigned long value, const char *name)
{
  //k (integer/long) [unsigned long]
  m_add_format_unit('k', "ulong_t", "int", name, &value, sizeof(unsigned long));
}

void TupleOutDefaultImpl::add(long long int value, const char *name)
{
  //L (long) [PY_LONG_LONG]
  m_add_format_unit('L', "int64_t", "int", name, &value, sizeof(long long int));
}

void TupleOutDefaultImpl::add(unsigned long long int value, const char *name)
{
  //K (long) [unsigned PY_LONG_LONG]
  m_add_format_unit('K', "uint64_t", "int", name, &value, sizeof(unsigned long long int));
}

void TupleOutDefaultImpl::add(float value, const char *name)
{
  //f (float) [float]
  m_add_format_unit('f', "float", "float", name, &value, sizeof(float));
}

void TupleOutDefaultImpl::add(double value, const char *name)
{
  //d (float) [double]
  m_add_format_unit('d', "double", "float", name, &value, sizeof(double));
}

void TupleOutDefaultImpl::add(const Py_complex *value, const char *name)
{
  //D (complex) [Py_complex *]
  m_add_format_unit('D', "Py_complex", "complex", name, value, 0);
}

void TupleOutDefaultImpl::add(const PyObject *value, const char *name)
{
  //O (object) [PyObject *]
  //S (object) [PyObject *]
  //N (object) [PyObject *]
  m_add_format_unit(opts & optIncrementReferenceCount ? 'O' : 'N', "PyObject", "object", name, value, 0);
}

void TupleOutDefaultImpl::add(const Py_UNICODE *value, const char *name)
{
  //u (Unicode string) [Py_UNICODE *]
  //u# (Unicode string) [Py_UNICODE *, int]
  m_add_format_unit('u', "Py_UNICODE", "unicode", name, value, 0);
}

void TupleOutDefaultImpl::add(out_cconv_t converter, const char *name)
{
  //O& (object) [converter, anything]
  m_add_format_unit('&', "out_cconv_t", "object", name, NULL, 0);
}

void TupleOutDefaultImpl::add(const anything *p, const char *name)
{
  /* NOTE TODO
  If this argument used unpaired, there is no error in signature,
  but guaranteed SIGSEGV because of unsafe conversion without out_cconv_t
  Possible workaround is using temporary specifier '@', which replaced
  with 'O' in case of paired converter shown up or causes tuple invalidation
  otherwise
  */
  //O& (object) [converter, anything]
  m_add_format_unit('O', "void *", "object", name, p, 0);
}

/* NOTE Following datatypes unsupported:
n (int) [Py_ssize_t]*/

} //namespace pyhrol
