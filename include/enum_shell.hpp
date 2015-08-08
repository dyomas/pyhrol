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

// $Date: 2014-04-22 18:14:41 +0400 (Вт., 22 апр. 2014) $
// $Revision: 913 $

#ifndef __enum_shell_hpp__
#define __enum_shell_hpp__

#include <typeinfo>
#include <set>
#ifdef _DEBUG_UTILS
#include <iomanip>
#endif //_DEBUG_UTILS
#include <sstream>
#include <stdexcept>
#include "binary_operations.h"
#include "demangling.h"
#include "strings_manip.h"
#include "enum_shell.h"

template <typename E> bool enum_2_string_stree<E>::add(const node_props props, const E cond)
{
  const bool ret_val = !m_stg.size();
  _st_node *pnode = new _st_node(cond, node_types(props & ~ npTrue), NULL);
  m_idx[cond] = pnode;
  m_stg.push_back(pnode);
  return ret_val;
}

template <typename E> bool enum_2_string_stree<E>::add(const node_props props, const E cond, const std::string &_val_true)
{
  const bool ret_val = !m_stg.size();
  node_types _type;
  _st_node *pnode = new _st_node(cond, node_types(props & ~ npTrue), NULL);
  const E _value = m_calculate_value(_type, true, pnode);
  pnode->ptrue = new _st_node(_value, _type, _val_true, pnode, 1U);

  m_idx[cond] = pnode;
  m_stg.push_back(pnode);
  m_stg.push_back(pnode->ptrue);

  if (!m_depth_max)
  {
    m_depth_max = 1U;
  }
  return ret_val;
}

template <typename E> bool enum_2_string_stree<E>::add(const node_props props, const E cond, const std::string &_val_true, const std::string &_val_false)
{
  const bool ret_val = !m_stg.size();
  node_types _type;
  _st_node *pnode = new _st_node(cond, node_types(props & ~ npTrue), NULL);

  E _value = m_calculate_value(_type, true, pnode);
  pnode->ptrue = new _st_node(_value, _type, _val_true, pnode, 1U);
  _value = m_calculate_value(_type, false, pnode);
  pnode->pfalse = new _st_node(_value, _type, _val_false, pnode);

  m_stg.push_back(pnode);
  m_stg.push_back(pnode->ptrue);
  m_stg.push_back(pnode->pfalse);

  if (!m_depth_max)
  {
    m_depth_max = 1U;
  }
  return ret_val;
}

template <typename E> bool enum_2_string_stree<E>::add(const node_props props, const E cond, const E parent)
{
  bool ret_val = true;
  typename _idx_t::iterator iter = m_idx.find(parent);
  uint32_t depth = 0;

  if (iter != m_idx.end())
  {
    depth = iter->second->depth + (props & npTrue ? 1 : 0);
  }
  else
  {
    ret_val = false;
  }
  if (m_depth_max < depth)
  {
    m_depth_max = depth;
  }
  _st_node *pnode = new _st_node(cond, node_types(props & ~ npTrue), ret_val ? iter->second : NULL, depth);
  if (iter != m_idx.end())
  {
    if (props & npTrue)
    {
      if (iter->second->ptrue)
      {
        ret_val = false;
      }
      iter->second->ptrue = pnode;
    }
    else
    {
      if (iter->second->pfalse)
      {
        ret_val = false;
      }
      iter->second->pfalse = pnode;
    }
    if (iter->second->ptrue && iter->second->pfalse)
    {
      m_idx.erase(iter);
    }
  }

  m_idx[cond] = pnode;
  m_stg.push_back(pnode);
  return ret_val;
}

template <typename E> bool enum_2_string_stree<E>::add(const node_props props, const E cond, const std::string &_val_true, const E parent)
{
  bool ret_val = true;
  typename _idx_t::iterator iter = m_idx.find(parent);
  uint32_t depth = 0;

  if (iter != m_idx.end())
  {
    depth = iter->second->depth + (props & npTrue ? 1 : 0);
  }
  else
  {
    ret_val = false;
  }
  if (m_depth_max < depth + 1)
  {
    m_depth_max = depth + 1;
  }
  _st_node *pnode = new _st_node(cond, node_types(props & ~ npTrue), ret_val ? iter->second : NULL, depth);
  if (iter != m_idx.end())
  {
    if (props & npTrue)
    {
      if (iter->second->ptrue)
      {
        ret_val = false;
      }
      iter->second->ptrue = pnode;
    }
    else
    {
      if (iter->second->pfalse)
      {
        ret_val = false;
      }
      iter->second->pfalse = pnode;
    }
    if (iter->second->ptrue && iter->second->pfalse)
    {
      m_idx.erase(iter);
    }
  }

  node_types _type;
  const E _value = m_calculate_value(_type, true, pnode);
  pnode->ptrue = new _st_node(_value, _type, _val_true, pnode, depth + 1);

  m_idx[cond] = pnode;
  m_stg.push_back(pnode);
  m_stg.push_back(pnode->ptrue);
  return ret_val;
}

template <typename E> bool enum_2_string_stree<E>::add(const node_props props, const E cond, const std::string &_val_true, const std::string &_val_false, const E parent)
{
  bool ret_val = true;
  typename _idx_t::iterator iter = m_idx.find(parent);
  uint32_t depth = 0;

  if (iter != m_idx.end())
  {
    depth = iter->second->depth + (props & npTrue ? 1 : 0);
  }
  else
  {
    ret_val = false;
  }
  if (m_depth_max < depth + 1)
  {
    m_depth_max = depth + 1;
  }
  _st_node *pnode = new _st_node(cond, node_types(props & ~ npTrue), ret_val ? iter->second : NULL, depth);
  if (iter != m_idx.end())
  {
    if (props & npTrue)
    {
      if (iter->second->ptrue)
      {
        ret_val = false;
      }
      iter->second->ptrue = pnode;
    }
    else
    {
      if (iter->second->pfalse)
      {
        ret_val = false;
      }
      iter->second->pfalse = pnode;
    }
    if (iter->second->ptrue && iter->second->pfalse)
    {
      m_idx.erase(iter);
    }
  }

  node_types _type;
  E _value = m_calculate_value(_type, true, pnode);
  pnode->ptrue = new _st_node(_value, _type, _val_true, pnode, pnode->depth + 1);
  _value = m_calculate_value(_type, false, pnode);
  pnode->pfalse = new _st_node(_value, _type, _val_false, pnode, pnode->depth);

  m_idx[cond] = pnode;
  m_stg.push_back(pnode);
  m_stg.push_back(pnode->ptrue);
  m_stg.push_back(pnode->pfalse);
  return ret_val;
}

template <typename E> void enum_2_string_stree<E>::free_index()
{
  m_idx.clear();
}

#ifdef _DEBUG_UTILS_ENSH
template <typename E> bool enum_2_string_stree<E>::__g_quiet = false;
#endif //_DEBUG_UTILS_ENSH

#ifdef _DEBUG_UTILS
template <typename E> void enum_2_string_stree<E>::__clear()
{
  for (typename _stg_t::const_iterator iter = m_stg.begin(); iter != m_stg.end(); iter ++)
  {
    delete *iter;
  }
  m_idx.clear();
  m_stg.clear();
  m_depth_max = 0;
}

template <typename E> void enum_2_string_stree<E>::__report(std::ostream &os) const
{
  uint32_t
      level = 0
    , cnt = 0
  ;
  const _st_node *pnode_cur;
  _stg_t pnodes(m_depth_max + 1);

  if (!m_stg.size())
  {
    os
      << "<empty>" << std::endl
    ;
    return;
  }

  pnodes[0] = m_stg[0];

  while ((pnode_cur = pnodes[level]))
  {
    cnt ++;
    __m_report_node(os, pnode_cur, level);

    pnodes[level] = pnode_cur->pfalse;

    if ((pnode_cur = pnode_cur->ptrue))
    {
      if (pnode_cur->type & ntiValue)
      {
        cnt ++;
        __m_report_node(os, pnode_cur, level + (pnode_cur->pparent->ptrue == pnode_cur ? 1 : 0));
      }
      else
      {
        pnodes[++ level] = pnode_cur;
      }
    }

    while (!pnodes[level])
    {
      if (!level)
      {
        break;
      }
      level --;
    }
  }
  if (cnt != m_stg.size())
  {
    os
      << "*** Shown " << cnt << " items from " << m_stg.size() << " total" << std::endl
    ;
  }

  if (m_idx.size())
  {
    os
      << "Index residue:" << std::endl
    ;
    for (typename _idx_t::const_iterator iter = m_idx.begin(); iter != m_idx.end(); iter ++)
    {
      os << static_cast<size_t>(iter->first);
      __m_report_node(os, iter->second, 1);
    }
  }
}

template <typename E> bool enum_2_string_stree<E>::__test_combinations
(
    __res_list_t &correct_strings
  , __res_list_t *psuperfluous_numbers
  , __res_list_t *pmissing_numbers
  , __res_list_t *perr_messages
  , const __flag_names_t *pnames
) const
{
  bool ret_val = true;
  __flag_names_t names;

  if (pnames)
  {
    names = *pnames;
  }
  else
  {
    for (typename _stg_t::const_iterator iter = m_stg.begin(); iter != m_stg.end(); iter ++)
    {
      if ((*iter)->type & ntiValue)
      {
        names.push_back((*iter)->name);
      }
    }
  }

  const size_t cnt_variants = 1 << names.size();
  size_t res_max = 0;

  for (size_t var = 1; var < cnt_variants; var ++)
  {
    size_t var_cur = var;
    bool delimiter_needed = false;
    uint8_t bit_pos = 0;
    _ids_t ids;
    std::string name;
    do
    {
      if (var_cur & 1)
      {
        if (delimiter_needed)
        {
          name += ' ';
        }
        name += names[bit_pos];
        ids.insert(typename _ids_t::value_type(names[bit_pos], strtIgnored));
        delimiter_needed = true;
      }
      var_cur >>= 1;
      bit_pos ++;
    }
    while (var_cur);

    size_t res = m_convert_reverse(ids);

    if (ids.size())
    {
      if (perr_messages)
      {
        std::ostringstream err_str;
        m_build_error_description(err_str, ids);
        perr_messages->insert(__res_list_t::value_type(var, err_str.str()));
      }
    }
    else
    {
      if (res > res_max)
      {
        res_max = res;
      }
      correct_strings.insert(__res_list_t::value_type(res, name));
    }
  }

  {
    __res_list_t numbers;
    for (size_t var = 0; var < res_max * 2; var ++)
    {
      std::string name;
      if (m_convert(&name, E(var)))
      {
        numbers.insert(__res_list_t::value_type(var, name));
      }
    }

    bool
        remainder_left
      , remainder_right
    ;
    __res_list_t::const_iterator
        iter_left = correct_strings.begin()
      , iter_right = numbers.begin()
    ;

    while (true)
    {
      remainder_left = (iter_left != correct_strings.end());
      remainder_right = (iter_right != numbers.end());
      if (!remainder_left && !remainder_right)
      {
        break;
      }

      if (remainder_left && remainder_right && iter_left->first == iter_right->first)
      {
        if (iter_left->second != iter_right->second)
        {
          if (psuperfluous_numbers)
          {
            psuperfluous_numbers->insert(*iter_right);
          }
          if (pmissing_numbers)
          {
            pmissing_numbers->insert(*iter_left);
          }
          ret_val = false;
        }
        iter_left ++;
        iter_right ++;
      }
      else if (!remainder_left || remainder_right && (iter_left->first > iter_right->first))
      {
        if (psuperfluous_numbers)
        {
          psuperfluous_numbers->insert(*iter_right);
        }
        ret_val = false;
        iter_right ++;
      }
      else if (!remainder_right || remainder_left && (iter_left->first < iter_right->first))
      {
        if (pmissing_numbers)
        {
          pmissing_numbers->insert(*iter_left);
        }
        ret_val = false;
        iter_left ++;
      }
    }
  }
  return ret_val;
}
#endif //_DEBUG_UTILS

template <typename E> bool enum_2_string_stree<E>::convert(std::string &res, const E src) const
{
  bool ret_val = false;

  if (m_stg.size())
  {
    ret_val = m_convert(&res, src);
  }
  return ret_val;
}

template <typename E> bool enum_2_string_stree<E>::convert(std::ostream &os, const E src) const
{
  bool ret_val = false;
  std::string res;

  if (m_stg.size())
  {
    ret_val = m_convert(&res, src);
    os << res;
  }
  return ret_val;
}

template <typename E> bool enum_2_string_stree<E>::valid(const E src) const
{
  bool ret_val = false;

  if (m_stg.size())
  {
    ret_val = m_convert(NULL, src);
  }
  return ret_val;
}

template <typename E> void enum_2_string_stree<E>::check(const E src) const
{
  if (!m_stg.size())
  {
    throw std::runtime_error("Solution tree not inited");
  }
  else if (!m_convert(NULL, src))
  {
    std::ostringstream ostr;
    ostr << "Flags combination " << size_t(src);
    if (size_t(src) > 9)
    {
      ostr << " (" << std::hex << size_t(src) << ")" << std::dec;
    }
    ostr << " invalid in type " << typeid(E);
    throw std::runtime_error(ostr.str());
  }
}


template <typename E> E enum_2_string_stree<E>::convert(const std::string &src) const
{
  E ret_val = E(0);
  const char
      *begin = NULL
    , *end = NULL
  ;
  _ids_t ids;

  while (parse_string(begin, end, src.c_str()))
  {
    ids.insert(typename _ids_t::value_type(std::string(begin, end), strtIgnored));
  }

  if (m_stg.size())
  {
    ret_val = m_convert_reverse(ids);
  }

  if (ids.size())
  {
    std::ostringstream ostr;
    m_build_error_description(ostr, ids);
    throw std::runtime_error(ostr.str());
  }

  return ret_val;
}

template <typename E> void enum_2_string_stree<E>::graph(std::ostream &os) const
{
  typedef std::multimap<node_types, std::pair<const _st_node *, size_t> > _poses_by_type_t;
  typedef std::map<const _st_node *, size_t> _poses_by_node_t;
  bool has_error = false;
  uint32_t
      level = 0
    , cnt = 0
  ;

  const _st_node *pnode_cur;
  _stg_t pnodes(m_depth_max + 1);
  _poses_by_type_t _all;
  _poses_by_node_t _alln;

  if (!m_stg.size())
  {
    os
      << "digraph stree" << std::endl
      << "{" << std::endl
      << "#<No nodes>" << std::endl
      << "}" << std::endl
    ;
    return;
  }

  pnodes[0] = m_stg[0];

  /*
  Нерекурсивный обход всего дерева.
  Строятся два индекса:
    -- _all, индекс по типу, используется для группировки узлов графа с одинаковыми атрибутами; по мере построения графа группы однотипных узлов удаляются; узлы, не вошедшие ни в одну группу и оставшиеся после удаления являются некорректными и отображаются с особыми атрибутами
    -- _alln, индекс по указателю, используется для отображения граней графа
  Каждый из индексов однозначно связывает с узлом уникальный идентификатор, который является простым порядковым числом (cnt). Графы анализируются в сравнительных тестах, поэтому указатель на узел не может быть использован как идентификатор.
  */
  while ((pnode_cur = pnodes[level]))
  {
    _all.insert(typename _poses_by_type_t::value_type(pnode_cur->type, typename _poses_by_type_t::mapped_type(pnode_cur, cnt)));
    _alln.insert(typename _poses_by_node_t::value_type(pnode_cur, cnt));
    cnt ++;
    if (!(pnode_cur->type & ntiValue) && (!pnode_cur->ptrue || !pnode_cur->pfalse))
    {
      has_error = true;
    }

    pnodes[level] = pnode_cur->pfalse;

    if ((pnode_cur = pnode_cur->ptrue))
    {
      if (pnode_cur->type & ntiValue)
      {
        _all.insert(typename _poses_by_type_t::value_type(pnode_cur->type, typename _poses_by_type_t::mapped_type(pnode_cur, cnt)));
        _alln.insert(typename _poses_by_node_t::value_type(pnode_cur, cnt));
        cnt ++;
      }
      else
      {
        pnodes[++ level] = pnode_cur;
      }
    }

    while (!pnodes[level])
    {
      if (!level)
      {
        break;
      }
      level --;
    }
  }

  os
    << "digraph stree" << std::endl
    << "{" << std::endl
    << std::endl
  ;

  if (_all.count(ntiValue))
  {
    os
      << "#Valid data:" << std::endl
      << "  node [shape=oval,style=bold,color=gray,fontcolor=black];" << std::endl
    ;
    for (typename _poses_by_type_t::const_iterator iter = _all.lower_bound(ntiValue); iter != _all.upper_bound(ntiValue); iter ++)
    {
      m_graph_node(os, iter->second.first, iter->second.second);
    }
    _all.erase(ntiValue);
  }

  if (_all.count(ntiValueContradicted))
  {
    os
      << std::endl
      << "#Contradicted data:" << std::endl
      << "  node [shape=oval,style=filled,fillcolor=gray,color=red,fontcolor=white];" << std::endl
    ;
    for (typename _poses_by_type_t::const_iterator iter = _all.lower_bound(ntiValueContradicted); iter != _all.upper_bound(ntiValueContradicted); iter ++)
    {
      m_graph_node(os, iter->second.first, iter->second.second);
    }
    _all.erase(ntiValueContradicted);
  }

  if (_all.count(ntiValueAmbiguous))
  {
    os
      << std::endl
      << "#Ambiguous data:" << std::endl
      << "  node [shape=oval,style=filled,fillcolor=gray,color=red,fontcolor=red];" << std::endl
    ;
    for (typename _poses_by_type_t::const_iterator iter = _all.lower_bound(ntiValueAmbiguous); iter != _all.upper_bound(ntiValueAmbiguous); iter ++)
    {
      m_graph_node(os, iter->second.first, iter->second.second);
    }
    _all.erase(ntiValueAmbiguous);
  }

  if (_all.count(ntiAnd))
  {
    os
      << std::endl
      << "#Nodes \"and\":" << std::endl
      << "  node [shape=triangle,style=bold,color=green,fontcolor=black];" << std::endl
    ;
    for (typename _poses_by_type_t::const_iterator iter =  _all.lower_bound(ntiAnd); iter != _all.upper_bound(ntiAnd); iter ++)
    {
      m_graph_node(os, iter->second.first, iter->second.second);
    }
    _all.erase(ntiAnd);
  }

  if (_all.count(ntiOr))
  {
    os
      << std::endl
      << "#Nodes \"or\":" << std::endl
      << "  node [shape=diamond,style=bold,color=magenta,fontcolor=black];" << std::endl
    ;
    for (typename _poses_by_type_t::const_iterator iter = _all.lower_bound(ntiOr); iter != _all.upper_bound(ntiOr); iter ++)
    {
      m_graph_node(os, iter->second.first, iter->second.second);
    }
    _all.erase(ntiOr);
  }

  if (_all.count(ntiMask))
  {
    os
      << std::endl
      << "#Masks:" << std::endl
      << "  node [shape=house,style=bold,color=black,fontcolor=black];" << std::endl
    ;
    for (typename _poses_by_type_t::const_iterator iter = _all.lower_bound(ntiMask); iter != _all.upper_bound(ntiMask); iter ++)
    {
      m_graph_node(os, iter->second.first, iter->second.second);
    }
    _all.erase(ntiMask);
  }

  if (_all.count(ntiFilter))
  {
    os
      << std::endl
      << "#Filters:" << std::endl
      << "  node [shape=house,style=filled,fillcolor=grey,color=black,fontcolor=black];" << std::endl
    ;
    for (typename _poses_by_type_t::const_iterator iter = _all.lower_bound(ntiFilter); iter != _all.upper_bound(ntiFilter); iter ++)
    {
      m_graph_node(os, iter->second.first, iter->second.second);
    }
    _all.erase(ntiFilter);
  }

  if (_all.size())
  {
    os
      << std::endl
      << "#Invalid nodes:" << std::endl
      << "  node [shape=hexagon,style=bold,color=red,fontcolor=black];" << std::endl
    ;
    for (typename _poses_by_type_t::const_iterator iter = _all.begin(); iter != _all.end(); iter ++)
    {
      m_graph_node(os, iter->second.first, iter->second.second);
    }
  }

  if (has_error)
  {
    os
      << std::endl
      << "#Error:" << std::endl
      << "  node [shape=box,style=filled,fillcolor=red,color=black,fontcolor=black];" << std::endl
      << "  ERROR" << std::endl
    ;
  }

  /*
  Порядок следования ссылок не должен зависеть от адресов узлов _st_node;
  размещаясь в _links_t он определяется именами узлов графа;
  требуется исключительно для устойчивости результатов в тестах
  */
  typedef std::set<std::pair<std::string, std::string> > _links_t;
  _links_t _links;
  for (typename _poses_by_node_t::const_iterator iter = _alln.begin(); iter != _alln.end(); iter ++)
  {
    pnode_cur = iter->first;
    if (!(pnode_cur->type & ntiValue))
    {
      const std::string link_from = m_graph_node(pnode_cur, iter->second, false, false);
      std::string link_to;
      if (pnode_cur->ptrue)
      {
        link_to = m_graph_node(pnode_cur->ptrue, _alln.at(pnode_cur->ptrue), false, false);
      }
      else
      {
        link_to = "ERROR";
      }
      link_to += " [label=\"";
      switch (pnode_cur->type)
      {
        case ntiAnd:
          link_to += "&=";
          break;
        case ntiMask:
          link_to += "&";
          break;
        case ntiFilter:
          link_to += "!&";
          break;
        case ntiOr:
          link_to += '+';
          break;
        default:
          link_to += '?';
          break;
      }
      link_to += "\"]";
      _links.insert(_links_t::value_type(link_from, link_to));

      if (pnode_cur->pfalse)
      {
        link_to = m_graph_node(pnode_cur->pfalse, _alln.at(pnode_cur->pfalse), false, false);
      }
      else
      {
        link_to = "ERROR";
      }
      link_to += " [label=\"";
      switch (pnode_cur->type)
      {
        case ntiAnd:
        case ntiMask:
        case ntiFilter:
          link_to += "&~";
          break;
        case ntiOr:
          link_to += '-';
          break;
        default:
          link_to += '?';
          break;
      }
      link_to += "\"]";
      _links.insert(_links_t::value_type(link_from, link_to));
    }
  }

  if (_links.size())
  {
    os
      << std::endl
      << "#Links:" << std::endl
      << "  edge [color=black];" << std::endl
    ;
    for (typename _links_t::const_iterator iter = _links.begin(); iter != _links.end(); iter ++)
    {
      os
        << "  " << iter->first << " -> " << iter->second << std::endl
      ;
    }
  }

  os
    << "}" << std::endl
  ;
}

template <typename E> const std::string enum_2_string_stree<E>::statement() const
{
  std::ostringstream ostr;
  statement(ostr);
  return ostr.str();
}

template <typename E> void enum_2_string_stree<E>::statement(std::ostream &os) const
{
  //TODO неэкономное расходование памяти, размер буфера должен соответствовать максимальной длине пути
  uint32_t level = 0;
  _stg_t pnodes(m_stg.size());
  const _st_node
      *pnode_cur
    , *pnode_parent
  ;
  std::vector<char> braces(m_stg.size());

  if (!m_stg.size())
  {
    return;
  }

  pnodes[0] = m_stg[0];

  while ((pnode_cur = pnodes[level]))
  {
    if ((pnode_parent = pnode_cur->pparent) && pnode_cur == pnode_parent->pfalse)
    {
      if (pnode_parent->type == ntiOr)
      {
        if (pnode_parent->ptrue)
        {
          os << " | ";
        }
        if (pnode_cur->ptrue && pnode_cur->pfalse && pnode_cur->type == ntiAnd)
        {
          os << '(';
          braces[level] = ')';
        }
      }
      else
      {
        os << ' ';
      }
    }

    if (pnode_cur->type & ntiValue)
    {
      if (pnode_cur->type == ntiValueContradicted)
      {
        os << '?';
      }
      else if (pnode_cur->type == ntiValueAmbiguous)
      {
        os << '*';
      }
      os << pnode_cur->name;
    }

    pnode_parent = pnode_cur;
    if ((pnode_cur = pnode_parent->ptrue))
    {
      level ++;
      if (pnode_parent->type == ntiAnd)
      {
        os << '[';
        braces[level] = ']';
      }
      else if (pnode_parent->type == ntiMask)
      {
        os << '(';
        braces[level] = ')';
      }
      else if (pnode_parent->type == ntiFilter)
      {
        os << '{';
        braces[level] = '}';
      }
      else
      {
        braces[level] = '\0';
      }
      pnodes[level] = pnode_cur;
    }
    else if ((pnode_cur = pnode_parent->pfalse))
    {
      level ++;
      pnodes[level] = pnode_cur;
      braces[level] = '\0';
    }
    else
    {
      do
      {
        pnode_cur = pnodes[level];
        if ((pnode_parent = pnode_cur->pparent))
        {
          const char _brace = braces[level];
          if (pnode_cur == pnode_parent->ptrue)
          {
            if (_brace)
            {
              os << _brace;
            }
            if ((pnode_cur = pnode_parent->pfalse))
            {
              braces[level] = '\0';
              pnodes[level] = pnode_cur;
              break;
            }
          }
          else if (pnode_cur == pnode_parent->pfalse && _brace)
          {
            os << _brace;
          }
        }
        if (level)
        {
          level --;
        }
      }
      while (level);

      if (!level)
      {
        break;
      }
    }
  }
}

template <typename E> const uint32_t enum_2_string_stree<E>::depth_max() const
{
  return m_depth_max;
}

template <typename E> bool enum_2_string_stree<E>::filled() const
{
  return m_stg.size();
}

template <typename E> enum_2_string_stree<E>::enum_2_string_stree()
  : m_depth_max(0U)
{
}

template <typename E> enum_2_string_stree<E>::~enum_2_string_stree()
{
  for (typename _stg_t::const_iterator iter = m_stg.begin(); iter != m_stg.end(); iter ++)
  {
    delete *iter;
  }
}

template <typename E> enum_2_string_stree<E>::_st_node::_st_node(const E _value, const node_types _type, const std::string &_name, const _st_node *_pparent, const uint32_t _depth)
  : depth(_depth)
  , ptrue(NULL)
  , pfalse(NULL)
  , pparent(_pparent)
  , value(_value)
  , type(_type)
  , name(_name)
{
}

template <typename E> enum_2_string_stree<E>::_st_node::_st_node(const E _value, const node_types _type, const _st_node *_pparent, const uint32_t _depth)
  : depth(_depth)
  , ptrue(NULL)
  , pfalse(NULL)
  , pparent(_pparent)
  , value(_value)
  , type(_type)
{
}

template <typename E> bool enum_2_string_stree<E>::m_convert(std::string *pres, const E value) const
{
#ifdef _DEBUG_UTILS_ENSH
  uint32_t __iters_cnt = 0;
#endif //_DEBUG_UTILS_ENSH
  bool
      ret_val = true
    , name_found = false
    , zero_checked = false
  ;
  const _st_node
      *pnode_cur
    , *pnode_child
  ;
  std::vector<stack_item> _stack(m_depth_max + 1);
  stack_item *pstack = &_stack[0];

  pstack->pnode = m_stg[0];
  pstack->value = value;

  while ((pnode_cur = pstack->pnode))
  {
  #ifdef _DEBUG_UTILS_ENSH
    __iters_cnt ++;
  #endif //_DEBUG_UTILS_ENSH
    const E value_cur = pstack->value;

    pnode_child = pnode_cur->pfalse;
    if (pnode_child && (pnode_child->type & ntiValue) || pnode_cur->type == ntiOr && value_cur == pnode_cur->value)
    {//данный уровень стека закончен
      pstack->pnode = NULL;
    }
    else
    {//следующая итерация на том же уровне стека, текущим узлом будет ложная ветка
      if (pnode_cur->type == ntiAnd || pnode_cur->type == ntiMask || pnode_cur->type == ntiFilter)
      {
        pstack->value = E(value_cur & ~pnode_cur->value);
      }
      pstack->pnode = pnode_child;
    }
    if (!pnode_child && (pnode_cur->type == ntiOr && value_cur != pnode_cur->value || (pnode_cur->type == ntiMask || pnode_cur->type == ntiFilter || pnode_cur->type == ntiAnd) && (value_cur & ~pnode_cur->value)))
    {//условие не выполнено, а ложной ветки нет -- ошибка
      ret_val = false;
    #ifndef _DEBUG_UTILS
      //отладочная версия должна завершить преобразование до конца, все данные важны
      break;
    #endif //_DEBUG_UTILS
    }

    if (pnode_cur->type == ntiOr && !pnode_cur->value)
    {
      zero_checked = true;
    }

    if (pnode_cur->type == ntiMask || (pnode_cur->type == ntiFilter && (value_cur & pnode_cur->value)) || pnode_cur->type == ntiAnd && ((value_cur & pnode_cur->value) == pnode_cur->value) || pnode_cur->type == ntiOr && value_cur == pnode_cur->value)
    {//условие выполнено, переход на ветку истинности
      if ((pnode_child = pnode_cur->ptrue))
      {
        if (pnode_child->type & ntiValue)
        {//конечное значение
          if (pnode_cur->type != ntiMask || (value_cur & pnode_cur->value) == pnode_child->value)
          {
            if (pres)
            {
              if (name_found)
              {
                *pres += ' ';
              }
              *pres += pnode_child->name;
            }
            if (pnode_child->type == ntiValueContradicted || pnode_child->type == ntiValueAmbiguous)
            {
            #ifdef _DEBUG_UTILS
              if (pres)
              {
                *pres += (pnode_child->type == ntiValueContradicted ? '?' : '*');
              }
            #endif //_DEBUG_UTILS
              ret_val = false;
            #ifndef _DEBUG_UTILS
              break;
            #endif //_DEBUG_UTILS
            }
            name_found = true;
          }
        }
        else
        {//еще одно условие, проверить на следующей итерации
          pstack ++;
          pstack->pnode = pnode_child;
          pstack->value = (pnode_cur->type == ntiOr ? value_cur : E(value_cur & pnode_cur->value));
        #ifdef _DEBUG_UTILS_ENSH
          if (!__g_quiet)
          {
            std::cout << std::setw((pstack - &_stack[0]) * 2) << ' ' << '+' << ' ' << static_cast<size_t>(pstack->value) << ' ';
            __m_report_node(std::cout, pstack->pnode, 0);
          }
        #endif //_DEBUG_UTILS_ENSH
          continue;
        }
      }
      else
      {//ошибка, истинность условия не предусматривалась
        ret_val = false;
      #ifndef _DEBUG_UTILS
        break;
      #endif //_DEBUG_UTILS
      }
    }

    pnode_child = pnode_cur->pfalse;
    if (pnode_child && (pnode_child->type & ntiValue) && (pnode_cur->type == ntiAnd && !(value_cur & pnode_cur->value) || pnode_cur->type == ntiOr && value_cur != pnode_cur->value))
    {//условие не выполнено, вывод значения ложной ветки
      if (pres)
      {
        if (name_found)
        {
          *pres += ' ';
        }
        *pres += pnode_child->name;
      }
      if (pnode_child->type == ntiValueContradicted || pnode_child->type == ntiValueAmbiguous)
      {
      #ifdef _DEBUG_UTILS
        if (pres)
        {
          *pres += (pnode_child->type == ntiValueContradicted ? '?' : '*');
        }
      #endif //_DEBUG_UTILS
        ret_val = false;
      #ifndef _DEBUG_UTILS
        break;
      #endif //_DEBUG_UTILS
      }
      name_found = true;
    }

    while (!pstack->pnode)
    {
      if (pstack == &_stack[0])
      {
        break;
      }
    #ifdef _DEBUG_UTILS_ENSH
      if (!__g_quiet)
      {
        std::cout
          << std::setw((pstack - &_stack[0]) * 2) << ' ' << '-' << std::endl
        ;
      }
    #endif //_DEBUG_UTILS_ENSH
      pstack --;
    }
  }
#ifdef _DEBUG_UTILS_ENSH
  if (!__g_quiet)
  {
    std::cout
      << "Iterations (direct) " << __iters_cnt << std::endl
    ;
  }
#endif //_DEBUG_UTILS_ENSH
  return ret_val && name_found;
}

template <typename E> E enum_2_string_stree<E>::m_convert_reverse(_ids_t &ids) const
{
  typename _ids_t::iterator iter;
  E ret_val = E(0);
#ifdef _DEBUG_UTILS_ENSH
  uint32_t __iters_cnt = 0;
#endif //_DEBUG_UTILS_ENSH
  const _st_node
      *pnode_cur
    , *pnode_child
  ;
  std::vector<stack_item_reverse> _stack(m_depth_max + 1);
  stack_item_reverse *pstack = &_stack[0];

  pstack->pnode = m_stg[0];

  while ((pnode_cur = pstack->pnode))
  {
    bool
        step_true = true
      , required_string = false
    ;

    const stack_item_reverse *pstack_parent = (pstack == &_stack[0] ? NULL : pstack - 1);
    pstack->is_filter = (pnode_cur->type == ntiFilter || pstack_parent && pstack_parent->is_filter);

  #ifdef _DEBUG_UTILS_ENSH
    if (!__g_quiet)
    {
      __m_report_node(std::cout, pnode_cur, pstack - &_stack[0]);
    }
    __iters_cnt ++;
  #endif //_DEBUG_UTILS_ENSH

    pnode_child = pnode_cur->pfalse;
    if (pnode_child && (pnode_child->type & ntiValue))
    {
      if ((iter = ids.find(pnode_child->name)) != ids.end())
      {
        if (pnode_child->type == ntiValue)
        {
          ids.erase(iter);
          ret_val = E(ret_val | pnode_child->value);
          if (pnode_cur->type == ntiOr)
          {
            step_true = false;
          }
        }
        else
        {
          iter->second = string_types(pnode_child->type);
        }
      }
      pstack->pnode = NULL;
    }
    else
    {
      if (!pnode_child && pnode_cur->type == ntiOr && !pstack->is_filter)
      {//альтернатива не предусмотрена, если слева находится значение, оно обязательно должно быть задано в строке ids
        required_string = true;
      }
      pstack->pnode = pnode_child;
    }

    if (step_true && (pnode_child = pnode_cur->ptrue))
    {
      if (pnode_child->type & ntiValue)
      {
        if ((iter = ids.find(pnode_child->name)) != ids.end())
        {
          if (pnode_child->type == ntiValue)
          {
            ids.erase(iter);
            ret_val = E(ret_val | pnode_child->value);
            if (pnode_cur->type == ntiOr)
            {
              pstack->pnode = NULL;
            }
          }
          else
          {
            iter->second = string_types(pnode_child->type);
          }
        }
        else if (required_string)
        {
          const _st_node *pnode_parent = pnode_cur;
          std::ostringstream ostr;
          do
          {
            if (pnode_parent != pnode_cur)
            {
              ostr << '|';
            }
            ostr << pnode_child->name;
          }
          while ((pnode_parent = pnode_parent->pparent) && pnode_parent->type == ntiOr && (pnode_child = pnode_parent->ptrue) && pnode_child->type == ntiValue);

          ids.insert(typename _ids_t::value_type(ostr.str(), strtRequired));
        }
      }
      else
      {
        pstack ++;
        pstack->pnode = pnode_child;
        continue;
      }
    }

    while (!pstack->pnode)
    {
      if (pstack == &_stack[0])
      {
        break;
      }
      pstack --;
    }
  }
#ifdef _DEBUG_UTILS_ENSH
  if (!__g_quiet)
  {
    std::cout
      << "Iterations (reverse) " << __iters_cnt << std::endl
    ;
  }
#endif //_DEBUG_UTILS_ENSH
  return ret_val;
}

template <typename E> void enum_2_string_stree<E>::m_build_error_description(std::ostream &os, _ids_t &ids) const
{
  bool group_delimiter_needed = false;

  os << "Following ";
  for (uint8_t pos = 0; pos < 5; pos ++)
  {
    const char *_description;
    string_types _st;
    switch (pos)
    {
      case 0:
        _description = "ignored";
        _st = strtIgnored;
        break;
      case 1:
        _description = "contradicted";
        _st = strtContradicted;
        break;
      case 2:
        _description = "ambiguous";
        _st = strtAmbiguous;
        break;
      case 3:
        _description = "required";
        _st = strtRequired;
        break;
      case 4:
        _description = "invalid";
        _st = strtInvalid;
        break;
    }
    bool delimiter_needed = false;
    for (typename _ids_t::const_iterator iter = ids.begin(); iter != ids.end(); iter ++)
    {
      if (iter->second == _st)
      {
        if (delimiter_needed)
        {
          os << "\", \"";
        }
        else
        {
          if (group_delimiter_needed)
          {
            os << ", ";
          }
          os << "strings {\"";
          group_delimiter_needed = true;
        }
        os << iter->first;
        delimiter_needed = true;
      }
    }
    if (delimiter_needed)
    {
      os << "\"} " << _description;
    }
  }
  os << " for type \"" << typeid(E) << "\"";
}

template <typename E> const E enum_2_string_stree<E>::m_calculate_value(node_types &_type, const bool is_true, const _st_node *_pparent)
{
  typedef std::set<E> values_t;
  bool
      zero_enabled = true
    , and_final_assigned = false
    , and_final_valid = true
    , mask_assigned = false
  ;
  E
      ret_val = E(0)
    , mask
    , and_final
    , and_cur
  ;
  values_t
      values_include
    , values_exclude
  ;

  const _st_node *_pprev = NULL;

  /*
  Проход от текущего узла к вершине дерева решений -- единственная ветвь, ведущая к текущему узлу. Совокупное влияние (как истинное, так и ложное) всех условий должно давать единственное значение. Узлы:
    ntiAnd -- ложные добавляются в объединенную маску mask, истинные добавляют в values_include единственно-возможное значение при условии непротиворечивости; два узла ntiAnd противоречат друг другу в случае, если последующий содержит хотя бы один бит, не установленный в предыдущем; например узлы в порядке 1, 3, 5 противоречивы, а 5, 3, 1 -- нет;
    ntiMask -- формируют объединенную маску mask;
    ntiOr -- формируют два списка values_include и values_exclude допустимых и запрещенных значений соответственно
  */
  while (_pparent)
  {
    if (_pprev && _pprev == _pparent->ptrue || !_pprev && is_true)
    {//"положительная" ветвь
      if (_pparent->type == ntiMask || _pparent->type == ntiFilter)
      {
        if (_pparent->type == ntiFilter)
        {
          zero_enabled = false;
        }
        mask = mask_assigned ? E(mask & _pparent->value) : _pparent->value;
        mask_assigned = true;
      }
      else if (_pparent->type == ntiAnd)
      {
        if (!and_final_assigned)
        {
          and_cur = and_final = _pparent->value;
          and_final_assigned = true;
        }
        else if (and_final_valid)
        {
          if (and_cur == E(and_cur & _pparent->value))
          {
            and_cur = _pparent->value;
          }
          else
          {
            and_final_valid = false;
          }
        }
      }
      else if (_pparent->type == ntiOr)
      {
        values_include.insert(_pparent->value);
      }
    }
    else
    {//"отрицательная" ветвь
      if (_pparent->type == ntiMask || _pparent->type == ntiFilter || _pparent->type == ntiAnd)
      {
        mask = mask_assigned ? E(mask & ~_pparent->value) : E(~_pparent->value);
        mask_assigned = true;
      }
      else if (_pparent->type == ntiOr)
      {
        values_exclude.insert(_pparent->value);
      }
    }
    _pprev = _pparent;
    _pparent = _pparent->pparent;
  }

  if (and_final_assigned && and_final_valid)
  {
    values_include.insert(and_final);
  }

  if (
       values_include.size() > 1 //несколько явно заданных значений
    || !and_final_valid //узлы ntiAnd противоречат друг другу
    // условие !mask какой бы противоречивой ни была маска допускает значение 0, будет проверено ниже
  )
  {
    _type = ntiValueContradicted;
  }
  else if (values_include.size() == 1)
  {//единственное значение
    ret_val = *values_include.begin();
    if (values_exclude.count(ret_val) || mask_assigned && (E(ret_val & mask) != ret_val))
    {//значение запрещено одним из вышестоящих узлов или маской
      _type = ntiValueContradicted;
    }
    else
    {
      _type = ntiValue;
    }
  }
  else if (mask_assigned)
  {//values_include.size() == 0
    const size_t vars_capacity = values_exclude.size() + 1;
    std::vector<size_t> vars(vars_capacity);
    /*
    Количество вариантов значений маски -- M = 2 ** N, где N -- количество установленных битов в маске
    Если из всего списка M значений, M - 1 являются исключениями (values_exclude), итоговое значение определено однозначно, если список значений превосходит список исключений, итоговое значение неоднозначно и список не проверяется (и даже не заполняется)
    */
    const size_t vars_cnt = expand_mask<size_t>(&vars[0], vars_capacity, size_t(mask), zero_enabled);
    if (vars_cnt <= vars_capacity)
    {
      size_t value_cnt = 0;
      for (size_t pos = 0; pos < vars_cnt; pos ++)
      {
        if (!values_exclude.count(E(vars[pos])))
        {
          if (!value_cnt)
          {
            ret_val = E(vars[pos]);
          }
          value_cnt ++;
        }
      }
      if (!value_cnt)
      {
        _type = ntiValueContradicted;
      }
      else if (value_cnt == 1)
      {
        _type = ntiValue;
      }
    }
    else
    {
      _type = ntiValueAmbiguous;
    }
  }
  else
  {
    _type = ntiValueAmbiguous;
  }

  return ret_val;
}

template <typename E> std::string enum_2_string_stree<E>::m_graph_node
(
    const _st_node *pnode
  , const size_t node_id
  , const bool label
  , const bool indent
)
{
  std::ostringstream ostr;
  m_graph_node(ostr, pnode, node_id, label, indent);
  return ostr.str();
}

template <typename E> void enum_2_string_stree<E>::m_graph_node
(
    std::ostream &os
  , const _st_node *pnode
  , const size_t node_id
  , const bool label
  , const bool indent
)
{
  if (indent)
  {
    os << "  ";
  }

  switch (pnode->type)
  {
    case ntiAnd:
      os << "_z_and_" << node_id;
      if (label)
      {
        os << " [label=\"&" << size_t(pnode->value);
        if (pnode->depth)
        {
          os << '@' << pnode->depth;
        }
        os
          << "\"]" << std::endl
        ;
      }
      break;
    case ntiOr:
      os << "_z_or_" << node_id;
      if (label)
      {
        os << " [label=\"=" << size_t(pnode->value);
        if (pnode->depth)
        {
          os << '@' << pnode->depth;
        }
        os
          << "\"]" << std::endl
        ;
      }
      break;
    case ntiMask:
      os << "_z_mask_" << node_id;
      if (label)
      {
        os << " [label=\"<" << size_t(pnode->value);
        if (pnode->depth)
        {
          os << '@' << pnode->depth;
        }
        os
          << ">\"]" << std::endl
        ;
      }
      break;
    case ntiFilter:
      os << "_z_fltr_" << node_id;
      if (label)
      {
        os << " [label=\"[" << size_t(pnode->value);
        if (pnode->depth)
        {
          os << '@' << pnode->depth;
        }
        os
          << "]\"]" << std::endl
        ;
      }
      break;
    case ntiValue:
      os << "_a_" << node_id;
      if (label)
      {
        os << " [label=\"" << pnode->name;
        if (pnode->depth)
        {
          os << '@' << pnode->depth;
        }
        os
          << "\\ =\\ " << size_t(pnode->value) << "\"]" << std::endl
        ;
      }
      break;
    case ntiValueContradicted:
      os << "_a_" << node_id;
      if (label)
      {
        os << " [label=\"" << pnode->name;
        if (pnode->depth)
        {
          os << '@' << pnode->depth;
        }
        os
          << "\"]" << std::endl
        ;
      }
      break;
    case ntiValueAmbiguous:
      os << "_a_" << node_id;
      if (label)
      {
        os << " [label=\"" << pnode->name;
        if (pnode->depth)
        {
          os << '@' << pnode->depth;
        }
        os
          << "\"]" << std::endl
        ;
      }
      break;
    default: //если узел был заведен ошибочно, например (npOr | npAnd)
      os << "_z_amb_" << node_id;
      if (label)
      {
        os << " [label=\"" << size_t(pnode->value);
        if (pnode->depth)
        {
          os << '@' << pnode->depth;
        }
        os
          << " type = " << size_t(pnode->type) << "?\"]" << std::endl
        ;
      }
      break;
  }
}

#ifdef _DEBUG_UTILS
template <typename E> void enum_2_string_stree<E>::__m_report_node (std::ostream &os, const _st_node *pnode, const size_t level)
{
  if (level)
  {
    os << std::setw(level * 2) << ' ';
  }

  switch (pnode->type)
  {
    case ntiAnd:
      os << "&" << size_t(pnode->value);
      if (size_t(pnode->value) > 0x9)
      {
        os << " (0x" << std::hex << size_t(pnode->value) << std::dec << ")";
      }
      break;
    case ntiOr:
      os << "=" << size_t(pnode->value);
      if (size_t(pnode->value) > 0x9)
      {
        os << " (0x" << std::hex << size_t(pnode->value) << std::dec << ")";
      }
      break;
    case ntiMask:
      os << '<' << size_t(pnode->value) << '>';
      if (size_t(pnode->value) > 0x9)
      {
        os << " (0x" << std::hex << size_t(pnode->value) << std::dec << ")";
      }
      break;
    case ntiFilter:
      os << '[' << size_t(pnode->value) << ']';
      if (size_t(pnode->value) > 0x9)
      {
        os << " (0x" << std::hex << size_t(pnode->value) << std::dec << ")";
      }
      break;
    case ntiValue:
    case ntiValueContradicted:
    case ntiValueAmbiguous:
      os << '\"' << pnode->name << '\"';
      break;
    default: //если узел был заведен ошибочно, например (npOr | npAnd)
      os << '?' << size_t(pnode->value) << '?';
      if (size_t(pnode->value) > 0x9)
      {
        os << " (0x" << std::hex << size_t(pnode->value) << std::dec << ")";
      }
      break;
  }
  os
    << std::endl
  ;
}
#endif //_DEBUG_UTILS

#endif //__enum_shell_h__
