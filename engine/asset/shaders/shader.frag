# version 450
# extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
// 纹理坐标系
layout(location = 1) in vec2 fragTexCoord;

// 2维图像纹理，使用unifrom传递
layout(binding = 1) uniform sampler2D texSampler;

// 这里表示我们需要用outColor覆盖掉接收的fragColor作为输出传递出去，fragColor后续就不用了，当然你不想覆盖也可以，location = 1即可，fragColor还是会保留
layout(location = 0) out vec4 outColor;

void main() {
	// 使用纹理和纹理坐标系完成贴图
	outColor = texture(texSampler, fragTexCoord);
	// 测试寻址模式, 让坐标系超出[0, 1]的范围即可
	// outColor = texture(texSampler, fragTexCoord * 2.0);
	// 测试颜色和纹理叠加
	// outColor = vec4(fragColor * texture(texSampler, fragTexCoord * 2.0).rgb, 1.0);
}