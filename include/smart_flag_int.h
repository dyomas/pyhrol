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

// $Date: 2013-11-29 03:26:15 +0400 (Fri, 29 Nov 2013) $
// $Revision: 847 $

#ifndef __smart_flag_int_h__
#define __smart_flag_int_h__

/*
NOTE
Классы SmartFlag<T> и SmartFlagInt спроектированы для использования статическиго объекта enum_2_string_stree<T>.
Первый инстанцирует и заполняет enum_2_string_stree для каждого типа T, последний возлагает это на своих потомков. У каждого из них своя статическая копия enum_2_string_stree
Кажущаяся очевидной необходимость объединения данных классов невозможна
*/

#include "enum_shell.hpp"

/**
Потомок класса заполняет статический экземпляр класса ftree_t и передает его адрес через конструктор SmartFlagInt
SmartFlagInt::m_pftree используется для преобразования строки в флаг (например, при инициализации через командную строку),
либо для преобразовании флага в строку (например, при выводе в stdout)
Если значение флага, заданное конструктором, изменяется, метод SmartFlagInt::assigned возвращает true
*/
class SmartFlagInt
{
  public:
    typedef enum_2_string_stree<int> ftree_t;
    SmartFlagInt(const ftree_t *);
    SmartFlagInt(const ftree_t *, const int /*value*/);
    SmartFlagInt(const SmartFlagInt &);

    const std::string as_string() const;
    void convert(std::ostream &/*os*/) const;
    const ftree_t *operator ->() const;
    bool assigned() const;
    const int get() const;
    operator const int () const;

    void assign (const int &);
    void assign (const std::string &);
    SmartFlagInt &operator = (const int &);
    SmartFlagInt &operator = (const std::string &);
    void clear();

  private:
    bool m_assigned;
    int m_value;
    const ftree_t *m_pftree;
};

std::ostream &operator << (std::ostream &os, const SmartFlagInt &f);

#endif //__smart_flag_int_h__

