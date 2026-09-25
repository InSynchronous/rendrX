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

```cpp
glm::vec3 direction;
direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
direction.y = sin(glm::radians(pitch));
direction.z = sin(glm::radians(yaw));
```

I know its not ideal rn to use "look at", when i do have a direction matrix but who cares.

### textures
Textuers rely on UV coordinates, which are xy pairs from 0 to 1. We need to add a UV coord
to each vertex.

```cpp
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

```glsl
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
```cpp
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

```cmake
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
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
```cpp
glEnable(GL_CULL_FACE);
glCullFace(GL_BACK);
glFrontFace(GL_CCW);
```

And since I from the start wrote the coorrdinates in the correct order (prob cuz i stole
them from a prev project). It worked perfectly. 

### More primatives
Right now I only support triangles, but that's going to become an issue. The solution 
is to make an abstract class/interface that has a comprerssion featuer that turns it
into a vector of floats in the p1p2p3uxuv format.

I'll begin.


It took a little longer than usual for me since I forgot basically everything about 
abstraction in C++ after writing Java for a month, so I had to rewatch some videos.
Eventually, I got it to work with the new Object system.

I'll make a QUAD primative now. It's pretty simple.

Quad primative simplyy takes in 4 points and generates 2 triangles when it flattens.
This project lwk is evolving into a minecraft clone ngl.....

## September 25th, 2026
I want to get a couple things working to keep this engine good for the future. First,
I want to get multiple textures loading. Currently, the top of the cube is the same
texture as the sides. Same with its bottom. I need to figure out how to switch textures
for different objects.

### textures
I read the code that hanldes texutres and its pretty simple.
Basically the main part thats important is this:

```cpp
glUseProgram(shaderProgram);
int textureLocation = glGetUniformLocation(shaderProgram, "texture1");
```
We pull out the location in the shader of where the input "texture1" is.

Then, we say 
```cpp
glUniform1i(textureLocation, 0);
```

This basically tells opengl to bind that texture location to 0. 
Recall in our draw loop:
```cpp
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, texture);
```

Our shader is configured to read from 0, and we set 0 to the texture.

Originally, my thought was to just change what GL_TEXTURE0 means by swapping
the texture int it poitns to. But this is really slow, also would break my
refactoring of the draw loop taking in flattened triangles only.

Turns out, theres a simple solution, the texture atlas.

I basically imported a large minecraft atlas, 512x512, and now,
instead of setting uv coords to like 0 to 1, i can do like 0 to 1/(row len)

I hard coded the UV tiles real quick, just to see if it works.

And voila, a nether wart cube. Well let's try to find a different more useful
block.

```cpp
constexpr float u0 = 0.0f;
constexpr float v0 = 0.0f;
constexpr float u1 = 32.0f / 512.0f;
constexpr float v1 = 32.0f / 512.0f;
```


All I have to do is tinker with these values till I get something useful
```cpp
constexpr float x = 0;
constexpr float y = 0;
constexpr float u0 = x * 32.0f / 512.0f;
constexpr float v0 = y * 32.0f / 512.0f;
constexpr float u1 = (x + 1) * 32.0f / 512.0f;
constexpr float v1 = (y + 1) * 32.0f / 512.0f;
```

Using this neat formula, I can cycle through blocks. It was looking really
weird though. I just realized. It's because the image is 512x256 not 512x512.

I changed the formulas to reflect. It looked like it was showing me 4 blocks.
I just divided everything by 2 and it worked. Smh. 

Anddd it didn't. Turns out textures are 10x10 not 16x16 or 8x8. I hate this.

Ykw this sucks. I'm switching to a 256x256 atlas. It works now.

### period 3 at school time
I decided to wrap this sketchy atlas code into a helper.


This took way too long, since I added a new constructor for Quad that just
takes in 3 points and a pair of two UV coordinates for the bottom left and right
aka the same format that the atlas helper returns.

### next steps for the project
Lwk realized making minecraft might be a easier, and more finishable project.
Voxel engines are pretty simple. You have a Chunk class, that holds blocks, 
then you loop through each block in a chunk, checking its neighbors, to then
selectively decide what quads to add to the object list, then render.

I don't really know anything about updating objects in the VBO yet.
Let's just focus on getting the block system to work for now.

I created the block class and I'm going to see if I can use its abstractions.

### blocks
After a traditional segfault (i tried to move a pointer I already moved),
I got it to work.

```cpp
auto uv = rendrx::getUV(3, 15);
    Block block({0, 0, 0}, uv); // block at origin

    scene.addObject(block.getFront());
    scene.addObject(block.getBack());
    scene.addObject(block.getLeft());
    scene.addObject(block.getRight());
    scene.addObject(block.getTop());
    scene.addObject(block.getBottom());
```

Although the current way I'm doing uv is not the way I want to do it,
for now this is fine.
