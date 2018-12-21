# portably convert binary file to header
function(FILE2C INFILE VARNAME OUTFILE)
  file(READ ${INFILE} HEXFILE HEX)
  string(LENGTH ${HEXFILE} XRSLEN)
  set(HEXPOS 0)
  file(WRITE ${OUTFILE}
          "/* generated from ${INFILE}; do not edit */\n"
          "const unsigned char ${VARNAME}[] = {")
  while(${HEXPOS} LESS ${XRSLEN})
    math(EXPR LPOS "${HEXPOS} % 32")
    if(NOT ${LPOS})
      file(APPEND ${OUTFILE} "\n")
    endif(NOT ${LPOS})
    string(SUBSTRING ${HEXFILE} ${HEXPOS} 2 HEXBYTE)
    file(APPEND ${OUTFILE} "0x${HEXBYTE}")
    math(EXPR HEXPOS "${HEXPOS} + 2")
    if(${HEXPOS} LESS ${XRSLEN})
      file(APPEND ${OUTFILE} ",")
    endif(${HEXPOS} LESS ${XRSLEN})
  endwhile(${HEXPOS} LESS ${XRSLEN})
  file(APPEND ${OUTFILE} "};\n")
endfunction(FILE2C)

FILE2C(${INFILE} ${VARNAME} ${OUTFILE})