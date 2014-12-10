#pragma once

/*Light structures for modernGL,*/

#include "glm/glm.hpp"

/* Hopefully this works to replace(replicate) the sun */
typedef struct Directional{

	glm::vec4 lookFrom;
	glm::vec4 lookTowards;

	glm::vec4 ambientColor;

} DirectionalLight;