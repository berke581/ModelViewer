#SHADER VERTEX
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	TexCoord = aTexCoord;
}

#SHADER FRAGMENT
#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex;
uniform vec4 Color;

void main()
{
	FragColor = texture(tex, TexCoord) * Color;
}
