One extra texture-related feature:
- Add the ability to use a specular map in the shader, controlling per-pixel shininess
	- use red channel to read the greyscale from specular map
	- use the value to scale the result in the specular calculation in the shaderhelper.hlsli