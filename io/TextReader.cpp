/******************************************************************************
* Copyright (c) 2016, Hobu Inc., info@hobu.co
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#include <pdal/PDALUtils.hpp>
#include <pdal/util/Algorithm.hpp>

#include "TextReader.hpp"

#include <pdal/pdal_macros.hpp>
#include <pdal/util/Algorithm.hpp>

namespace pdal
{

static PluginInfo const s_info = PluginInfo(
    "readers.text",
    "Text Reader",
    "http://pdal.io/stages/readers.text.html" );

CREATE_STATIC_PLUGIN(1, 0, TextReader, Reader, s_info)

std::string TextReader::getName() const { return s_info.name; }

void TextReader::initialize(PointTableRef table)
{
    m_istream = Utils::openFile(m_filename);
    if (!m_istream)
        throwError("Unable to open text file '" + m_filename + "'.");

    std::string buf;
    std::getline(*m_istream, buf);

    auto isspecial = [](char c)
        { return (!std::isalnum(c) && c != ' '); };

    // If the separator wasn't provided on the command line extract it
    // from the header line.
    if (m_separator == ' ')
    {
        // Scan string for some character not a number, space or letter.
        for (size_t i = 0; i < buf.size(); ++i)
            if (isspecial(buf[i]))
            {
                m_separator = buf[i];
                break;
            }
    }

    if (m_separator != ' ')
        m_dimNames = Utils::split(buf, m_separator);
    else
        m_dimNames = Utils::split2(buf, m_separator);
    Utils::closeFile(m_istream);
}


void TextReader::addArgs(ProgramArgs& args)
{
    args.add("separator", "Separator character that overrides special "
        "character in header line", m_separator, ' ');
}


void TextReader::addDimensions(PointLayoutPtr layout)
{
    for (auto name : m_dimNames)
    {
        Utils::trim(name);
        Dimension::Id id = layout->registerOrAssignDim(name,
            Dimension::Type::Double);
        if (Utils::contains(m_dims, id))
            throwError("Duplicate dimension '" + name +
                "' detected in input file '" + m_filename + "'.");
        m_dims.push_back(id);
    }
}


void TextReader::ready(PointTableRef table)
{
    m_istream = Utils::openFile(m_filename);
    if (!m_istream)
        throwError("Unable to open text file '" + m_filename + "'.");

    // Skip header line.
    std::string buf;
    std::getline(*m_istream, buf);
    m_line = 1;
}


point_count_t TextReader::read(PointViewPtr view, point_count_t numPts)
{
    PointId idx = view->size();
    point_count_t cnt = 0;
    PointRef point(*view, idx);
    while (cnt < numPts)
    {
        point.setPointId(idx);
        if (!processOne(point))
            break;
        cnt++;
        idx++;
    }
    return cnt;
}


bool TextReader::processOne(PointRef& point)
{
    if (!fillFields())
        return false;

    double d;
    for (size_t i = 0; i < m_fields.size(); ++i)
    {
        if (!Utils::fromString(m_fields[i], d))
        {
            log()->get(LogLevel::Error) << "Can't convert "
                "field '" << m_fields[i] << "' to numeric value on line " <<
                m_line << " in '" << m_filename << "'.  Setting to 0." <<
                std::endl;
            d = 0;
        }
        point.setField(m_dims[i], d);
    }
    return true;
}


bool TextReader::fillFields()
{
    while (true)
    {
        if (!m_istream->good())
            return false;

        std::string buf;

        std::getline(*m_istream, buf);
        m_line++;
        if (buf.empty())
            continue;
        if (m_separator != ' ')
        {
            Utils::remove(buf, ' ');
            m_fields = Utils::split(buf, m_separator);
        }
        else
            m_fields = Utils::split2(buf, m_separator);
        if (m_fields.size() != m_dims.size())
        {
            log()->get(LogLevel::Error) << "Line " << m_line <<
                " in '" << m_filename << "' contains " << m_fields.size() <<
                " fields when " << m_dims.size() << " were expected.  "
                "Ignoring." << std::endl;
            continue;
        }
        return true;
    }
}


void TextReader::done(PointTableRef table)
{
    Utils::closeFile(m_istream);
}


} // namespace pdal

