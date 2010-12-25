# Build a Graphic Equaliser example.
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

project(geq)
find_package(SDL REQUIRED)
set(geq_SRCS main.cpp)
include_directories(${SDL_INCLUDE_DIR})
add_executable(geq ${geq_SRCS})
target_link_libraries(geq mattulizer)
add_dependencies(examples geq)
