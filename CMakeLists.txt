# Top Level build.
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

cmake_minimum_required(VERSION 2.8)

add_subdirectory(src)
add_custom_target(examples)
add_subdirectory(examples EXCLUDE_FROM_ALL)
