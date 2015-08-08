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

struct myClass
{
  typedef std::map<std::string, std::string> strings_t;
  strings_t strings;
};

class PyType: public pyhrol::TypeMap<myClass>, public pyhrol::TypeWrapper<myClass>
{
  PyType()
    : pyhrol::TypeBase<myClass>("MyClass", "help")
  {
  }

  virtual void constructor(myClass &obj, pyhrol::Tuples &_args) const
  {
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    new (&obj) myClass;

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void destructor(myClass &obj) const
  {
    obj.~myClass();
  }

  virtual void print(std::ostream &os, const myClass &obj) const
  {
    bool delimit = false;
    os << '{';
    for (myClass::strings_t::const_iterator iter = obj.strings.begin(), iter_end = obj.strings.end(); iter != iter_end; iter ++)
    {
      if (delimit)
      {
        os << ", ";
      }
      os << iter->first << " = " << iter->second;
      delimit = true;
    }
    os << '}';
  }
  virtual Py_ssize_t length(const pyhrol::Ptr<const myClass> &obj) const
  {
    return obj->strings.size();
  }

  virtual void get(const pyhrol::Ptr<const myClass> &obj, pyhrol::Tuples &_args) const
  {
    const char *arg;
    long long num;
    std::string res;

    PYHROL_PARSE_TUPLE_1(NULL, _args, arg)
    PYHROL_PARSE_TUPLE_1(NULL, _args, num)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_BUILD_VALUE_1(NULL, _args, res.c_str())
    PYHROL_AFTER_BUILD_VALUE(_args)

    std::ostringstream ostr;
    switch (_args.parsed_variant())
    {
      case 0:
        ostr << arg;
        break;
      case 1:
        ostr << num;
        break;
    }
    res = obj->strings.at(ostr.str());

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void del(const pyhrol::Ptr<myClass> &obj, pyhrol::Tuples &_args) const
  {
    const char *arg;
    long long num;

    PYHROL_PARSE_TUPLE_1(NULL, _args, arg)
    PYHROL_PARSE_TUPLE_1(NULL, _args, num)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    std::ostringstream ostr;
    switch (_args.parsed_variant())
    {
      case 0:
        ostr << arg;
        break;
      case 1:
        ostr << num;
        break;
    }

    if (!obj->strings.erase(ostr.str()))
    {
      throw std::out_of_range(std::string("Item \"") + ostr.str() + "\" not found");
    }

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void assign(const pyhrol::Ptr<myClass> &obj, pyhrol::Tuples &_args) const
  {
    const char *key_str, *value_str;
    long long key_num, value_num;

    PYHROL_PARSE_TUPLE_2(NULL, _args, key_str, value_str)
    PYHROL_PARSE_TUPLE_2(NULL, _args, key_str, value_num)
    PYHROL_PARSE_TUPLE_2(NULL, _args, key_num, value_str)
    PYHROL_PARSE_TUPLE_2(NULL, _args, key_num, value_num)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    std::ostringstream key, value;
    switch (_args.parsed_variant())
    {
      case 0:
        key << key_str;
        value << value_str;
        break;
      case 1:
        key << key_str;
        value << value_num;
        break;
      case 2:
        key << key_num;
        value << value_str;
        break;
      case 3:
        key << key_num;
        value << value_num;
        break;
    }

    if (!obj->strings.insert(myClass::strings_t::value_type(key.str(), value.str())).second)
    {
      throw std::out_of_range(std::string("Item \"") + key.str() + "\" already exists");
    }

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  static void init() __attribute__ ((constructor))
  {
    m_get(new PyType);
  }
};
