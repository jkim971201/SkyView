function(add_swig)

# Parse args
set(options "")
set(oneValueArgs I_FILE NAME NAMESPACE)
set(multiValueArgs  "")
  
cmake_parse_arguments(
  ARG  # prefix on the parsed args
  "${options}"
  "${oneValueArgs}"
  "${multiValueArgs}"
  ${ARGN}
)

# Validate args
if (DEFINED ARG_UNPARSED_ARGUMENTS)
  message(FATAL_ERROR "Unknown argument(s) to add_swig: ${ARG_UNPARSED_ARGUMENTS}")
endif()

if (DEFINED ARG_KEYWORDS_MISSING_VALUES)
  message(FATAL_ERROR "Missing value for argument(s) to add_swig: ${ARG_KEYWORDS_MISSING_VALUES}")
endif()

foreach(arg I_FILE NAME NAMESPACE)
  if (NOT DEFINED ARG_${arg})
    message(FATAL_ERROR "${arg} argument must be provided to add_swig")
  endif()
endforeach()

set(LANGUAGE_OPTIONS -namespace -prefix ${ARG_NAMESPACE})
set_property(SOURCE ${ARG_I_FILE} PROPERTY CPLUSPLUS ON)
set_property(SOURCE ${ARG_I_FILE} PROPERTY SWIG_MODULE_NAME ${ARG_NAME})
set_property(SOURCE ${ARG_I_FILE} PROPERTY USE_SWIG_DEPENDENCIES TRUE)
set_property(SOURCE ${ARG_I_FILE} PROPERTY USE_TARGET_INCLUDE_DIRECTORIES true)
set_property(SOURCE ${ARG_I_FILE} PROPERTY COMPILE_OPTIONS ${LANGUAGE_OPTIONS})

swig_add_library(${ARG_NAME}
  LANGUAGE tcl
  TYPE     STATIC
  SOURCES  ${ARG_I_FILE}
)

target_include_directories(${ARG_NAME}
    PRIVATE
		${CMAKE_SOURCE_DIR}/src
)

endfunction()
