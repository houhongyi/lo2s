#include <lo2s/time/time.hpp>

#include <otf2xx/definition/metric_class.hpp>
#include <otf2xx/local/writer.hpp>
#include <otf2xx/definition/metric_instance.hpp>
#include <otf2xx/event/metric_event.hpp>

namespace lo2s
{
namespace perf
{
namespace counter
{
class MetricWriter
{
public:
    MetricWriter(otf2::local::writer& writer,const otf2::definition::metric_class& metric_class, const otf2::definition::location& scope)
        : time_converter_(time::Converter::instance())
        writer_(writer),metric_instance_(trace::Trace::instance().metric_instance(metric_class, writer.location(), scope), metric_event_(otf2::chrono::genesis(), metric_instance_)
protected:
    time::Converter time_converter_;
    otf2::writer::local& writer_;
    otf2::definition::metric_instance metric_instance_;
    otf2::event::metric metric_event_;
};
}
}
}
