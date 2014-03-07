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

#ifndef __pyhrol_index_h__
#define __pyhrol_index_h__

#include <sys/types.h>
#include <cstddef>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

#include "nullable.h"
#include "smart_flag.h"
#include "pyhrol_common.h"

namespace pyhrol
{

class Index
{
public:
  static const size_t num_width;

  enum elementTypes
  {
      etModule
    , etType
    , etCtor
    , etMethod
    , etGetsetter
    , etSpecialCall
    , etFunction
  };

  class NotFoundException: public KeyException
  {
    static const std::string m_describe(const char *, const elementTypes et);
  public:
    NotFoundException(const char *);
    NotFoundException(const char *, const elementTypes et);
  };

  class NonUniqueException: public KeyException
  {
    static const std::string m_describe(const char *, const size_t);
  public:
    NonUniqueException(const char *, const size_t);
  };

  class WrongTypeException: public TypeException
  {
    static const std::string m_describe(const char *, const elementTypes real);
    static const std::string m_describe(const char *, const elementTypes real, const elementTypes expected);
  public:
    WrongTypeException(const char *, const elementTypes real);
    WrongTypeException(const char *, const elementTypes real, const elementTypes expected);
  };

  void add(const elementTypes et, const std::string &name, const size_t offset);
  void show(std::ostream &) const;
  void show_names(std::ostream &) const;

  struct iterator
  {
    const elementTypes et;
    const Nullable<size_t> filter;
    Nullable<size_t> pos;

    iterator(const elementTypes);
    iterator(const elementTypes, const size_t filter);
  };
  bool iterate(iterator &) const;

  struct item
  {
    elementTypes et;
    std::string name;
    ssize_t
        next
      , prev
      , child
      , parent
    ;
    size_t
        offset
      , children
    ;
  };

  const item *get(const size_t) const;
  size_t find(const char *) const;
  size_t find(const char *, const size_t parent) const;
  size_t find(const char *, const elementTypes et) const;
  size_t find(const char *, const elementTypes et, const size_t parent) const;
  size_t find(const char *, const elementTypes *pet, const size_t et_len) const;
  size_t find(const char *, const elementTypes *pet, const size_t et_len, const size_t parent) const;

private:

  typedef std::vector<item> idx_t;
  typedef std::multimap<std::string, size_t> names_t;

  bool m_find(iterator &) const;
  bool m_next(Nullable<size_t> &pos, const bool filtered) const;
  static char m_element_letter(const elementTypes et);
  static const std::string m_element_typename(const elementTypes et);

  Nullable<size_t>
      m_module_notch
    , m_type_notch
    , m_method_notch
    , m_function_notch
  ;
  idx_t m_idx;
  names_t m_names;
};

}; //namespace pyhrol

typedef SmartFlag<pyhrol::Index::elementTypes> index_element_types_flag_t;

#endif //__pyhrol_index_h__
