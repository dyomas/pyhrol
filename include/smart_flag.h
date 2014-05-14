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

// $Date: 2014-02-23 20:22:57 +0400 (Sun, 23 Feb 2014) $
// $Revision: 895 $

#ifndef __smart_flag_h__
#define __smart_flag_h__

//Примитивная надстройка над enum_2_string_stree

#include <ostream>


template <class T> struct enum_2_string_stree;

template <class T> class SmartFlag
{
  public:
    typedef enum_2_string_stree<T> ttree_t;

    void convert(std::ostream &os) const;
    const std::string to_string() const;
    const ttree_t *operator ->() const;
    operator const T () const;
    bool assigned() const;
    const T get() const;

    SmartFlag &operator = (const std::string &/*s*/);
    SmartFlag &operator = (const T /*value*/);
    void clear();

    SmartFlag();
    explicit SmartFlag(const char *);
    SmartFlag(const T _value);

  private:
    bool m_assigned;
    T
        m_value
      , m_value_default
    ;
    static const ttree_t &m_get_tree();
};

//template <class T> ostream &operator << (ostream &/*os*/, const T &/*t*/);
template <class T> std::ostream &operator << (std::ostream &/*os*/, const SmartFlag<T> &/*sf*/);

#endif //__smart_flag_h__

