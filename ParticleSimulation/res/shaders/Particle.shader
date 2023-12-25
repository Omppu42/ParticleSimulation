#shader vertex
#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in float alpha;

out vec4 FragColor;

void main() {
   gl_Position = vec4(pos, 0.0, 1.0);
   FragColor = vec4(1.0, 1.0, 1.0, alpha);
}

#shader fragment
#version 330 core

in vec4 FragColor;

out vec4 FinalColor;

void main() {
	FinalColor = FragColor;
}