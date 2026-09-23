#
#
# Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
# http://www.fpvidal.net/
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# 3. Neither the name of the Bangor University nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


################################################################################
#
#	@file		Bin2C.cmake
#
#	@brief		Configuration file to generate a C header file from any file.
#			 using the CMake tool. (see http://www.cmake.org/)
#
#	@version	1.0
#
#	@date		02/03/2014
#
#	@author		Dr Franck P. Vidal
#
#	@section	License
#				BSD 3-Clause License.
#
#				For details on use and redistribution please refer
#				to http://opensource.org/licenses/BSD-3-Clause
#
#	@section	Copyright
#				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
#				http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
#				BSD 3-Clause License
#
################################################################################


# Minimum CMake version ########################################################
cmake_minimum_required(VERSION 3.26 FATAL_ERROR)


# portably convert binary file to header
FUNCTION(FILE2C INFILE VARNAME OUTFILE)
	FILE(READ ${INFILE} HEXFILE HEX)
	STRING(LENGTH ${HEXFILE} XRSLEN)
	SET(HEXPOS 0)
	SET(OUTPUT_CONTENT "/* generated from ${INFILE}.tmp; do not edit */\nconst unsigned char ${VARNAME}[] = {")
	
	WHILE(${HEXPOS} LESS ${XRSLEN})
		MATH(EXPR LPOS "${HEXPOS} % 32")
	
		IF(NOT ${LPOS})
			STRING(APPEND OUTPUT_CONTENT "\n")
		ENDIF(NOT ${LPOS})
	
		STRING(SUBSTRING ${HEXFILE} ${HEXPOS} 2 HEXBYTE)
		STRING(APPEND OUTPUT_CONTENT "0x${HEXBYTE}")
		MATH(EXPR HEXPOS "${HEXPOS} + 2")
	
		IF(${HEXPOS} LESS ${XRSLEN})
			STRING(APPEND OUTPUT_CONTENT ",")
		ENDIF(${HEXPOS} LESS ${XRSLEN})
	ENDWHILE(${HEXPOS} LESS ${XRSLEN})
	STRING(APPEND OUTPUT_CONTENT "};\n")
	FILE(WRITE ${OUTFILE} "${OUTPUT_CONTENT}")
ENDFUNCTION(FILE2C)
