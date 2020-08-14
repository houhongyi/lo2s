 /*
  * This file is part of the lo2s software.
  * Linux OTF2 sampling
  *
  * Copyright (c) 2017,
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


#include <lo2s/perf/counter/perf_counter.hpp>
#include <lo2s/perf/time/converter.hpp>
#include <lo2s/trace/trace/hpp>

#include <vector>
extern "C"
{
#include <sys/types.h>
}

namespace lo2s
{
namespace perf
{
namespace safe
{
class AbstractWriter
{
    public:
        AbstractWriter(pid_t tid, int cpuid, otf2::writer::local& writer, otf2::definition::metric_instance metric_instance, bool enable_on_exec);
        int fd()
        {
            return timer_fd;
        }
        void read();
    protected:
        std::vector<counter::PerfCounter> counters_;
        int timer_fd;
        time::Converter time_converter_;
        otf2::writer::local& writer_;
        otf2::definition::metric_instance metric_instance_;
        otf2::event::metric metric_event_;
};
}
}
}