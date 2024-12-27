#include <core/bitmap.h>
#include <core/bvh.h>
#include <core/model.h>
#include <core/timer.h>
#include <fragment_shader/fragment_shader.h>
#include <vertex_shader/vertex_shader.h>
#include <zbuffer/bvh_hierarchical_zbuffer.h>
#include <zbuffer/hierarchical_zbuffer.h>
#include <zbuffer/naive_zbuffer.h>
#include <zbuffer/scanline_zbuffer.h>

void render(const std::shared_ptr<VertexShader> &vertex_shader, const std::shared_ptr<FragmentShader> &fragment_shader,
            const std::shared_ptr<Model> &model, const std::shared_ptr<GBuffer> &gbuffer,
            const std::shared_ptr<ZBuffer> &zbuffer) {
    // Load
    Timer timer;

    // Vertex shader
    vertex_shader->apply(model);
    std::cout << "Vertex shader took " << timer.lap_string() << "\n";

    // Rasterize and zbuffer
    zbuffer->apply(model, gbuffer);
    std::cout << "zbuffer took " << timer.lap_string() << "\n";

    // Fragment shader
    fragment_shader->apply(gbuffer);
    std::cout << "Fragment shader took " << timer.lap_string() << "\n";
}

void object_test() {
    // Set parameters
    int height   = 1280;
    int width    = 1280;
    Pattern type = EDepth;
    float3 camera_origin(6.0f, 3.0f, 6.0f);
    float3 camera_target(0.0f, 0.0f, 0.0f);
    float3 up(0.0f, 1.0f, 0.0f);
    float fov           = 30.0f;
    matrix4 view_matrix = matrix4::look_at(camera_origin, camera_target, up);
    matrix4 perspective_matrix =
        matrix4::perspective(fov, static_cast<float>(width) / static_cast<float>(height), 0.1f, 20.0f);
    matrix4 screen_matrix = matrix4::scale(static_cast<float>(width), static_cast<float>(height), 1.0f);

    std::vector<std::string> filenames{
        "../assets/cube.obj",      "../assets/torus1k.obj",    "../assets/knob4k.obj",
        "../assets/teapot15k.obj", "../assets/spiral120k.obj", "../assets/bunny144k.obj"
    };
    std::vector<std::string> output_filenames{
        "../assets/depth_result/cube.png",       "../assets/depth_result/torus1k.png",
        "../assets/depth_result/knob4k.png",     "../assets/depth_result/teapot15k.png",
        "../assets/depth_result/spiral120k.png", "../assets/depth_result/bunny144k.png"
    };

    auto vertex_shader = std::make_shared<VertexShader>(view_matrix, perspective_matrix, screen_matrix, width, height);
    auto fragment_shader = std::make_shared<FragmentShader>(type, vertex_shader->get_transform_matrix(),
                                                            (camera_target - camera_origin).normalize());
    fragment_shader->set_blinn_phong_params(float3(0, 10, 0), float3(1.0f, 1.0f, 1.0f), float3(0.2f, 0.2f, 0.2f));

    int start_index = 0;
    int end_index   = 6;
    for (int i = start_index; i < end_index; i++) {

        matrix4 model_matrix;
        switch (i) {
            case 0:
                model_matrix = matrix4::translate(0.25, -0.1, 0);
                break;
            case 1:
                model_matrix = matrix4::translate(0.25, -1.5, 0);
                break;
            case 2:
                model_matrix = matrix4::translate(0.25, -0.2, 0);
                break;
            case 3:
                model_matrix = matrix4::translate(0.25, -1, 0);
                break;
            case 4:
                model_matrix = matrix4::translate(0.25, -0.2, 0);
                break;
            case 5:
                model_matrix = matrix4::translate(0.25, -1, 0);
                break;
            default:
                model_matrix = matrix4::identity();
                break;
        }

        for (int j = 0; j < 4; j++) {
            auto model   = std::make_shared<Model>(filenames[i], model_matrix);
            auto bitmap  = std::make_shared<Bitmap>(height, width);
            auto gbuffer = std::make_shared<GBuffer>(height, width);
            std::string posix;
            std::shared_ptr<ZBuffer> zbuffer;
            switch (j) {
                case 0:
                    zbuffer = std::make_shared<NaiveZBuffer>(width, height);
                    posix   = "_naive";
                    break;
                case 1:
                    zbuffer = std::make_shared<ScanlineZBuffer>(width, height);
                    posix   = "_scanline";
                    break;
                case 2:
                    zbuffer = std::make_shared<HierarchicalZBuffer>(width, height);
                    posix   = "_hierarchical";
                    break;
                case 3:
                    zbuffer = std::make_shared<BVHHierarchicalZBuffer>(width, height);
                    posix   = "_bvh";
                    break;
                default:
                    std::cerr << "Unknown zbuffer type!\n";
                    return;
            }

            std::cout << "\nStart rendering " << filenames[i] << " with" << posix << " zbuffer" << std::endl;

            render(vertex_shader, fragment_shader, model, gbuffer, zbuffer);

            // Save result
            bitmap->set_data(gbuffer->m_color_buffer);
            std::string output_filename = output_filenames[i];
            output_filename             = output_filename.substr(0, output_filename.size() - 4)
                                  .append(posix)
                                  .append(output_filename.substr(output_filename.size() - 4));
            bitmap->save_png(output_filename);
        }
    }
}

void scene_test() {
    // Set parameters
    int height   = 1280;
    int width    = 1280;
    Pattern type = EBlinnPhong;
    float3 camera_origin(400.0f, 200.0f, 400.0f);
    float3 camera_target(0.0f, 0.0f, 0.0f);
    float3 up(0.0f, 1.0f, 0.0f);
    float fov           = 40.0f;
    matrix4 model_matrix = matrix4::identity();
    matrix4 view_matrix = matrix4::look_at(camera_origin, camera_target, up);
    matrix4 perspective_matrix =
        matrix4::perspective(fov, static_cast<float>(width) / static_cast<float>(height), 1.0f, 10000.0f);
    matrix4 screen_matrix = matrix4::scale(static_cast<float>(width), static_cast<float>(height), 1.0f);

    std::vector<std::string> filenames{
        "../assets/stormveil1000k.obj"
    };
    std::vector<std::string> output_filenames{
        "../assets/light_result/stormveil1000k.png"
    };

    auto vertex_shader = std::make_shared<VertexShader>(view_matrix, perspective_matrix, screen_matrix, width, height);
    auto fragment_shader = std::make_shared<FragmentShader>(type, vertex_shader->get_transform_matrix(),
                                                            (camera_target - camera_origin).normalize());
    fragment_shader->set_blinn_phong_params(float3(0, 1000, 0), float3(1.0f, 1.0f, 1.0f), float3(0.2f, 0.2f, 0.2f));

    int start_index = 0;
    int end_index   = 1;
    for (int i = start_index; i < end_index; i++) {
        for (int j = 0; j < 4; j++) {
            auto model   = std::make_shared<Model>(filenames[i], model_matrix);
            auto bitmap  = std::make_shared<Bitmap>(height, width);
            auto gbuffer = std::make_shared<GBuffer>(height, width);
            std::string posix;
            std::shared_ptr<ZBuffer> zbuffer;
            switch (j) {
                case 0:
                    zbuffer = std::make_shared<NaiveZBuffer>(width, height);
                    posix   = "_naive";
                    break;
                case 1:
                    zbuffer = std::make_shared<ScanlineZBuffer>(width, height);
                    posix   = "_scanline";
                    break;
                case 2:
                    zbuffer = std::make_shared<HierarchicalZBuffer>(width, height);
                    posix   = "_hierarchical";
                    break;
                case 3:
                    zbuffer = std::make_shared<BVHHierarchicalZBuffer>(width, height);
                    posix   = "_bvh";
                    break;
                default:
                    std::cerr << "Unknown zbuffer type!\n";
                    return;
            }

            std::cout << "\nStart rendering " << filenames[i] << " with" << posix << " zbuffer" << std::endl;

            render(vertex_shader, fragment_shader, model, gbuffer, zbuffer);

            // Save result
            bitmap->set_data(gbuffer->m_color_buffer);
            std::string output_filename = output_filenames[i];
            output_filename             = output_filename.substr(0, output_filename.size() - 4)
                                  .append(posix)
                                  .append(output_filename.substr(output_filename.size() - 4));
            bitmap->save_png(output_filename);
        }
    }
}

int main() {
    scene_test();

    return 0;
}