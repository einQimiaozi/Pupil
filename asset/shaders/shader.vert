# version 450
# extension GL_ARB_separate_shader_objects : enable

// binding是描述符布局专用的布局符号，一般用于描述对全部顶点都应用的矩阵等描述符
layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

// location用于绑定framebuffer中的下标和变量之间的关系，以此在顶点和片段着色之间传递数据，类似opengl中的attribute
// out和in表示这个变量是我们需要输出，还是从别的着色器那里接受到的
// 特别提示: vecx 这种数据类型是32位数据类型，dvecx是64位，32位数据布局后的location + 1, 64位数据布局后的location + 2
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	// 特别提示: vulkan中的坐标系的y轴和opengl是反的......
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
	fragColor = inColor;
	fragTexCoord = inTexCoord;
}