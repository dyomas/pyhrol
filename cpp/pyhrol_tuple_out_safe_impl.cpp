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

// $Date: 2014-04-04 16:35:38 +0400 (Пт., 04 апр. 2014) $
// $Revision: 906 $

#include <stdio.h>
#include <pyport.h>
#include <object.h>
#include <complexobject.h>
#include <unicodeobject.h>

#include "pyhrol_tuple_out_safe_impl.h"

namespace pyhrol
{

TupleOutSafeImpl::TupleOutSafeImpl(const options _opts)
  : TupleBaseSafe(_opts)
{
}

TupleOutSafeImpl::TupleOutSafeImpl(const TupleOutSafeImpl &cp)
  : TupleBaseSafe(cp)
{
}

void TupleOutSafeImpl::add(char /*value*/, const char *name)
{
  //c (string of length 1) [char]
  m_add_format_unit('c', name);
}

void TupleOutSafeImpl::add(int8_t /*value*/, const char *name)
{
  //b (integer) [char]
  m_add_format_unit('b', name);
}

void TupleOutSafeImpl::add(uint8_t /*value*/, const char *name)
{
  //b (integer) [char]
  m_add_format_unit('b', name);
}

void TupleOutSafeImpl::add(bool /*value*/, const char *name)
{
  //B (integer) [unsigned char]
  m_add_format_unit('B', name);
}

void TupleOutSafeImpl::add(const char */*value*/, const char *name)
{/*
  s (string) [char *]
  s# (string) [char *, int]
  z (string or None) [char *]
  z# (string or None) [char *, int]*/
  m_add_format_unit('s', name);
}

void TupleOutSafeImpl::add(int16_t /*value*/, const char *name)
{
  //h (integer) [short int]
  m_add_format_unit('h', name);
}

void TupleOutSafeImpl::add(uint16_t /*value*/, const char *name)
{
  //H (integer) [unsigned short int]
  m_add_format_unit('H', name);
}

void TupleOutSafeImpl::add(int32_t /*value*/, const char *name)
{
  //i (integer) [int]
  m_add_format_unit('i', name);
}

void TupleOutSafeImpl::add(uint32_t /*value*/, const char *name)
{
  //I (integer/long) [unsigned int]
  m_add_format_unit('I', name);
}

void TupleOutSafeImpl::add(long /*value*/, const char *name)
{
  //l (integer) [long int]
  m_add_format_unit('l', name);
}

void TupleOutSafeImpl::add(unsigned long /*value*/, const char *name)
{
  //k (integer/long) [unsigned long]
  m_add_format_unit('k', name);
}

void TupleOutSafeImpl::add(long long int /*value*/, const char *name)
{
  //L (long) [PY_LONG_LONG]
  m_add_format_unit('L', name);
}

void TupleOutSafeImpl::add(unsigned long long int /*value*/, const char *name)
{
  //K (long) [unsigned PY_LONG_LONG]
  m_add_format_unit('K', name);
}

void TupleOutSafeImpl::add(float /*value*/, const char *name)
{
  //f (float) [float]
  m_add_format_unit('f', name);
}

void TupleOutSafeImpl::add(double /*value*/, const char *name)
{
  //d (float) [double]
  m_add_format_unit('d', name);
}

void TupleOutSafeImpl::add(const Py_complex */*value*/, const char *name)
{
  //D (complex) [Py_complex *]
  m_add_format_unit('D', name);
}

void TupleOutSafeImpl::add(const PyObject */*value*/, const char *name)
{
  //O (object) [PyObject *]
  //S (object) [PyObject *]
  //N (object) [PyObject *]
  m_add_format_unit(opts & optIncrementReferenceCount ? 'O' : 'N', name);
}

void TupleOutSafeImpl::add(const Py_UNICODE */*value*/, const char *name)
{
  //u (Unicode string) [Py_UNICODE *]
  //u# (Unicode string) [Py_UNICODE *, int]
  m_add_format_unit('u', name);
}

void TupleOutSafeImpl::add(out_cconv_t /*converter*/, const char *name)
{
  //O& (object) [converter, anything]
  m_add_format_unit('&', NULL);
}

void TupleOutSafeImpl::add(const anything *, const char *name)
{
  /* NOTE TODO
  If this argument used unpaired, there is no error in signature,
  but guaranteed SIGSEGV because of unsafe conversion without out_cconv_t
  Possible workaround is using temporary specifier '@', which replaced
  with 'O' in case of paired converter shown up or causes tuple invalidation
  otherwise
  */
  //O& (object) [converter, anything]
  m_add_format_unit('O', name);
}

/* NOTE Following datatypes unsupported:
n (int) [Py_ssize_t]*/

} //namespace pyhrol
