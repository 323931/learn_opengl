Mesh        // 顶点/索引/VAO/EBO 信息
Material    // shader + 材质参数
Renderable // mesh + material + modelMatrix
Light       // position/color/attenuation
Renderer    // 负责 draw
Scene       // 持有 renderables 和 light
