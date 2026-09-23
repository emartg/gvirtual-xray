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


FIND_PACKAGE(LATEX)
IF (LATEX_FOUND)
    FILE(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/doc/latex)
    FILE(COPY ${PROJECT_SOURCE_DIR}/doc/latex/xtab.sty    DESTINATION ${PROJECT_BINARY_DIR}/doc/latex)
    FILE(COPY ${PROJECT_SOURCE_DIR}/doc/latex/sectsty.sty DESTINATION ${PROJECT_BINARY_DIR}/doc/latex)
    FILE(COPY ${PROJECT_SOURCE_DIR}/doc/latex/tocloft.sty DESTINATION ${PROJECT_BINARY_DIR}/doc/latex)
ENDIF (LATEX_FOUND)


FIND_PACKAGE(Doxygen)


CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/doxygen/doxygen.config.in
                 ${PROJECT_BINARY_DIR}/doc/doxygen/doxygen.config)

FILE(COPY ${PROJECT_SOURCE_DIR}/doc/doxygen/index.rst DESTINATION ${PROJECT_BINARY_DIR}/doc)
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/doxygen/conf.py.in ${PROJECT_BINARY_DIR}/doc/conf.py)

ADD_CUSTOM_TARGET(Documentation 
    COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/doc/doxygen/doxygen.config
    MAIN_DEPENDENCY ${PROJECT_BINARY_DIR}/doc/doxygen/doxygen.config
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc/doxygen
)


ADD_CUSTOM_TARGET(sphinx 
    COMMAND 
    sphinx-build -b html -Dbreathe_projects.gVXR=xml ${PROJECT_BINARY_DIR}/doc ${PROJECT_BINARY_DIR}/doc/sphinx/
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc
)

ADD_DEPENDENCIES(sphinx Documentation)
