/*
 * This file is part of the lo2s software.
 * Linux OTF2 sampling
 *
 * Copyright (c) 2018,
 *    Technische Universitaet Dresden, Germany
 *
 * lo2s is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lo2s is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lo2s.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <lo2s/dtrace/types.hpp>

#include <memory>

namespace lo2s
{
namespace dtrace
{

class EventReader
{
public:
    EventReader();
    ~EventReader();

    void read();

    virtual void handle(int cpu, const ProbeDesc& pd, nitro::lang::string_ref data,
                        void* raw_data) = 0;

    lo2s::dtrace::Handle dtrace_handle() const
    {
        return handle_;
    }

    void init_dtrace_program(const std::string&);

private:
    static int consume_callback(ConstProbeDataPtr data, ConstRecDescPtr rec, void* arg);
    static int buffer_callback(ConstBufDataPtr bufdata, void* arg);

private:
    lo2s::dtrace::Handle handle_;
    std::unique_ptr<struct dtrace_proginfo> prog_info_;
};
} // namespace dtrace
} // namespace lo2s
