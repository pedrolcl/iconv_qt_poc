/*
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
*/

#include <stdexcept>
#include <QScopedArrayPointer>
#include <QDebug>

#include "iconvconverter.h"

const ::iconv_t ICONV_INVALID = reinterpret_cast< ::iconv_t >(-1);
const ::size_t ICONV_ERROR = static_cast< ::size_t >(-1);

IconvConverter::IconvConverter(const char* source):
    m_iconv(ICONV_INVALID)
{
    open(source);
}

IconvConverter::IconvConverter(const QByteArray& source):
    m_iconv(ICONV_INVALID)
{
    open(source);
}

IconvConverter::~IconvConverter()
{
    close();
}

void IconvConverter::reset()
{
    if (m_iconv != ICONV_INVALID) {
        ::iconv(m_iconv, nullptr, nullptr, nullptr, nullptr);
    }
}

QByteArray IconvConverter::source() const
{
    return m_source;
}

void IconvConverter::open(const QByteArray &source)
{
    ::iconv_t tmp = ::iconv_open("UTF-8", source.data());
    if (ICONV_INVALID == tmp) {
		throw std::runtime_error("iconv initialization failed");
    }
    if (ICONV_INVALID != m_iconv) {
		::iconv_close(m_iconv);
    }
	m_iconv = tmp;
	m_source = source;
}

void IconvConverter::close()
{
    if (ICONV_INVALID != m_iconv) {
		::iconv_close(m_iconv);
    }
    m_iconv = ICONV_INVALID;
    m_source.clear();
}

QByteArray IconvConverter::convert(const QByteArray &text)
{
    ::size_t source_size = text.size();
    ::size_t dest_size = 6 * source_size;
    QByteArray source_buffer(text.data(), source_size);
    QByteArray dest_buffer(dest_size, Qt::Uninitialized);
    ::size_t tmp_source_size = source_size;
    ::size_t tmp_dest_size = dest_size;
    char* source_ptr = source_buffer.data();
    char* dest_ptr = dest_buffer.data();
    if (ICONV_ERROR == ::iconv(m_iconv, &source_ptr, &tmp_source_size, &dest_ptr, &tmp_dest_size)) {
        throw std::runtime_error("iconv conversion failed");
    }
    dest_buffer.resize( dest_size - tmp_dest_size );
    return dest_buffer;
}
