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

typedef std::map<int, int> dummy_t;

class DummyType: public pyhrol::TypeWrapper<dummy_t>, public pyhrol::TypeMap<dummy_t>, pyhrol::TypeSequence<dummy_t>, pyhrol::TypeNumber<dummy_t>, pyhrol::TypeIterable<dummy_t, dummy_t>
{
  DummyType()
    : pyhrol::TypeBase<dummy_t>("MyClass", NULL)
  {
    //Assigned by TypeWrapper
    m_type_object.tp_dealloc = NULL;
    m_type_object.tp_print = NULL;
    m_type_object.tp_repr = NULL;
    m_type_object.tp_str = NULL;
    m_type_object.tp_new = NULL;

    //Assigned by TypeMap
    m_type_object.tp_as_mapping->mp_length = NULL;
    m_type_object.tp_as_mapping->mp_subscript = NULL;
    m_type_object.tp_as_mapping->mp_ass_subscript = NULL;

    //Assigned by TypeSequence
    m_type_object.tp_as_sequence->sq_length = NULL;
    m_type_object.tp_as_sequence->sq_concat = NULL;
    m_type_object.tp_as_sequence->sq_repeat = NULL;
    m_type_object.tp_as_sequence->sq_item = NULL;
    m_type_object.tp_as_sequence->sq_slice = NULL;
    m_type_object.tp_as_sequence->sq_ass_item = NULL;
    m_type_object.tp_as_sequence->sq_ass_slice = NULL;
    m_type_object.tp_as_sequence->sq_contains = NULL;
    m_type_object.tp_as_sequence->sq_inplace_concat = NULL;
    m_type_object.tp_as_sequence->sq_inplace_repeat = NULL;

    //Assigned by TypeNumberCommon
    m_type_object.tp_as_number->nb_negative = NULL;
    m_type_object.tp_as_number->nb_positive = NULL;
    m_type_object.tp_as_number->nb_absolute = NULL;
    m_type_object.tp_as_number->nb_nonzero = NULL;
    m_type_object.tp_as_number->nb_invert = NULL;
    m_type_object.tp_as_number->nb_coerce = NULL;
    m_type_object.tp_as_number->nb_int = NULL;
    m_type_object.tp_as_number->nb_long = NULL;
    m_type_object.tp_as_number->nb_float = NULL;
    m_type_object.tp_as_number->nb_oct = NULL;
    m_type_object.tp_as_number->nb_hex = NULL;
    m_type_object.tp_as_number->nb_index = NULL;

    //Assigned by TypeNumber
    m_type_object.tp_as_number->nb_add = NULL;
    m_type_object.tp_as_number->nb_subtract = NULL;
    m_type_object.tp_as_number->nb_multiply = NULL;
    m_type_object.tp_as_number->nb_divide = NULL;
    m_type_object.tp_as_number->nb_remainder = NULL;
    m_type_object.tp_as_number->nb_divmod = NULL;
    m_type_object.tp_as_number->nb_power = NULL;
    m_type_object.tp_as_number->nb_lshift = NULL;
    m_type_object.tp_as_number->nb_rshift = NULL;
    m_type_object.tp_as_number->nb_and = NULL;
    m_type_object.tp_as_number->nb_xor = NULL;
    m_type_object.tp_as_number->nb_or = NULL;
    m_type_object.tp_as_number->nb_inplace_add = NULL;
    m_type_object.tp_as_number->nb_inplace_subtract = NULL;
    m_type_object.tp_as_number->nb_inplace_multiply = NULL;
    m_type_object.tp_as_number->nb_inplace_divide = NULL;
    m_type_object.tp_as_number->nb_inplace_remainder = NULL;
    m_type_object.tp_as_number->nb_inplace_power = NULL;
    m_type_object.tp_as_number->nb_inplace_lshift = NULL;
    m_type_object.tp_as_number->nb_inplace_rshift = NULL;
    m_type_object.tp_as_number->nb_inplace_and = NULL;
    m_type_object.tp_as_number->nb_inplace_xor = NULL;
    m_type_object.tp_as_number->nb_inplace_or = NULL;
    m_type_object.tp_as_number->nb_floor_divide = NULL;
    m_type_object.tp_as_number->nb_true_divide = NULL;
    m_type_object.tp_as_number->nb_inplace_floor_divide = NULL;
    m_type_object.tp_as_number->nb_inplace_true_divide = NULL;

    //Assigned by TypeIterable
    m_type_object.tp_iter = NULL;
    m_type_object.tp_iternext = NULL;
  }

  virtual void destructor(dummy_t &) const
  {
  }

  static void init() __attribute__ ((constructor))
  {
    m_get(new DummyType);
  }
};
