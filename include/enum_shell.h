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

// $Date: 2013-04-14 11:48:09 +0400 (Sun, 14 Apr 2013) $
// $Revision: 701 $

#ifndef __enum_shell_h__
#define __enum_shell_h__


#include <stdint.h>
#include <map>
#include <vector>
#include <string>


#ifdef _DEBUG_UTILS
typedef std::vector<std::string> __flag_names_t;
typedef std::multimap<size_t, std::string> __res_list_t;
#endif //_DEBUG_UTILS

enum node_props
{
    npAnd = 0x1
  , npMask = 0x2
  , npFilter = 0x4
  , npOr = 0x8
  , npTrue = 0x80
  , npFalse = 0x00
};

/**
Преобразование "флагов" в строку и обратно. Флаги в строке разделяются пробелом.
Под "флагом" понимается совокупность целочисленных констант, объединенных в одно число.
В простейшем случае флаг состоит из взаимоисключающих значений, например флаги "SEEK_*" для функции lseek или взаимодополняющих непротиворечащих друг другу значений, как, например, флаги "S_I_*" в chmod, более общий случай -- комбинированный, как поля типа tcflag_t в структуре termios. В общем виде допустимость различных значений флага определяется деревом решений (_st_node[]).
Дерево решений заполняется методами add, каждый из которых добавляет от одного до трех узлов.
Методы add без параметра parent создают первый узел дерева -- вершину, остальные -- дочерние узлы.
Узлы дерева делятся на условия и значения. Условие может иметь два дочерних узла, соответствующих истинности и ложности, узлы значений дочерних узлов не имеют. Отсутствие дочернего узла в условии означает недопустимость значения, при преобразовании такой узел всегда будет вызывать ошибку.
Тип E -- целочисленный или enum, должен преобразовываться к size_t, поддерживать операторы &, ~
*/
template <typename E> struct enum_2_string_stree
{
/* TODO
  -- есть ошибки при обратном преобразовании
  -- проверка непротиворечивости флага, то же, что и преобразование в строку, только без строкового буфера
  -- генерация списка всех возможных значений
  -- значения в графе, превосходящие 32, показывать в формате 0x
  -- удалить m_stg
  -- синонимы -- разные строковые названия для одной целочисленной константы
  -- префиксы и суффиксы (обязательные/опциональные, [не]дополняющие вывод)
  -- регистрозависимость -- включить/отключить

Переходы по ветке "истинности":
  v & M -- безусловный (npMask)
  (v & M) != 0 -- хотя бы один бит из маски установлен (npFilter)
  v & M == M -- все биты из маски установлены (npAnd)
  v == M -- все биты из маски установлены, остальные -- нет (npOr)
Переходы по "ложной" ветке:
  Условные только для узлов npOr
  Безусловные с обнулением битов по маске
  TODO Безусловные с неизменным содержанием (для npCheckAnd и npCheckAll)
*/
  typedef E value_type;
  /**
  ? <- cond -> ?
  */
  bool add(const node_props /*props*/, const E /*cond*/);
  /**
  _val_true <- cond -> ?
  */
  bool add(const node_props /*props*/, const E /*cond*/, const std::string &/*_val_true*/);
  /**
  _val_true <- cond -> _val_false
  */
  bool add(const node_props /*props*/, const E /*cond*/, const std::string &/*_val_true*/, const std::string &/*_val_false*/);
  /**
  if props & npTrue:
    (? <- cond -> ?) <- parent
  else:
    parent -> (? <- cond -> ?)
  */
  bool add(const node_props /*props*/, const E /*cond*/, const E /*parent*/);
  /**
  if props & npTrue:
    (_val_true <- cond -> ?) <- parent
  else:
    parent -> (_val_true <- cond -> ?)
  */
  bool add(const node_props /*props*/, const E /*cond*/, const std::string &/*_val_true*/, const E /*parent*/);
  /**
  if props & npTrue:
    (_val_true <- cond -> _val_false) <- parent
  else:
    parent -> (_val_true <- cond -> _val_false)
  */
  bool add(const node_props /*props*/, const E /*cond*/, const std::string &/*_val_true*/, const std::string &/*_val_false*/, const E /*parent*/);
  /**
  В процессе заполнения дерева решений используется индекс, необходимость в котором отпадает по окончании процесса
  */
  void free_index();
#ifdef _DEBUG_UTILS_ENSH
  static bool __g_quiet;
#endif //_DEBUG_UTILS_ENSH
#ifdef _DEBUG_UTILS
  void __clear();
  void __report(std::ostream &/*os*/) const;
  bool __test_combinations
  (
      __res_list_t &//correct_strings
    , __res_list_t *//superfluous_numbers
    , __res_list_t *//missing_numbers
    , __res_list_t *//perr_messages
    , const __flag_names_t */*pnames*/ = NULL
  ) const;
#endif //_DEBUG_UTILS

  bool convert(std::string &/*res*/, const E /*src*/) const;
  bool convert(std::ostream &/*os*/, const E /*src*/) const;
  bool valid(const E /*src*/) const;
  void check(const E /*src*/) const;
  E convert(const std::string &/*src*/) const;
  void graph(std::ostream &/*os*/) const;
  const std::string statement() const;
  void statement(std::ostream &os) const;
  const uint32_t depth_max() const;
  bool filled() const;

  enum_2_string_stree();
  ~enum_2_string_stree();

  private:
    enum_2_string_stree(const enum_2_string_stree &);
    enum_2_string_stree &operator =(const enum_2_string_stree &);

    enum node_types
    {
        ntiAnd = 0x01
      , ntiMask = 0x02
      , ntiFilter = 0x04
      , ntiOr = 0x08
      , ntiValue = 0x100
      , ntiValueContradicted = 0x110
      , ntiValueAmbiguous = 0x120
    };
    enum string_types
    {
        strtIgnored = 0
      , strtContradicted = 0x110
      , strtAmbiguous = 0x120
      , strtRequired = 1
      , strtInvalid = 2
    };
    struct _st_node //decision tree node
    {
      const uint32_t depth;
      const _st_node
          *ptrue
        , *pfalse
      ;
      const _st_node* const pparent;
      const E value;
      const node_types type;
      const std::string name;

      _st_node(const E /*_value*/, const node_types /*_type*/, const std::string &/*_name*/, const _st_node */*_pparent*/, const uint32_t /*_depth*/ = 0U); //ntiValue
      _st_node(const E /*_value*/, const node_types /*_type*/, const _st_node */*_pparent*/, const uint32_t /*_depth*/ = 0U);
    };
    typedef std::map<E, _st_node *> _idx_t;
    typedef std::vector<const _st_node *> _stg_t;
    typedef std::map<std::string, string_types> _ids_t;
    _idx_t m_idx;
    _stg_t m_stg;
    uint32_t m_depth_max;

    bool m_convert(std::string */*pres*/, const E /*value*/) const;
    E m_convert_reverse(_ids_t &/*ids*/) const;
    void m_build_error_description(std::ostream &/*os*/, _ids_t &/*ids*/) const;
    static const E m_calculate_value(node_types &/*_type*/, const bool /*is_true*/, const _st_node */*_pparent*/);
    static void m_graph_node (std::ostream &/*os*/, const _st_node */*pnode*/, const size_t /*node_id*/, const bool /*label*/ = true, const bool /*indent*/ = true);

  #ifdef _DEBUG_UTILS
    static void __m_report_node (std::ostream &/*os*/, const _st_node */*pnode*/, const size_t level = 0);
  #endif //_DEBUG_UTILS
};

#endif //__enum_shell_h__

