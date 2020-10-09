#include <function>

#include <otf2xx/otf2.hpp>

namespace lo2s
{
namespace metric
{

class Writer
{
public:
    Writer(otf2::definition::metric_class metric_class, otf2::local::writer writer, otf2::definition::system_tree_node node)
        : writer_(writer), metric_instance_(trace.metric_instance(metric_class, writer_.location(), node)), event_(otf2::chrono::genesis, metric_instance_)
    {
    }

    void write(otf2::definition::timestamp tp, std::function<void(int)> &data)
    {
        event_.timestamp(tp);

        for(std::size_t i = 0; i < event_.raw_values().size(); i++)
        {
            event_.raw_values()[i] = data(i);
        }

        writer_.write(event_);
    }



private:
    otf2::local::writer writer_;
    otf2::definition::metric_instance metric_instance_;
    otf2::definition::metric_event event_;
};

}
}
