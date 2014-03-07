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

// $Date: 2014-02-13 01:38:33 +0400 (Thu, 13 Feb 2014) $
// $Revision: 888 $

#include <stdio.h>
#include <pyport.h>
#include <object.h>
#include <unicodeobject.h>
#include <pyerrors.h>
#include <stdexcept>

#include "pyhrol_exception_handler.h"
#include "pyhrol_tuples_exceptions.h"
#include "pyhrol_common.h"
#include "pyhrol_container.h"
#ifdef PYHROL_SAFE_MODE
  #include "pyhrol_tuple_base_safe.h"
#else
  #include "pyhrol_check_calls.h"
  #include "pyhrol_tuple_base.h"
#endif //PYHROL_SAFE_MODE
using namespace std;

namespace pyhrol
{

exceptionHandler::~exceptionHandler()
{
}

bool exceptionHandler::call(exceptionHandler &h, const void *address)
{
  PyObject *error = NULL;
  string message;

  try
  {
    h.call();
  }
  catch (const ImplementationException &ex)
  {
    error = PyExc_NotImplementedError;
    message = ex.what();
  }
  //catch (const NullPointer &ex) <-- domain_error
  catch (const TypeInitException &ex)
  {
    throw;
  }
#ifdef PYHROL_SAFE_MODE
  //catch (const TupleBaseSafe::FormatException &ex) <-- AttributeException
  catch (const ParseException &ex)
  {
    //Неверные аргументы Python; только в режиме исполнения
    error = PyExc_TypeError;
    message = ex.what();
  }
#else
  //catch (const TupleBase::FormatException &ex) <-- AttributeException
  //catch (const CheckCalls::ForbiddenCall &ex) <-- AttributeException
  //catch (const ArgsException &ex) <-- AttributeException
  //catch (const OverflowException &ex) <-- overflow_error
  //catch (const StateException &ex) <-- AttributeException
  catch (const ParseException &)
  {
    throw;
  }
#endif //PYHROL_SAFE_MODE
  //catch (const UseException &ex) <-- domain_error
  //catch (const Index::NotFoundException &ex) <-- KeyException
  //catch (const Index::NonUniqueException &ex) <-- KeyException
  //catch (const Index::WrongTypeException &ex) <-- TypeException
  catch (const AttributeException &ex)
  {
    error = PyExc_AttributeError;
    message = ex.what();
  }
  catch (const KeyException &ex)
  {
    error = PyExc_KeyError;
    message = ex.what();
  }
  catch (const TypeException &ex)
  {
    error = PyExc_TypeError;
    message = ex.what();
  }

  catch (const ios_base::failure &ex)
  {
    error = PyExc_IOError;
    message = ex.what();
  }
  catch (const bad_alloc &ex)
  {
    error = PyExc_MemoryError;
    message = ex.what();
  }
  catch (const domain_error &ex)
  {
    error = PyExc_ValueError;
    message = ex.what();
  }
  //catch (const invalid_argument &ex)
  //catch (const length_error &ex)
  catch (const out_of_range &ex)
  {
    error = PyExc_LookupError;
    message = ex.what();
  }
  //catch (const logic_error &ex)
  catch (const range_error &ex)
  {
    error = PyExc_IndexError;
    message = ex.what();
  }
  catch (const overflow_error &ex)
  {
    error = PyExc_OverflowError;
    message = ex.what();
  }
  //catch (const underflow_error &ex)
  catch (const runtime_error &ex)
  {
    error = PyExc_RuntimeError;
    message = ex.what();
  }
  //catch (const bad_cast &ex)
  //catch (const bad_typeid &ex)
  catch (const exception &ex)
  {
    error = PyExc_StandardError;
    message = ex.what();
  }

  if (error)
  {
    PyErr_Format(error, "%s (%s)", message.c_str(), Container::container().context_by_address(address).c_str());
  }

  return !error;
}

}; //namespace pyhrol
