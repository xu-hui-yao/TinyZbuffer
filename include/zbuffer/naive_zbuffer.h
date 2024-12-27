#pragma once

#include <zbuffer/zbuffer.h>

class NaiveZBuffer : public ZBuffer {
public:
    NaiveZBuffer(int width, int height);

    ~NaiveZBuffer() override;

    void apply(const std::shared_ptr<Model> &model, const std::shared_ptr<GBuffer> &gbuffer) override;
};
