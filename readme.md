# ZBuffer



## 介绍

一个简易的软光栅渲染器，包括模型读取、顶点着色器、zbuffer、片段着色器等功能。

- zbuffer包含传统zbuffer、扫描线zbuffer、层次zbuffer和BVH层次zbuffer。
- 片段着色器包含渲染法向、渲染深度、渲染三角形面片id、blinnphong光照模型。



## 测试平台

操作系统：Windows11 专业版。

CPU：Intel 12900HX。



## 编译运行

请在编译前进入main.cpp将：

```cpp
std::vector<std::string> filenames；
std::vector<std::string> output_filenames；
```

改为合理的相对路径或者绝对路径，否则无法读取模型或者保存渲染图片。

1. 在项目根目录下创建`build`目录并进入。
2. 执行`cmake .. -G "Visual Studio 17 2022"`。
3. 执行`cmake --build . --config Release`。
4. 在`build/Release`下找到exe文件运行。

同时可以修改main.cpp的

```cpp
Pattern type
```

来改变片段着色器的模式。



## 结果展示

展示7个模型的渲染结果，分辨率为$1280\times 1280$，面片数量分别为12、1K、4K、15K、120K、144K，1000K。

### 法向渲染

<div style="display: grid; grid-template-columns: repeat(4, 1fr); gap: 10px;">
    <div>
    <img src="assets/normal_result/cube_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/torus1k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/knob4k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/teapot15k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/spiral120k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/bunny144k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/stormveil1000k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">Naive</p>
    </div>
    <div>
    <img src="assets/normal_result/cube_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/torus1k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/knob4k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/teapot15k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/spiral120k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/bunny144k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/stormveil1000k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">Scanline</p>
    </div>
    <div>
    <img src="assets/normal_result/cube_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/torus1k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/knob4k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/teapot15k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/spiral120k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/bunny144k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/stormveil1000k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">Hierarchical</p>
    </div>
    <div>
    <img src="assets/normal_result/cube_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/torus1k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/knob4k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/teapot15k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/spiral120k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/bunny144k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/normal_result/stormveil1000k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">BVH Hierarchical</p>
    </div>
</div>


### 三角形ID渲染

<div style="display: grid; grid-template-columns: repeat(4, 1fr); gap: 10px;">
    <div>
    <img src="assets/id_result/cube_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/torus1k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/knob4k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/teapot15k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/spiral120k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/bunny144k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/stormveil1000k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">Naive</p>
    </div>
    <div>
    <img src="assets/id_result/cube_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/torus1k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/knob4k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/teapot15k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/spiral120k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/bunny144k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/stormveil1000k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">Scanline</p>
    </div>
    <div>
    <img src="assets/id_result/cube_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/torus1k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/knob4k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/teapot15k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/spiral120k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/bunny144k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/stormveil1000k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">Hierarchical</p>
    </div>
    <div>
    <img src="assets/id_result/cube_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/torus1k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/knob4k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/teapot15k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/spiral120k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/bunny144k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/id_result/stormveil1000k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">BVH Hierarchical</p>
    </div>
</div>

### 深度

<div style="display: grid; grid-template-columns: repeat(4, 1fr); gap: 10px;">
    <div>
    <img src="assets/depth_result/cube_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/torus1k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/knob4k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/teapot15k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/spiral120k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/bunny144k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/stormveil1000k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">Naive</p>
    </div>
    <div>
    <img src="assets/depth_result/cube_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/torus1k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/knob4k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/teapot15k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/spiral120k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/bunny144k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/stormveil1000k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">Scanline</p>
    </div>
    <div>
    <img src="assets/depth_result/cube_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/torus1k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/knob4k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/teapot15k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/spiral120k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/bunny144k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/stormveil1000k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">Hierarchical</p>
    </div>
    <div>
    <img src="assets/depth_result/cube_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/torus1k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/knob4k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/teapot15k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/spiral120k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/bunny144k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/depth_result/stormveil1000k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">BVH Hierarchical</p>
    </div>
</div>

### Blinn-Phong光照模型渲染

<div style="display: grid; grid-template-columns: repeat(4, 1fr); gap: 10px;">
    <div>
    <img src="assets/light_result/cube_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/torus1k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/knob4k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/teapot15k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/spiral120k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/bunny144k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/stormveil1000k_naive.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">Naive</p>
    </div>
    <div>
    <img src="assets/light_result/cube_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/torus1k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/knob4k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/teapot15k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/spiral120k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/bunny144k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/stormveil1000k_scanline.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">Scanline</p>
    </div>
    <div>
    <img src="assets/light_result/cube_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/torus1k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/knob4k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/teapot15k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/spiral120k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/bunny144k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/stormveil1000k_hierarchical.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">Hierarchical</p>
    </div>
    <div>
    <img src="assets/light_result/cube_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/torus1k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/knob4k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/teapot15k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/spiral120k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/bunny144k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <img src="assets/light_result/stormveil1000k_bvh.png" alt="Image 1" style="width: 100%; margin-bottom: 10px;">
    <p style="margin-top: 10px;">BVH Hierarchical</p>
    </div>
</div>



## 运行效率对比

所有运行时间测试均在单线程下进行，分辨率为$1280\times 1280$。

![time](./assets/result/time.png)

最快的是原始的逐像素ZBuffer，其原理是遍历每个三角形，更新其投影后的二维包围盒内的像素深度值。

在几十万面片范畴下扫描线ZBuffer和逐像素ZBuffer表现接近，略慢于逐像素ZBuffer。扫描线ZBuffer因为存储了三角形的斜率信息以及深度梯度信息而理论上快于逐像素ZBuffer，但又因为每帧需要构建分类多边形表和分类边表而额外花费时间，此外活化边表和活化多边形表的插入和删除也会花费部分时间。因此，三角形面片数量巨大时，扫描线ZBuffer需要耗费大量算力于维护表关系上，因此在1000K面片数量下表现不甚理想。

层次ZBuffer因为可以在粗粒度上拒绝整个三角形而有理论上的加速，但这通常建立在三角形遍历时是从近到远进行遍历的假设。如果三角形是从远到近遍历，那么层次ZBuffer不仅无法在粗粒度上拒绝三角形，而且会因为递归查询和深度更新而浪费额外时间。

层次包围盒与层次ZBuffer进一步得使得ZBuffer能够在粗粒度上拒绝Bounding Box内的所有三角形，但是缺点在于每帧需要为投影到NDC空间的三角面片建立BVH加速结构，这一过程无法与计算，从而产生巨大的时间开销。