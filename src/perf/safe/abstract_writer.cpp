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

#include <lo2s/perf/safe/abstract_writer.hpp>
#include <lo2s/perf/event_collection.hpp>

extern "C"
{
#include <sys/timerfd.h>
}
namespace lo2s
{
namespace perf
{
namespace counter
{
AbstractWriter::AbstractWriter(pid_t tid, int cpuid, otf2::writer::local& writer,
        otf2::definition::metric_instance metric_instance)
    :time_converter_(time::Converter::instance()), writer_(writer), metric_instance_(metric_instance),
        metric_event_(otf2::chrono::genesis(), metric_instance)
{
    std::chrono::nanoseconds = read_interval = config().safe_read_interval;
    struct itimerspec tspec;
    memset(&tspec, 0, sizeof(struct itimerspec));

    tspec.it_value.tv_nsec = 1;
    tspec.it_interval.tv_sec =std::chrono::duration_cast<std::chrono::seconds>(read_interval).count();

    tspec.it_interval.tv_nsec = (read_interval % std::chrono::seconds(1)).count();

    timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    timerfd_settime(timer_fd, TFD_TIMER_ABSTIME, &tspec, NULL);

    for(const auto& counter : requested_safe_events())
    {
        counters_.emplace_back(tid, cpuid, counter);
    }
}

void AbstractWriter::read()
{
    metric_event_.timestamp(time_converter_(time::now()));

    otf2::event::metric::values& values = metric_event_.raw_values();

    assert(counters_.size() <= values.size());
    for (std::size_t i = 0; i < counters_.size(); i++)
    {
        values[i] = counters_[i].read();
    }

    auto index = counters_.size();
    values[index++] = counters_.enabled();
    values[index++] = counters_.running();

    writer_.write(metric_event_);

    uint64_t expirations;
                 uint64_t expirations;
             if (read(timer_fd, &expirations,                     sizeof(expirations)) == -1)
             {
                 Log::error() << "Flushing timer fd failed";
                 throw_errno();
             }
             (void)expirations;
}
}
}
}
}
