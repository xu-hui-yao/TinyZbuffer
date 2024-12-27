#pragma once

#include <core/gbuffer.h>
#include <core/model.h>
#include <memory>

class ZBuffer {
public:
    ZBuffer(int width, int height);

    virtual ~ZBuffer() = default;

    virtual void apply(const std::shared_ptr<Model> &model,
                       const std::shared_ptr<GBuffer> &gbuffer) = 0;

protected:
    int m_width, m_height;
};
