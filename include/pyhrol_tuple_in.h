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

#ifndef __pyhrol_tuple_in_h__
#define __pyhrol_tuple_in_h__

#include <stdint.h>
#include <unistd.h>
#include <pyport.h>
#include <object.h>
#include <complexobject.h>
#include <unicodeobject.h>
#include <stringobject.h>

namespace pyhrol
{

class TupleIn
{
  public:
    struct anything
    {
      static int converter(PyObject *args, anything *);
    private:
      virtual int m_converter(PyObject *args) = 0;
    };
    typedef int (* in_conv_t)(PyObject *, anything *);

    /* NOTE
    -- аргумент типа uint32_t, следующий за аргументом char * или Py_UNICODE * всегда воспринимается как длина
       предыдущего аргумента (т. е. становится частью формата "s#" или "u#" соответственно)
    -- вслед за аргументом типа PyTypeObject всегда должен следовать аргумент типа PyObject (формат "O!")
    -- вслед за аргументом типа in_conv_t всегда должен следовать аргумент типа anything * (формат "O&")
    -- вслед за аргументом типа const char * ('e') всегда должен следовать аргумент типа char ** (форматы "es", "es#", "et", "et#")
    */
    virtual void add(char *, const char *name) = 0;
    virtual void add(const char *, const char *name) = 0;
    virtual void add(char **, const char *name) = 0;
    virtual void add(const char **, const char *name) = 0;
    virtual void add(uint8_t *, const char *name) = 0;
    virtual void add(bool *, const char *name) = 0;
    virtual void add(int16_t *, const char *name) = 0;
    virtual void add(uint16_t *, const char *name) = 0;
    virtual void add(int32_t *, const char *name) = 0;
    virtual void add(uint32_t *, const char *name) = 0;
    virtual void add(long int *, const char *name) = 0;
    virtual void add(unsigned long int *, const char *name) = 0;
    virtual void add(long long int *, const char *name) = 0;
    virtual void add(unsigned long long int *, const char *name) = 0;
    virtual void add(float *, const char *name) = 0;
    virtual void add(double *, const char *name) = 0;
    virtual void add(Py_complex *, const char *name) = 0;
    virtual void add(PyObject **, const char *name) = 0;
    virtual void add(PyTypeObject *, const char *name) = 0;
    virtual void add(PyUnicodeObject **, const char *name) = 0;
    virtual void add(PyStringObject **, const char *name) = 0;
    virtual void add(Py_UNICODE **, const char *name) = 0;
    virtual void add(Py_buffer *, const char *name) = 0;
    virtual void add(in_conv_t, const char *name) = 0;
    virtual void add(const anything *, const char *name) = 0;
};

struct soother: TupleIn::anything
{
  virtual int m_converter(PyObject *);
};

} //namespace pyhrol

#endif //__pyhrol_tuple_in_h__
