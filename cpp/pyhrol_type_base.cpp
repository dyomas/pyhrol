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

// $Date: 2014-02-23 20:22:57 +0400 (Вс., 23 февр. 2014) $
// $Revision: 895 $

#include <stdio.h>
#include <pyport.h>
#include <object.h>
#include <unicodeobject.h>
#include <pyerrors.h>
#include <tupleobject.h>

#include "pyhrol_type_base.h"
#include "pyhrol_container.h"
#include "self_addressed_cstring.hpp"

using namespace std;

namespace pyhrol
{

const size_t register_type(PyTypeObject &type)
{
  return Container::container().register_type(type);
}

void unregister_type(size_t id)
{
  Container::container().unregister_type(id);
}

bool is_error_occured()
{
  return PyErr_Occurred();
}

PyObject *clear_on_error(PyObject *obj)
{
  PyObject *retval = NULL;
  if (PyErr_Occurred())
  {
    Py_DecRef(obj);
  }
  else
  {
    retval = obj;
  }
  return retval;
}

PyObject *pack_tuple(PyObject *arg1, PyObject *arg2)
{
  PyObject *retval = NULL;
  if (arg1)
  {
    if (arg2)
    {
      retval = PyTuple_Pack(2, arg1, arg2);
    }
    else
    {
      retval = PyTuple_Pack(1, arg1);
    }
  }
  return retval;
}

} //pyhrol
