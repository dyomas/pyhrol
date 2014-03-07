#  Copyright (c) 2013 Pyhrol, pyhrol@rambler.ru
#  GEO: N55.703431,E37.623324 .. N48.742359,E44.536997
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#  4. Neither the name of the Pyhrol nor the names of its contributors
#     may be used to endorse or promote products derived from this software
#     without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
#  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
#  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
#  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
#  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
#  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
#  SUCH DAMAGE.
#
# Find Python library and headers and define targets for extension modules
# Behaviour:
#
#  -- If Python library and headers found, define targets:
#     pygen, pyhrol[safe], pyself[safe] used by any Pyhrol based projects
#
#  -- If Python library or headers not found, and PYTHON_REQUIRED defined
#     fatal error occured
#
#  -- If Python library or headers not found or invalid, and PYTHON_REQUIRED
#     not defined warns and continue;
#     Python related targets must be inactivated by PYTHONLIBS_FOUND switch

# $Date: 2014-02-27 22:19:34 +0400 (Thu, 27 Feb 2014) $
# $Revision: 896 $

find_package (PythonLibs 2.4)


set (PYHROL_MAX_INPUT_ARGS 16 CACHE STRING "Max number of arguments for PyArg_*ParseTuple* function, limited by the number of relevant macros PYHROL_PARSE_TUPLE_{M}")
set (PYHROL_MAX_OUTPUT_ARGS 8 CACHE STRING "Max number of arguments for Py_VaBuildValue, limited by the number of relevant macros PYHROL_BUILD_VALUE_{N}")
# set (PYHROL_DESTINATION lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/dist-packages/ CACHE PATH "Directory where compiled Pyhrol modules should be installed")

if (NOT PYTHONLIBS_FOUND)
  if (PYTHON_REQUIRED)
    message (FATAL_ERROR "Python libraries not found")
  else()
    message (WARNING "Python related targets disabled")
  endif()
endif()

if (CMAKE_VERBOSE_MAKEFILE)
  message (STATUS "PythonLibs
  PYTHONLIBS_FOUND           = \"${PYTHONLIBS_FOUND}\"
  PYTHON_LIBRARIES           = \"${PYTHON_LIBRARIES}\"
  PYTHON_INCLUDE_PATH        = \"${PYTHON_INCLUDE_PATH}\"
  PYTHON_INCLUDE_DIRS        = \"${PYTHON_INCLUDE_DIRS}\"
  PYTHON_DEBUG_LIBRARIES     = \"${PYTHON_DEBUG_LIBRARIES}\"
  PYTHONLIBS_VERSION_STRING  = \"${PYTHONLIBS_VERSION_STRING}\"
")
endif()

string(REPLACE ";" " -I" TMP_INCLUDE "${PYTHON_INCLUDE_DIRS}")

if (PYTHONLIBS_FOUND)
  set (PYHROL_SAFE_MODE OFF CACHE BOOL "Use library version with minimal functionality")

  set (PYHROL_BASE
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/array_square.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/binary_operations.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/demangling.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_auto_holder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_common.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_container.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_exception_handler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_index.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_keywords.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_trace.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_describer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_in.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_out.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuples.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuples_exceptions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_type_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_type_iterable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_type_number_any.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_type_sequence.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_type_traits.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/self_addressed_cstring.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/signature_parser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/strings_manip.cpp
  )

  set (PYHROL_COMMON_DEV
    ${PYHROL_BASE}
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/array_void.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/printf_parser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_check_calls.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_describer_flexible.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_fill_state.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_fill_state_getsetter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_fill_state_normal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_format.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_in_default_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_out_default_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_state.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuples_data2_default_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuples_data_default_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuples_default_impl.cpp
  )

  set (PYHROL_COMMON_LIB
    ${PYHROL_COMMON_DEV}
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_init.cpp
  )

  add_library(pyhrol SHARED ${PYHROL_COMMON_LIB})
  add_dependencies(pyhrol pygen)
  set_target_properties (pyhrol PROPERTIES
    COMPILE_FLAGS "-I${PYTHON_INCLUDE_PATH}"
    COMPILE_DEFINITIONS USE_TRACER
  )
  target_link_libraries (pyhrol ${PYTHON_LIBRARIES})

  set (PYHROL_COMMON
    ${PYHROL_COMMON_LIB}
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_starter.cpp
  )

  set (PYHROL_SAFE_DEV
    ${PYHROL_BASE}
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_base_safe.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_describer_safe.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_in_safe_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_out_safe_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuples_data_safe_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuples_safe_impl.cpp
  )

  set (PYHROL_SAFE_LIB
    ${PYHROL_SAFE_DEV}
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_init.cpp
  )

  add_library(pyhrolsafe SHARED ${PYHROL_SAFE_LIB})
  add_dependencies(pyhrolsafe pygen)
  set_target_properties (pyhrolsafe PROPERTIES
    COMPILE_FLAGS "-I${PYTHON_INCLUDE_PATH}"
    COMPILE_DEFINITIONS "USE_TRACER; PYHROL_SAFE_MODE"
  )
  target_link_libraries (pyhrolsafe ${PYTHON_LIBRARIES})

  if (PYHROL_SAFE_MODE)
    set (PYHROL_SUFFIX safe)
  endif()


  add_executable(pygen
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/strings_manip.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_generator.cpp
  )
  set_target_properties (pygen PROPERTIES
    OUTPUT_NAME __pygen
    COMPILE_FLAGS "-I${CMAKE_CURRENT_LIST_DIR}/../include"
  )
  add_custom_command(
    TARGET pygen POST_BUILD
    COMMAND $<TARGET_FILE:pygen> -L ../LICENSE -C -m1 -N${PYHROL_MAX_INPUT_ARGS} > pyhrol_fmt_in_macro.h
    COMMAND $<TARGET_FILE:pygen> -L ../LICENSE -C -m2 -N${PYHROL_MAX_OUTPUT_ARGS} > pyhrol_fmt_out_macro.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../include
    COMMENT "Generating pyhrol PARSE/BUILD macros"
  )


  set (PYSELF_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_flags.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_starter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_self.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_self_modules.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_self_types.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_self_methods.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_self_functions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/smart_flag_int.cpp
  )

  add_library(pyself MODULE ${PYSELF_SOURCE})
  set_target_properties (pyself PROPERTIES
    PREFIX ""
    COMPILE_DEFINITIONS "USE_TRACER;_OUTPUT=pyhrol"
    COMPILE_FLAGS "${TMP_FLAGS} \\
      -I${TMP_INCLUDE} \\
      -I${CMAKE_CURRENT_LIST_DIR}/../include"
    OUTPUT_NAME pyselfstd
  )
  target_link_libraries (pyself pyhrol)
  add_dependencies(pyself pygen)


  add_library(pyselfsafe MODULE ${PYSELF_SOURCE})
  set_target_properties (pyselfsafe PROPERTIES
    PREFIX ""
    COMPILE_DEFINITIONS "USE_TRACER;_OUTPUT=pyhrol;PYHROL_SAFE_MODE"
    COMPILE_FLAGS "${TMP_FLAGS} \\
      -I${TMP_INCLUDE} \\
      -I${CMAKE_CURRENT_LIST_DIR}/../include"
    OUTPUT_NAME pyselfsafe
  )
  target_link_libraries (pyselfsafe pyhrolsafe)
  add_dependencies(pyselfsafe pygen)


  add_custom_target(link COMMAND sh -c "ln -sf $<TARGET_FILE:pyself${PYHROL_SUFFIX}> pyhrol.so" DEPENDS pyself pyselfsafe)

endif (PYTHONLIBS_FOUND)


function (pyhrol_configure_targets)
  string(REGEX MATCHALL "([^ ]+)" PYHROL_TARGETS "${ARGV}")
  foreach (PYHROL_TARGET ${PYHROL_TARGETS})
    get_target_property (TMP_NAME ${PYHROL_TARGET} PREFIX)
    if (NOT TMP_NAME)
      set_target_properties (${PYHROL_TARGET} PROPERTIES PREFIX "")
    endif()
    get_target_property (TMP_NAME ${PYHROL_TARGET} LOCATION)
    get_filename_component(TMP_NAME ${TMP_NAME} NAME_WE)
    get_target_property (TMP_FLAGS ${PYHROL_TARGET} COMPILE_FLAGS)
    if (NOT TMP_FLAGS)
      unset(TMP_FLAGS)
    endif()
    get_target_property (TMP_DEFS ${PYHROL_TARGET} COMPILE_DEFINITIONS)
    if (NOT TMP_DEFS)
      unset(TMP_DEFS)
    endif()

    set_target_properties (${PYHROL_TARGET} PROPERTIES
      COMPILE_FLAGS "${TMP_FLAGS} \\
        -I${TMP_INCLUDE} \\
        -I${CMAKE_CURRENT_LIST_DIR}/../include"
      COMPILE_DEFINITIONS "${TMP_DEFS}; _OUTPUT=${TMP_NAME}"
    )

    if (PYTHONLIBS_FOUND)
      target_link_libraries (${PYHROL_TARGET} pyhrol${PYHROL_SUFFIX} ${PYTHON_LIBRARIES})
      add_dependencies(${PYHROL_TARGET} pygen link)
    endif()

    if (CMAKE_VERBOSE_MAKEFILE)
      get_target_property (TMP_FLAGS ${PYHROL_TARGET} COMPILE_FLAGS)
      message (STATUS "CXXFLAGS for ${PYHROL_TARGET}: \"${TMP_FLAGS}\"")
      get_target_property (TMP_DEFS ${PYHROL_TARGET} COMPILE_DEFINITIONS)
      message (STATUS "Definitions for ${PYHROL_TARGET}: \"${TMP_DEFS}\"")
    endif()
  endforeach()
endfunction (pyhrol_configure_targets)


