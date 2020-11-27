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

#pragma once

#include <filesystem>

#include <chrono>
#include <fstream>
#include <mutex>
#include <string>
#include <unordered_map>

#include <cstdint>
#include <ctime>

extern "C"
{
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>
}

#include <fmt/core.h>

namespace lo2s
{

template <typename T>
class StringCache
{
public:
    static StringCache& instance()
    {
        static StringCache l;
        return l;
    }

    T& operator[](const std::string& name)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        if (elements_.count(name) == 0)
        {
            elements_.emplace(std::piecewise_construct, std::forward_as_tuple(name),
                              std::forward_as_tuple(name));
        }
        return elements_.at(name);
    }

private:
    std::unordered_map<std::string, T> elements_;
    std::mutex mutex_;
};

 enum class LocationType
 {
     THREAD,
     CPU,
     SAMPLE_CPU,
     UNKOWN
 };

 struct Location
 {
     LocationType type;
     int location;

     Location()
         : type(LocationType::UNKOWN), location(-1)
     {
     }

     Location(LocationType t, int l)
         : type(t), location(l)
     {
     }

     std::string name() const
     {
         switch(type)
         {
             case LocationType::THREAD:
                 return fmt::format("thread {}", location);
             case LocationType::CPU:
                 return fmt::format("cpu {}", location);
             case LocationType::SAMPLE_CPU:
                 return fmt::format("sample cpu {}", location);
             default:
                 throw new std::runtime_error("Unknown LocationType!");
         }
     }

     pid_t tid() const
     {
         return (type == LocationType::THREAD ? location : -1);
     }
     int cpuid() const
     {
         return (type != LocationType::THREAD ? location : -1);
     }

     //Needed becausse this is used as a Key in some Structures.
     //Simply order (arbitrarly) by type first, then by location
     friend bool operator<(const Location& lhs, const Location& rhs)
     {
         if(lhs.type != rhs.type)
         {
             return lhs.type < rhs.type;
         }
         else
         {
             return lhs.location < rhs.location;
         }
     }

     friend bool operator==(const Location& lhs, const
             Location& rhs)
     {
         return lhs.type == rhs.type && lhs.location == rhs.location;
     }
 };

 struct CpuLocation : public Location
 {
     CpuLocation(int cpuid)
         : Location(LocationType::CPU, cpuid)
     {
     }
 };

 struct ThreadLocation : public Location
 {
     ThreadLocation(pid_t tid)
         : Location(LocationType::THREAD, tid)
     {
     }
 };

 struct SampleCpuLocation : public Location
 {
     SampleCpuLocation(int cpuid)
         : Location(LocationType::SAMPLE_CPU, cpuid)
     {
     }
 };
                  

std::size_t get_page_size();
std::string get_process_exe(pid_t pid);
std::string get_process_comm(pid_t pid);
std::string get_task_comm(pid_t pid, pid_t task);

std::chrono::duration<double> get_cpu_time();
std::string get_datetime();

const struct ::utsname& get_uname();

template <typename T>
T get_sysctl(const std::string& group, const std::string& name)
{
    auto sysctl_path = std::filesystem::path("/proc/sys") / group / name;
    std::ifstream sysctl_stream;
    sysctl_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    sysctl_stream.open(sysctl_path);

    T result;
    sysctl_stream >> result;
    return result;
}

int32_t get_task_last_cpu_id(std::istream& proc_stat);

std::unordered_map<pid_t, std::string> get_comms_for_running_processes();

void try_pin_to_location(Location location);

pid_t gettid();
} // namespace lo2s
