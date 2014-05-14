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

#include <pyhrol.h>
#include <pyhrol_auto_holder.h>

using namespace std;
using namespace pyhrol;

struct iterator
{
  string::const_iterator iter;
  const string::const_iterator iter_end;

  iterator(const Ptr<const string> &container)
    : iter(container->begin())
    , iter_end(container->end())
    , m_container(container)
  {
    Py_IncRef(m_container);
  }

  ~iterator()
  {
    Py_DecRef(m_container);
  }

private:
  PyObject *m_container;
};

struct item: AutoHolder
{
  const char data;
  item(const Ptr< ::iterator> &iter)
    : AutoHolder(iter)
    , data(*iter->iter ++)
  {
  }
};


class Container: public TypeWrapper<string>, public TypeIterable<string, ::iterator>
{
  Container()
    : TypeBase<string>("MyClass", NULL)
  {
    m_type_object.tp_iternext = NULL;
  }

  virtual void constructor(string &res, Tuples &_args) const
  {
    const char *data;
    PYHROL_PARSE_TUPLE_1(NULL, _args, data)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    new(&res) string(data);

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void destructor(string &obj) const
  {
    obj.~string();
  }

  virtual void print(ostream &os, const string &obj) const
  {
    os << obj;
  }

  virtual void iter(const Ptr< ::iterator> &iter, const Ptr<const string> &container) const
  {
    new(&*iter) ::iterator(container);
  }

  static void init() __attribute__ ((constructor))
  {
    m_get(new Container);
  }
};

class Iterator: public TypeWrapper< ::iterator>, public TypeIterable< ::iterator, item>
{
  Iterator()
    : TypeBase< ::iterator>("iter", NULL)
  {
    m_type_object.tp_iter = NULL;
  }

  virtual void constructor(::iterator &res, Tuples &_args) const
  {
    const char *data;
    PYHROL_PARSE_TUPLE_1(NULL, _args, data)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    ::Container::T_struct *obj = ::Container::allocate_static();
    new(&obj->endosome) string(data);
    new(&res) ::iterator(Ptr<const string>(&obj->endosome, *obj));

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void destructor(::iterator &obj) const
  {
    obj.~iterator();
  }

  virtual bool next(const Ptr<item> &itm, const Ptr< ::iterator> &iter) const
  {
    bool retval = false;
    if (iter->iter != iter->iter_end)
    {
      new (&*itm) item(iter);
      retval = true;
    }
    return retval;
  }

  static void init() __attribute__ ((constructor))
  {
    m_get(new Iterator);
  }
};

class Data: public TypeWrapper<item>
{
  Data()
    : TypeBase< ::item>("item", NULL)
  {
    m_add_member<const char, &item::data>("data", NULL);
  }

  virtual void destructor(item &itm) const
  {
    itm.~item();
  }

  static void init() __attribute__ ((constructor))
  {
    m_get(new Data);
  }
};

