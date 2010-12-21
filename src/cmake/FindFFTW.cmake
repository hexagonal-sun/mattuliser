# Locate fftw library
# This module defines
# FFTW_LIBRARY, the name of the library to link against
# FFTW_FOUND, if false, do not try to link to FFTW
# FFTW_INCLUDE_DIR, where to find fftw3.h
#
# Copyright 2010 (c) Matthew Leach.
#
# This file is part of mattulizer.
#
# Mattulizer is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# Mattulizer is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Mattulizer.  If not, see <http://www.gnu.org/licenses/>.#

FIND_PATH(FFTW_INCLUDE_DIR fftw3.h
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include
  /opt/include
)

FIND_LIBRARY(FFTW_LIBRARY
  NAMES fftw3
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

SET(FFTW_FOUND "NO")
IF(FFTW_LIBRARY AND FFTW_INCLUDE_DIR)
  SET(FFTW_FOUND "YES")
ENDIF(FFTW_LIBRARY AND FFTW_INCLUDE_DIR)
