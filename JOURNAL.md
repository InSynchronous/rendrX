# Journal
Journal to keep track of my progress.
## September 23rd, 2026
I want to make more 3D apps but don't want to become a web technology person bc I don't like JS.
There aren't many altenratives out there, I did some research and they all seem to be really dated.
I mean 3D engines like unity are great but the lag my computer so hard I can never use them.
I need something that makes it easy to render 3D objects without forcing me to worry about OpenGL.

### learning
I understand the 3D pipeline pretty good in terms of software rendering contexts, but know nothing aobut GPU rendering.

I did some research and here's the general flow:

 - Create vertexes and upload them to a VBO
 - Send them to a vertex shader, which runs for EVERY vertex. In this case it can do the perspective transformations
 - Tell the GPU to render it 
 - GPU calls fragment shader for each pixel that is "in the triangle". Return its color.
 - Fast because fragment and vertex shaders run in parallel.

### implementation???
To not waste my time with OS specific stuff I just decided to use GLFW and GLEW for GL extensions.
I'm gonna try to render a triangle now.


Just wasted so long to learn i needed 
`glewExperimental = GL_TRUE;`

Nvm that didn't work. Time to use GLAD.

### matricies
Ok so to my knowledge of 3d graphics, its not that hard to get perspective
given x, y, z, return x' and y' such that x',y' represents the 2D projection
of x,y,z

```
x' = x/z
y' = y/z
```
I used this technique a lot during software rendering, but this doesnt fly
in real graphics programming. They use matricies instead.

Matricies are calculated on the CPU, and then pushed to the GPU, where
the vertex shader takes them and uses them to move the point relative to
the cameras perspective, rotation, etc.

This wasn't hard to setup because the GLM library does all the hard math for you.

### Input
GLFW provides some input hooks I believe. I'm going to implement camera controls using them.
`glfwGetKey`

This was really easy to set up. I don't think FPS will be an issue, but if someone on a
supercomputer might travel at Mach 5 due to the hard-coded offsets.

```
glm::vec3 direction;
direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
direction.y = sin(glm::radians(pitch));
direction.z = sin(glm::radians(yaw));
```

I know its not ideal rn to use "look at", when i do have a direction matrix but who cares.

### textures
Textuers rely on UV coordinates, which are xy pairs from 0 to 1. We need to add a UV coord
to each vertex.

```
float vertices[] = {
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // xyz, ux,uy
        0.5f, -0.5f, 0.0f,    1.0f, 0.0f,
        0.0f,  0.5f, 0.0f,    0.5f, 1.0f
}
```


Since we addedd allat, we can modify our input VAO explainer. We just change increment to
5 that way it skips over UV coodinates. We also want to add another VAO just for UV coords.
I'm starting to understand it now. VBO is raw bytes, VAO is a map to the bytes for the GPU.

Next I took in the input in the vertex shader and just pass it to the fragment shader.
This means our vertex shader outputs something, in this case, uv coords.

```    
#version 330 core

in vec2 uvCoord;
out vec4 FragColor;

void main() {
    FragColor = vec4(uvCoord.x, uvCoord.y, 0.0f, 1.0f);
}
```

This should hopefully work.
I see a beautiful UV triangle. Well now i have to somehow map an image on it. Fah.

So basically the shader just uses a magic lookup func that returns a colo value,
we still gotta use opengl to like well upload the texture.

Also added something right before draw that tells opengl to bind that texture
