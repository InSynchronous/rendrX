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

## September 24, 2026
```
#include "rendrx/Scene.h"
#include "rendrx/Triangle.h"

int main() {
    rendrx::Scene scene;
    rendrx::Triangle tri = {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f},
                            {0.5f, -0.5f, 0.0f},  {1.0f, 0.0f},
                            {0.0f, 0.5f, 0.0f},   {0.5f, 1.0f}};
    scene.addTriangle(tri);
    scene.launch();

    while (!scene.shouldClose()) {
        scene.render();
    }

    return 0;
}
```

It's refactor day! I came up with my "ideal usage" and then started creating files.
After like 17 minutes of work I got all the header files done. I reused the
Vector3/Vector2 classes from previous projects for simplicity.

51 minutes in, I have a Scene.cpp class and I've porrted most of the code over copy paste. Now
all that's left is the render loop. I changed pitch and yaw for camera rotation to use glm:vec3
Wait. GLM has a vector3, why did i make a vector3 class myself. Smh. Ok well thats for a later 
refactor.

Ngl. the build system failed to compile my vector3 class, its probably an easy patch, but im lazy.
Lets just switch to GLM ig. Well I can't avoid it, turns out i configured CMAKE wrong. Had to add
CONFIGURE_DEPENDS:

```file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)
```

Alright. I just got it to compile, and nothing showed up on my screen. Great. Debug time.
Turns out the mistake was forgetting the minus sign in `-90.0f` yaw. Waste of time.

### cubes n stuff
I wrote up the coodinates for a cube made of 12 triangles, two per face by hand, and then rendered.
As expected, it created a really weird look, likely because the order of rendering is cooked.
This is probably because I'm rendering triangles that are behind the front of the block last,
which probably screws everything up. I'm gonna do some more researcch on this topic.


### Backface culling
So basically things behind you are a different rotation in terms of A B and C than things
infornt of you. Its hard to explain without images but the math works.
```
glEnable(GL_CULL_FACE);
glCullFace(GL_BACK);
glFrontFace(GL_CCW);
```

And since I from the start wrote the coorrdinates in the correct order (prob cuz i stole
them from a prev project). It worked perfectly. 
