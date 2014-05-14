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

// $Date: 2014-02-27 22:19:34 +0400 (Thu, 27 Feb 2014) $
// $Revision: 896 $

#include <iostream>
#include <stdio.h>
#include <pyport.h>
#include <object.h>
#include <unicodeobject.h>
#include <pyerrors.h>
#include <pyarena.h>
#include <pystate.h>
#include <pythonrun.h>
#include <modsupport.h>

#include "nullable.hpp"
#include "pyhrol_common.h"
#include "pyhrol_container.h"

using namespace std;
using namespace pyhrol;

bool _G_signature_hack_enabled = getenv("__PY_HACK_SIGNATURES");

void pyhrol_init(const char *module_name)
{
  Container &c = Container::container();

  c.indexate(module_name);

  if (getenv("__PY_SHOW_CALLS"))
  {
    c.show_calls(cout);
  }

  try
  {
    c.check_calls();
  }
  catch (const Container::WrappersReused &ex)
  {
    PyErr_Format(PyExc_AttributeError, "%s (%s)", ex.what(), __func__);
  #ifdef _DEBUG_PYTHON
    PyErr_PrintEx(0);
    PyErr_Clear();
  #else
    return;
  #endif //_DEBUG_PYTHON
  }

#ifndef PYHROL_SAFE_MODE
  c.describe_tuples();
#endif //PYHROL_SAFE_MODE

  PyObject *module;
  if (!(module = Py_InitModule(module_name, c.functions())))
  {
    return;
  }

  Container::types_range_t types = c.types();
  while (types.first != types.second)
  {
    if (PyType_Ready(*types.first) < 0)
    {
      return;
    }
    types.first ++;
  }

  types = c.types();
  while (types.first != types.second)
  {
    PyTypeObject *ptype = *types.first;
    Py_INCREF(ptype);
    PyModule_AddObject(module, isolate_type_name(ptype->tp_name), reinterpret_cast<PyObject *>(ptype));
    types.first ++;
  }
}

