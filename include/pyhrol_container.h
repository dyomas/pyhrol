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

// $Date: 2014-03-07 00:18:03 +0400 (Fri, 07 Mar 2014) $
// $Revision: 899 $

#ifndef __pyhrol_container_h__
#define __pyhrol_container_h__

#include <stdint.h>
#include <pyport.h>
#include <object.h>
#include <methodobject.h>
#include <vector>
#include <map>
#include <set>
#include <stdexcept>
#include <tr1/memory>

namespace pyhrol
{

class Tuples;
class Index;

#ifndef PYHROL_SAFE_MODE
class TupleFormat;
#endif //PYHROL_SAFE_MODE

class Container
{
  public:
    typedef std::vector<PyTypeObject *> types_t;
    typedef std::pair<types_t::const_iterator, types_t::const_iterator> types_range_t;

    static const std::string ctor_caption;

    class WrappersReused: public std::logic_error
    {
    public:
      WrappersReused();
    };

    class TypeDeleted: public std::logic_error
    {
      const std::string m_describe(const size_t);
    public:
      TypeDeleted(const size_t);
    };

    class InvalidLink: public std::logic_error
    {
      const std::string m_describe(const void *);
    public:
      InvalidLink();
      InvalidLink(const void *);
    };

    virtual ~Container();
    static Container &container();
  #ifndef PYHROL_SAFE_MODE
    const TupleFormat &tupleFormat();
    void setTupleFormat(const char *th, const char *tb, const char *eh, const char *eb);
  #endif //PYHROL_SAFE_MODE

    PyMethodDef *functions();
    const PyMethodDef *functions(const size_t) const;
    types_range_t types() const;
    const std::string &module_name(const size_t) const;
    const Index &index() const;

    const PyTypeObject *type(const size_t) const;
    const char *protof_name(const size_t, const size_t) const;
    const char *protof_doc(const size_t, const size_t) const;

    const char **doc_by_address(const void *) const;
    const std::string context_by_address(const void *) const throw();

    void indexate(const char *module_name);
    void check_calls() const;
    void show_calls(std::ostream &) const;
  #ifndef PYHROL_SAFE_MODE
    void describe_tuples();
  #endif //PYHROL_SAFE_MODE

  #ifdef _DEBUG_PYTHON_CONTAINER
    void __add_dummy_function(const char */*name*/, const size_t address);
  #endif //_DEBUG_PYTHON_CONTAINER

    template <void (*F)(Tuples &)> void add_function(const char */*name*/, const char */*help*/ = NULL);
    template <void (*F)(Tuples &)> void add_function_with_keywords(const char */*name*/, const char */*help*/ = NULL);
    template <void (*F)(Tuples &)> void add_function_no_args(const char */*name*/, const char */*help*/ = NULL);

    const size_t register_type(PyTypeObject &type);
    void unregister_type(const size_t id);

  private:
    Container();
    Container(const Container &);
    Container &operator= (const Container &);

    typedef std::vector<PyMethodDef> module_functions_t;
    typedef std::vector<module_functions_t *> functions_t;

    enum call_types
    {
        ctFunction
      , ctMethod
      , ctGetter
      , ctSetter
      , ctCtor
      , ctNumberCall
      , ctSequenceCall
      , ctMapCall
      , ctOther
    };

    enum name_props
    {
        npOK
      , npUnderscorePrefixed
      , npIsReservedWord
    };

    struct call_position
    {
      const call_types ct;
      const size_t
          offset_type
        , offset_call
        , offset_module
      ;
    };

    struct artificial
    {
      void call() const;
      size_t address() const;
      artificial(binaryfunc, const char *, const call_types);
      artificial(ternaryfunc, const char *, const call_types);
      artificial(objobjproc, const char *, const call_types);
      artificial(ssizeobjargproc, const char *, const call_types);
      artificial(ssizessizeobjargproc, const char *, const call_types);
      artificial(objobjargproc, const char *, const call_types);
      const char * const name;
      const char *doc;
      const call_types ct;

    private:
      const char signature;
      union
      {
        binaryfunc callA; //nb(|_inplace)_(add|subtract|multiply|divide|remainder|lshift|rshift|and|xor|or) sq(|_inplace)_concat, mp_subscript
        ternaryfunc callB; //nb(|_inplace)_power
        objobjproc callC; //sq_contains
        ssizeobjargproc callD; //sq_ass_item
        ssizessizeobjargproc callE; //sq_ass_slice
        objobjargproc callF; //mp_ass_subscript
      };
    };

    struct module
    {
      size_t
          offset_self
        , types_begin
        , types_end
      ;
      std::string name;
    };

    typedef std::multimap<size_t, call_position> address_idx_t;
    typedef std::multimap<std::string, call_position> name_idx_t;
    typedef std::set<std::string> reserved_t;
    typedef std::map<std::pair<size_t, size_t>, artificial> artificials_t;
    typedef std::map<std::pair<size_t, size_t>, std::pair<const char *, const void *> > others_t;
    typedef std::vector<module> modules_t;

    template <void (*F)(Tuples &)> static PyObject *m_function(PyObject */*self*/);
    template <void (*F)(Tuples &)> static PyObject *m_function2(PyObject */*self*/, PyObject */*args*/);
    template <void (*F)(Tuples &)> static PyObject *m_function3(PyObject */*self*/, PyObject */*args*/, PyObject */*kwds*/);

    static const reserved_t m_init_reserved();
    const char m_call_type_2_char(const call_types) const;
    PyTypeObject *m_get_type(const size_t);
    const PyTypeObject *m_get_type(const size_t) const;
    void m_get_name_props(name_props &, name_props &, const call_position &) const;
    const std::string m_get_call_name(const call_position &, const bool with_type = true) const;
    void m_qualify_types_with_module_name(const module &);
    void m_fill_artificials(artificials_t &, const module &) const;
    void m_fill_others(others_t &, const module &) const;
    void m_indexate_addresses_and_names(address_idx_t &, name_idx_t &, const module &) const;
    void m_build_meta_index(Index &, const module &) const;
    const std::string m_show_addresses_and_names(const address_idx_t &, const name_idx_t &) const;
    void m_show_addresses_and_names(std::ostream &, const address_idx_t &, const name_idx_t &) const;
    bool m_check_addresses(const address_idx_t &) const;
    void m_check_names(const name_idx_t &) const;
    void m_describe_tuples(const module &);
    void m_add_function(const char *name, const PyCFunction &function, const int flags, const char *help);

    functions_t m_functions;
    types_t m_types;
    address_idx_t m_address_idx;
    name_idx_t m_name_idx;
    artificials_t m_artificials;
    others_t m_others;
    modules_t m_modules;
    std::auto_ptr<Index> m_index;
  #ifndef PYHROL_SAFE_MODE
    std::auto_ptr<TupleFormat> m_tuple_format;
  #endif //PYHROL_SAFE_MODE
};

}; //namespace pyhrol

#endif //__pyhrol_container_h__
