#include <core/bitmap.h>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#include <stb_image_write.h>
#ifndef TINYEXR_IMPLEMENTATION
#define TINYEXR_IMPLEMENTATION
#define TINYEXR_USE_MINIZ 0
#define TINYEXR_USE_STB_ZLIB 1
#undef max
#undef min
#endif
#include <iostream>
#include <tinyexr.h>

Bitmap::Bitmap(int height, int width) : m_rows(height), m_cols(width) {
    m_data.resize(width * height);
    std::fill(m_data.begin(), m_data.end(), float3(0.0f, 0.0f, 0.0f));
}

void Bitmap::set_data(std::vector<float3> data) { m_data = std::move(data); }

int Bitmap::index(int row, int col) const { return row * m_cols + col; }

void Bitmap::save_exr(const std::string &filename) const {
    // Convert the TensorXf data to a float array suitable for EXR
    int width  = m_cols;
    int height = m_rows;

    EXRHeader header;
    InitEXRHeader(&header);

    EXRImage image;
    InitEXRImage(&image);

    image.num_channels = 3;

    std::vector<float> images[3];
    images[0].resize(width * height);
    images[1].resize(width * height);
    images[2].resize(width * height);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            images[0][i * width + j] = m_data[index(i, j)].x;
            images[1][i * width + j] = m_data[index(i, j)].y;
            images[2][i * width + j] = m_data[index(i, j)].z;
        }
    }

    float *image_ptr[3];
    image_ptr[0] = &images[2].at(0); // B
    image_ptr[1] = &images[1].at(0); // G
    image_ptr[2] = &images[0].at(0); // R

    image.images        = reinterpret_cast<unsigned char **>(image_ptr);
    image.width         = width;
    image.height        = height;
    header.num_channels = 3;
    header.channels     = static_cast<EXRChannelInfo *>(
        malloc(sizeof(EXRChannelInfo) * header.num_channels));
    // Must be (A)BGR order, since most of EXR viewers expect this channel
    // order.
    strncpy_s(header.channels[0].name, "B", 255);
    header.channels[0].name[strlen("B")] = '\0';
    strncpy_s(header.channels[1].name, "G", 255);
    header.channels[1].name[strlen("G")] = '\0';
    strncpy_s(header.channels[2].name, "R", 255);
    header.channels[2].name[strlen("R")] = '\0';

    header.pixel_types =
        static_cast<int *>(malloc(sizeof(int) * header.num_channels));
    header.requested_pixel_types =
        static_cast<int *>(malloc(sizeof(int) * header.num_channels));
    for (int i = 0; i < header.num_channels; i++) {
        header.pixel_types[i] =
            TINYEXR_PIXELTYPE_FLOAT; // pixel type of input image
        header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF;
        // pixel type of output image to be stored in .EXR
    }

    const char *err = nullptr;
    int ret = SaveEXRImageToFile(&image, &header, filename.c_str(), &err);
    if (ret != TINYEXR_SUCCESS) {
        std::cerr << "Save EXR err: " << err << std::endl;
        FreeEXRErrorMessage(err); // free's buffer for an error message
        return;
    }
    free(header.channels);
    free(header.pixel_types);
    free(header.requested_pixel_types);
}

void Bitmap::save_png(const std::string &filename) const {
    int width  = m_cols;
    int height = m_rows;

    // Convert the float data to 8-bit (0-255) data for saving as PNG
    std::vector<unsigned char> img_data(width * height * 3);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float3 color;
            color.x   = m_data[index(y, x)].x;
            color.y   = m_data[index(y, x)].y;
            color.z   = m_data[index(y, x)].z;
            auto srgb = color.to_srgb();
            for (int c = 0; c < 3; ++c) {
                // Convert to 8-bit by clamping to 0-255 range
                int index  = ((height - 1 - y) * width + x) * 3 + c;
                auto value = static_cast<unsigned char>(
                    clamp(srgb[c] * 255.0f, 0.0f, 255.0f));
                img_data[index] = value;
            }
        }
    }

    // Save as PNG
    int ret = stbi_write_png(filename.c_str(), width, height, 3,
                             img_data.data(), width * 3);
    if (ret == 0) {
        throw std::runtime_error("Failed to save PNG file: " + filename);
    }
}
