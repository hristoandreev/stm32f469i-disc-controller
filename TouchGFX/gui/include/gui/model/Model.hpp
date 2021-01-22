#ifndef MODEL_HPP
#define MODEL_HPP

#include <touchgfx/Callback.hpp>
#include <cstdint>

#define MS_TO_TICK(ms) (static_cast<int>(ms / 16)) // Tick period = 16ms.

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

protected:
    ModelListener* modelListener;

private:
    bool is_http_ready;
    int tickCount;
    int scanPeriod;
    bool is_scanning;
    int progress;

    touchgfx::Callback<Model, const char*> webResCompleteCallback;

    void webResCompleteCallbackHandler(const char* res);
};

#endif // MODEL_HPP
