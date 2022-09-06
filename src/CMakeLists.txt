# YAEP (Yet Another Earley Parser)
#
# Copyright (c) 1997-2018  Vladimir Makarov <vmakarov@gcc.gnu.org>
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# Generate sources from yacc first
find_package( BISON REQUIRED )
bison_target( sgramm sgramm.y ${CMAKE_CURRENT_BINARY_DIR}/sgramm.c )
if( NOT ${BISON_sgramm_DEFINED} )
	message( FATAL_ERROR "Failed to add rule for sgramm.c" )
endif()

# Now build libraries

# C
set( YAEP_LIB ${PROJECT_NAME} )
if( UNIX )
	set( YAEP_LIB_STATIC ${YAEP_LIB} )
else()
	set( YAEP_LIB_STATIC "${YAEP_LIB}_static" )
endif()
set( YAEP_LIB_SOURCES
	allocate.c
	hashtab.c
	objstack.c
	vlobject.c
	yaep.c
	${BISON_sgramm_OUTPUTS}
)
add_library( yaep_static STATIC ${YAEP_LIB_SOURCES} )
set_target_properties( yaep_static PROPERTIES OUTPUT_NAME ${YAEP_LIB_STATIC} )
target_include_directories(
	yaep_static PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/yaep_internal
)
target_include_directories( yaep_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

# Install
set( YAEP_INCLUDE
	yaep.h
)
install( TARGETS yaep_static DESTINATION ${CMAKE_INSTALL_LIBDIR} )
install( FILES ${YAEP_INCLUDE} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}" )
