#include <lo2s/time/time.hpp>

#include <otf2xx/otf2.hpp>

namespace lo2s
{
namespace perf
{
namespace counter
{
class MetricWriter
{
public:
    MetricWriter(Location location, const otf2::definition::metric_class& metric_class)
        : time_converter_(time::Converter::instance()),
        writer_(trace::Trace::instance().metric_writer(location)),metric_instance_(trace::Trace::instance().metric_instance(metric_class, writer_.location(), trace::Trace::instance().location(location))), metric_event_(otf2::chrono::genesis(), metric_instance_)
                {}
protected:
    time::Converter time_converter_;
    otf2::writer::local& writer_;
    otf2::definition::metric_instance metric_instance_;
    otf2::event::metric metric_event_;
};
} // namespace counter
} // namespace perf
} // namespace lo2s
