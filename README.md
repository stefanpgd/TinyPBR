# TinyPBR - A small & simple PBR rasterizer 
## This is a small renderer that is able to load in a (single) chosen model and uses a PBR pipeline to present it. 

### There are a few scenes to pick from:
- **Sphere | Simple Color**: presents a sphere with some properties the user can edit like 'Color' or 'Metallic'
- **Model | Simple Color**: the user can select a model, then manipulate it's shading with some properties.
- **Model | Load Texture(s)**: the user can select a model and all bound (PBR) textures to that model will be loaded in and used.
- **Model | Select Texture(s)**: the user can select a model and then select textures per (PBR) texture type. Use this when you've textures that could be used on any sort of model (e.g. ground or rock textures ).

### Why did I make this?
I made this project since I was learning about Physically Based Rendering for the first time. So I wanted a fresh environment with just a straightforward way of using it so I could test my PBR implementation and see if it worked. Afterwards once it was working I added some extra UI so I could play around with it more, basically making it a toy renderer. The project overall took around 10-20 hours of work. Mind that the code quality is quite low, this project wasn't made to be a robust or performant but to test out the things I just learned/researched :).

____

### API/Libraries/Tools used for the project:
- Graphics API: OpenGL (4.6)
- GLFW
- GLAD
- ImGui
- GLM
- Assimp 
- stb_image
____
## Most recent screenshot:
<img src="https://raw.githubusercontent.com/stefanpgd/TinyPBR/main/Resources/Screenshots/githubScreenshot.png" height=480px>
