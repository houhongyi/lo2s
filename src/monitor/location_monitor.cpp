/*
 * This file is part of the lo2s software.
 * Linux OTF2 sampling
 *
 * Copyright (c) 2016,
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

#include <lo2s/monitor/thread_monitor.hpp>

#include <lo2s/config.hpp>
#include <lo2s/log.hpp>
#include <lo2s/monitor/process_monitor.hpp>
#include <lo2s/perf/sample/writer.hpp>
#include <lo2s/time/time.hpp>

#include <memory>
#include <string>

#include <cassert>

extern "C"
{
#include <sched.h>
}

namespace lo2s
{
namespace monitor
{

LocationMonitor::LocationMonitor(Location location, MainMonitor& parent, bool enable_on_exec)
: PollMonitor(parent.trace(), location.name(), config().perf_read_interval), location_(location)
{
#ifdef USE_PERF_RECORD_SWITCH
    if (config().sampling)
#else
    if (config().sampling || location.type == LocationType::CPU)
#endif
    {
        sample_writer_ = std::make_unique<perf::sample::Writer>(location, parent, parent.trace(),
            enable_on_exec);
        add_fd(sample_writer_->fd());
    }

    if (!perf::counter::requested_counters().counters.empty())
    {
        counter_writer_ = std::make_unique<perf::counter::group::Writer>(location, enable_on_exec);
        add_fd(counter_writer_->fd());
    }

#ifndef USE_PERF_RECORD_SWITCH
    switch_writer_ = std::make_unique<perf::tracepoint::SwitchWriter>(location, parent.trace());
#endif
    /* setup the sampling counter(s) and start a monitoring thread */
    start();
}

void LocationMonitor::initialize_thread()
{
    try_pin_to_location();
}

void LocationMonitor::finalize_thread()
{
    if (sample_writer_)
    {
        sample_writer_->end();
    }
}

void LocationMonitor::monitor(int fd)
{
    if(location_.type = LocationType::THREAD)
    {
        try_pin_to_location();
    }

    if (sample_writer_ &&
        (fd == timer_pfd().fd || fd == stop_pfd().fd || sample_writer_->fd() == fd))
    {
        sample_writer_->read();
    }

    if (counter_writer_ &&
        (fd == timer_pfd().fd || fd == stop_pfd().fd || counter_writer_->fd() == fd))
    {
        counter_writer_->read();
    }
    if (switch_writer_ &&
            (fd == timer_pfd().fd || fd == stop_pfd.fd))
    {
        switch_writer_->read();
    }
}
} // namespace monitor
} // namespace lo2s
