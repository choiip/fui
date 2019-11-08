# portably convert binary file to header
function(FILE2C INFILE VARNAME OUTFILE)
  if(NOT DEFINED INFILE)
    message(FATAL_ERROR
      "Required cmake variable INFILE not set!"
    )
  endif()

  if(NOT DEFINED OUTFILE)
    message(FATAL_ERROR
      "Required cmake variable OUTFILE not set!"
    )
  endif()

  if(NOT DEFINED VARNAME)
    message(FATAL_ERROR
      "Required cmake variable VARNAME not set!"
    )
  endif()

  if(NOT EXISTS ${INFILE})
    message(FATAL_ERROR "File '${INFILE}' does not exist!")
  endif()

  file(READ "${INFILE}" contents HEX)

  file(WRITE ${OUTFILE}
          "/* Generated from ${INFILE}; do not edit */\n"
          "#pragma once\n")

  file(APPEND "${OUTFILE}" "#ifdef __cplusplus\n")
  file(APPEND "${OUTFILE}" "extern \"C\" {\n")
  file(APPEND "${OUTFILE}" "#endif\n")

  file(APPEND "${OUTFILE}" "const unsigned char "
  "${VARNAME}[] = {\n")

  string(REGEX MATCHALL ".." output "${contents}")
  string(REGEX REPLACE ";" ",\n  0x" output "${output}")

  file(APPEND "${OUTFILE}" "  0x${output}\n")

  file(APPEND "${OUTFILE}" "};\n")

  file(APPEND "${OUTFILE}" "#ifdef __cplusplus\n")
  file(APPEND "${OUTFILE}" "}\n")
  file(APPEND "${OUTFILE}" "#endif\n")

endfunction(FILE2C)

FILE2C(${INFILE} ${VARNAME} ${OUTFILE})