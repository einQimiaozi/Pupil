# version 450
# extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
// ��������ϵ
layout(location = 1) in vec2 fragTexCoord;

// 2άͼ������ʹ��unifrom����
layout(binding = 1) uniform sampler2D texSampler;

// �����ʾ������Ҫ��outColor���ǵ����յ�fragColor��Ϊ������ݳ�ȥ��fragColor�����Ͳ����ˣ���Ȼ�㲻�븲��Ҳ���ԣ�location = 1���ɣ�fragColor���ǻᱣ��
layout(location = 0) out vec4 outColor;

void main() {
	// ʹ���������������ϵ�����ͼ
	outColor = texture(texSampler, fragTexCoord);
	// ����Ѱַģʽ, ������ϵ����[0, 1]�ķ�Χ����
	// outColor = texture(texSampler, fragTexCoord * 2.0);
	// ������ɫ���������
	// outColor = vec4(fragColor * texture(texSampler, fragTexCoord * 2.0).rgb, 1.0);
}