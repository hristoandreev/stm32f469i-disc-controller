#ifndef MODEL_HPP
#define MODEL_HPP

#include <cstdint>
#ifndef SIMULATOR
//#include <cmsis_os2.h>
#include <touchgfx/Callback.hpp>

#endif

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

//    char *getData() { return in_data; }
protected:
    ModelListener* modelListener;
//    char *in_data;

private:
//    const std::size_t in_data_size = 4096;
    bool is_http_ready;
    int tickCount;
    int scanPeriod;
    bool is_scanning;
    int progress;
#ifndef SIMULATOR
//    osMessageQueueId_t https_APsScan_response_Handle;
#endif
    touchgfx::Callback<Model, const char*> webResCompleteCallback;

    void webResCompleteCallbackHandler(const char* res);
};

#endif // MODEL_HPP
