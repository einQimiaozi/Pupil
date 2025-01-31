#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace Pupil {
	REFLECTION_TYPE(Vertex)
	STRUCT(Vertex, WhiteListFields) {
		REFLECTION_BODY(Vertex)

		glm::vec3 pos;
		glm::vec4 color;
		glm::vec2 texCoord;
		glm::vec3 normal;

		META(Enable)
		float alpha;

		bool operator == (const Vertex& other) const {
			return pos == other.pos && color == other.color && texCoord == other.texCoord && normal == other.normal;
		}
	};

	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};
}

namespace std {
	template<> struct hash<Pupil::Vertex> {
		size_t operator()(Pupil::Vertex const& vertex) const {
			size_t pos_hash = hash<glm::vec3>{}(vertex.pos);
			size_t color_hash = hash<glm::vec4>{}(vertex.color);
			size_t texCoord_hash = hash<glm::vec2>{}(vertex.texCoord);
			size_t normal_hash = hash<glm::vec3>{}(vertex.normal);

			return pos_hash ^ (color_hash << 1) ^ ((texCoord_hash << 1) >> 1) ^ (normal_hash >> 1);
		}
	};
}