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

// $Date: 2014-04-04 16:35:38 +0400 (Fri, 04 Apr 2014) $
// $Revision: 906 $

#ifndef __pyhrol_tuples_h__
#define __pyhrol_tuples_h__

#include <stdint.h>
#include "pyhrol_common.h"

typedef struct _object PyObject;

namespace pyhrol
{

class Tuples;
struct exceptionHandler;

class TuplesData
{
public:
  virtual ~TuplesData();

  static TuplesData *factory(const callVariants cv, const size_t);
};


class TupleIn;
class TupleOut;

class Tuples
{
  public:
    enum callerTypes
    {
        ctUnspecified
      , ctGetter
      , ctSetter
    };

    virtual TupleIn &t_in() const = 0;
    virtual TupleOut &t_out() const = 0;

    /**
    @param required
      Количество обязательных аргументов
      -1 -- все обязательные, разделитель '|' не вставляется
      0 -- нет обязательных, форматная строка начинается с разделителя '|'
      N > 0 -- разделитель '|' вставляется после N-го аргумента (нумерация с 0)
      Актуально для режима optParse, в остальных игнорируется
    */
    virtual void set_options(const uint8_t /*_required*/) = 0;
    virtual void set_options(const options /*_opts*/) = 0;
    virtual void set_options(const options /*_opts*/, const uint8_t /*_required*/) = 0;

    /**
    @return
      true -- в режиме probe или если предыдущая попытка парсинга (TupleState::parse) не удалась
      false -- если перед этим TupleState::parse был удачным
    */
    virtual bool prepare_parse(const char *) = 0;
    /**
    @return
      true -- в режиме execute, если предыдущая попытка парсинга (TupleState::parse) не удалась
      false -- в режиме probe или если перед этим TupleState::parse был удачным
    */
    virtual bool is_parse_enabled() = 0;
      virtual void parse(const bool /*dummy*/, ...) = 0;

    /**
    @return
      true -- в режиме probe или после TupleState::choose_build_variant
      false -- на первой итерации в режиме execute
    */
    virtual bool before_build() = 0;
      /**
      @return
        true -- в режиме probe или если внутренний счетчик совпадает со значением, установленным TupleState::choose_build_variant
        false -- в режиме execute, если внутренний счетчик не совпадает со значением, установленным TupleState::choose_build_variant
      */
      virtual bool prepare_build(const char *) = 0;
      /**
      @return
        true -- в режиме execute, если внутренний счетчик совпадает со значением, установленным TupleState::choose_build_variant
        false -- в режиме probe, или если внутренний счетчик не совпадает со значением, установленным TupleState::choose_build_variant
      */
      virtual bool is_build_enabled() = 0;
        virtual void build(const bool dummy, ...) = 0;

    /**
    @return
      true -- в режиме execute, дальнейшие вызовы недопустимы
      false -- в режиме probe
    */
    virtual bool after_build() = 0;

    /**
    @return
    */
    virtual const uint8_t parsed_variant() const = 0;
    /**
    @param build_variant
      0 -- нет возвращаемого значения
      [0, N) -- одна из сигнатур, N -- количество вызовов prepare_build
    */
    virtual void choose_build_variant(const uint8_t = 0U) = 0;

    virtual PyObject *ubiquitous_caller
    (
        exceptionHandler &
      , PyObject *args
      , PyObject *kwds
      , const bool is_probe
      , const callerTypes = ctUnspecified
    ) = 0;

    virtual ~Tuples();

    static Tuples *factory(TuplesData &);
};

typedef Tuples Args;


} //namespace pyhrol

#endif //__pyhrol_tuples_h__
