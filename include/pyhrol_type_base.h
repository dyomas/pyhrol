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

// $Date: 2013-12-04 12:48:11 +0400 (Wed, 04 Dec 2013) $
// $Revision: 855 $

#ifndef __pyhrol_type_base_h__
#define __pyhrol_type_base_h__

#include <vector>
#include <map>
#include <ostream>

#include <pyport.h>
#include <object.h>
#include <methodobject.h>
#include <descrobject.h>

#include "pyhrol_tuples.h"
#include "pyhrol_exception_handler.h"
#include "self_addressed_cstring.h"


namespace pyhrol
{

template <class T> class TypeBase
{
public:
  typedef T endosome_t;

  struct T_struct
  {
  private:
    PyObject_HEAD;
  public:
    T endosome;
    inline operator PyObject *()
    {
      return reinterpret_cast<PyObject *>(this);
    }
  };

  struct argObject
  {
    PyTypeObject &cls;
    PyObject *pobj;

    inline argObject(PyObject *_pobj = NULL)
      : cls(m_get())
      , pobj(_pobj)
    {
    }

    inline operator T *() const
    {
      return &reinterpret_cast<typename TypeBase<T>::T_struct *>(pobj)->endosome;
    }
    inline operator T &() const
    {
      return reinterpret_cast<typename TypeBase<T>::T_struct *>(pobj)->endosome;
    }
    inline T *operator ->() const
    {
      return &reinterpret_cast<typename TypeBase<T>::T_struct *>(pobj)->endosome;
    }
    inline T *get() const
    {
      return &reinterpret_cast<typename TypeBase<T>::T_struct *>(pobj)->endosome;
    }
  };

  const char *name() const;
  operator PyTypeObject &();
  operator const PyTypeObject &() const;
  const PyTypeObject *operator ->() const;

  const size_t id;
  static const size_t endosome_offset;

  T_struct *allocate();
  void free(T_struct *) const throw();
  static T_struct *allocate_static();
  static void free_static(T_struct *) throw();

  virtual ~TypeBase();

protected:
  TypeBase(const char */*name*/, const char */*doc*/ = NULL);
  TypeBase();

  virtual const void *address(const T &) const = 0;
  virtual void constructor(T &, Tuples &) const;
  virtual void destructor(T &) const = 0;
  virtual void print(std::ostream &, const T &) const = 0;
  virtual const std::string repr(const T &) const = 0;
  virtual void str(std::ostream &, const T &) const = 0;

  PyTypeObject m_type_object;

  void m_add_method(const char */*name*/, const PyCFunction &/*_method*/, const char */*help*/, const int /*flags*/ = 0);
  void m_add_method(const char */*name*/, const PyCFunctionWithKeywords &/*_method*/, const char */*help*/, const int /*flags*/ = 0);
  void m_add_method(const char */*name*/, const PyNoArgsFunction &/*_method*/, const char */*help*/, const int /*flags*/ = 0);
  void m_add_getseter(const char */*name*/, const getter &/*_get*/, const setter &/*_set*/, const char */*doc*/);

  static TypeBase &m_get(TypeBase * = NULL);

private:
  TypeBase(const TypeBase &);
  TypeBase &operator= (const TypeBase &);

  void m_init(const char */*name*/, const char */*doc*/);

  static PyObject *mediator_constructor(PyTypeObject */*type*/, PyObject */*args*/, PyObject */*kwds*/);


  struct call_constructor: exceptionHandler
  {
    const bool is_probe;
    Tuples &args;
    PyTypeObject *ptype;
    PyObject *retval;
    const char * const signature;
    virtual void call();
    call_constructor(Tuples &, PyTypeObject *, const char *_signature, const bool);
  };

  struct call_destructor: exceptionHandler
  {
    PyObject *self;
    const char * const signature;
    const TypeBase<T> &type;
    virtual void call();
    call_destructor(PyObject *, const char *_signature);
  };

  struct call_print: exceptionHandler
  {
    const T &self;
    FILE *fp;
    const char * const signature;
    const TypeBase<T> &type;
    int retval;
    virtual void call();
    call_print(const PyObject *, FILE *, const char *_signature);
  };

  struct call_repr: exceptionHandler
  {
    const T &self;
    const char * const signature;
    const TypeBase<T> &type;
    PyObject *retval;
    virtual void call();
    call_repr(const PyObject *, const char *_signature);
  };

  struct call_str: exceptionHandler
  {
    const T &self;
    const char * const signature;
    const TypeBase<T> &type;
    PyObject *retval;
    virtual void call();
    call_str(const PyObject *, const char *_signature);
  };

  static void mediator_destructor(PyObject* self);
  static int mediator_print(PyObject *self, FILE *fp, int /*flags*/);
  static PyObject *mediator_repr(PyObject *self);
  static PyObject *mediator_str(PyObject *self);

  typedef std::vector<PyMethodDef> methods_t;
  typedef std::vector<PyGetSetDef> getseters_t;

  methods_t m_methods;
  getseters_t m_getseters;
};

}; //namespace pyhrol

#endif //__pyhrol_type_base_h__
