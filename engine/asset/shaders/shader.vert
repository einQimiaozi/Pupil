# version 450
# extension GL_ARB_separate_shader_objects : enable

// binding������������ר�õĲ��ַ��ţ�һ������������ȫ�����㶼Ӧ�õľ����������
layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

// location���ڰ�framebuffer�е��±�ͱ���֮��Ĺ�ϵ���Դ��ڶ����Ƭ����ɫ֮�䴫�����ݣ�����opengl�е�attribute
// out��in��ʾ���������������Ҫ��������Ǵӱ����ɫ��������ܵ���
// �ر���ʾ: vecx ��������������32λ�������ͣ�dvecx��64λ��32λ���ݲ��ֺ��location + 1, 64λ���ݲ��ֺ��location + 2
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	// �ر���ʾ: vulkan�е�����ϵ��y���opengl�Ƿ���......
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
	fragColor = inColor;
	fragTexCoord = inTexCoord;
}