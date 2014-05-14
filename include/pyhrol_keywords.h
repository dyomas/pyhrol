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

// $Date: 2013-08-30 21:20:58 +0400 (Fri, 30 Aug 2013) $
// $Revision: 762 $

#ifndef __pyhrol_keywords_h__
#define __pyhrol_keywords_h__


#include <stddef.h>
#include <stdio.h>
#include <map>
#include <set>
#include <vector>
#include <stdexcept>

#ifdef _DEBUG_PYTHON_KEYWORDS
#include <iostream>
#endif //_DEBUG_PYTHON_KEYWORDS

class ArraySquare;

namespace pyhrol
{

class Keywords
{
  struct part
  {
    std::string value;
    bool nums_start, nums_end;
  };

  typedef std::vector<part> parts_t;
  typedef std::set<std::pair<size_t, size_t> > variants_t;

  struct keyword
  {
    variants_t variants;
    parts_t parts;
  };
  struct idx_item
  {
    size_t offset, begin, end;
    bool is_whole;
  };
  typedef std::multimap<std::string, idx_item> parts_idx_t;
  typedef std::vector<keyword> keywords_t;

  keywords_t m_keywords;

  void m_add_keyword(const char *begin, const char *end);
  void m_assign_variants(const parts_idx_t &);
  void m_build_idx(parts_idx_t &) const;
  const std::string m_build_string(const parts_t &parts, const size_t begin, const size_t end) const;

public:
#ifdef _DEBUG_PYTHON_KEYWORDS
  void __report(std::ostream &os) const;
#endif //_DEBUG_PYTHON_KEYWORDS

  class ChoiceError: public std::logic_error
  {
  public:
    typedef std::vector<size_t> failure_idx_t;
    ChoiceError(const failure_idx_t &non_unique, const failure_idx_t &illegal);
  private:
    static const std::string m_describe(const failure_idx_t &, const failure_idx_t &);
  };

  void build(ArraySquare &);
  void add(const std::string &name);
  void add(const char *begin, const char *end);

  Keywords();
};

} //namespace pyhrol

#endif //__pyhrol_keywords_h__
