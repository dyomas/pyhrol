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

// $Date: 2013-11-10 16:07:03 +0400 (Sun, 10 Nov 2013) $
// $Revision: 832 $

#include <stdio.h>
#include <tr1/memory>
#include <pyport.h>
#include <object.h>
#include <tupleobject.h>
#include <unicodeobject.h>
#include <pyerrors.h>

#include "pyhrol_tuples.h"
#include "pyhrol_type_number_any.h"

using namespace std;

namespace pyhrol
{

bool actual_call(exceptionHandler &h, Tuples &tuples, PyObject *arg, PyObject *arg2, PyObject *arg3)
{
  PyObject *args_packed = NULL;

  if (arg)
  {
    if (arg2 && arg2 != Py_None)
    {
      if (arg3 && arg3 != Py_None)
      {
        args_packed = PyTuple_Pack(3, arg, arg2, arg3);
      }
      else
      {
        args_packed = PyTuple_Pack(2, arg, arg2);
      }
    }
    else
    {
      args_packed = PyTuple_Pack(1, arg);
    }
  }

  if (arg && !args_packed)
  {
    return false;
  }

  tuples.ubiquitous_caller(h, args_packed, NULL, !arg);
  Py_DecRef(args_packed);
  return true;
}

bool actual_call_inplace(exceptionHandler &h, Tuples &tuples, const bool is_probe, PyObject *arg, PyObject *arg2)
{
  PyObject *args_packed = NULL;

  if (arg)
  {
    if (arg2 && arg2 != Py_None)
    {
      args_packed = PyTuple_Pack(2, arg, arg2);
    }
    else
    {
      args_packed = PyTuple_Pack(1, arg);
    }
  }

  if (arg && !args_packed)
  {
    return false;
  }

  tuples.ubiquitous_caller(h, args_packed, NULL, is_probe);
  Py_DecRef(args_packed);
  return true;
}

} //namespace pyhrol
