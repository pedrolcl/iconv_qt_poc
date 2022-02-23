#[===========================================================================[
    IconvQtPOC - Integrating iconv() for Unicode Conversion in Qt apps
    Copyright (C) 2022 Pedro Lopez-Cabanillas
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
#]===========================================================================]

cmake_minimum_required(VERSION 3.11)

project(IconvQtPOC VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Iconv REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules( UCHARDET REQUIRED IMPORTED_TARGET uchardet )

add_executable(IconvQtPOC
    iconvconverter.cpp
    iconvconverter.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

target_link_libraries(IconvQtPOC PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    PkgConfig::UCHARDET
    Iconv::Iconv
)
