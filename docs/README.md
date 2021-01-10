# SematEngine

## Description
Semat Engine is a small Game Engine developed in C++ by one student in Bachelor's degree in Video Game Design and Development in CITM Barcelona.

[Bachelor's degree in Video Game Design and Development](<https://www.citm.upc.edu/ing/estudis/graus-videojocs/>)

[CITM](<https://www.citm.upc.edu/>)

[Tech Talent Center](<https://www.talent.upc.edu/cat/school/ttc/>)

## Members

- Pau Pedra 

<img src="https://github.com/paupedra/SematEngine/blob/master/docs/Resources/Images/my_photo.jpeg?raw=true" alt= "Pau Pedra" height="150">

- GitHub: [@paupedra](https://github.com/paupedra)
- LinkedIn: [Pau Pedra Bonifacio](https://www.linkedin.com/in/pau-pedra-bonifacio/)

## Core Sub-Systems

### Game Objects

Game objects are the different, well, Objects that live inside a scene. They have different Components that add different functionality to the object:

- Transform: Holds the position, rotation and scale of the game objects. User can modify all these parameters. Transform automatically computes the global position of objects that are in tree tructure.

- Mesh: Holds the information of the mesh and calls the draw of the mesh to the Renderer system.

- Material: Holds the information of the image that will be applied as a texture to the Mesh Component. Also the color of a mesh if no texture is drawn.

- Camera: This is the point of view of the game mode. The camera takes care of the culling and the Field of view (FOV).

- Animator: Allows the user to interact with the different animations that will be loaded from the fbx. The user can cut them into clips and play them.

### Editor

This refers to the general UI that allows the user to manipulate the scene to be able to add, delete or modify game object and various configurations in the engine. The most notable parts are:

- Hierarchy window, that shows all the game objects inside the current scene in a tree-like fashion. The user can reparent, select, add and delete game objects through this window. 

- The inspector window shows the components that the currently selected object has. User can modify and add different components inside it.

- Game window is a graphical representation of the current scene, showing all the game objects with their meshes and textures drawn on them. User can select a game object by clicking on them.

### Resourcwe manager

The resource manager is in charge of managing the memory usage and resources loading in the engine. To be able to save as much memory as posible we use reference counting to be able to know how many time our game objects and components are using the resources loaded in memory. 

The resource manager will only load a resource into memory when it is used in the current scene, if not it will be saved in our different custom file format to be loaded when it is needed.

It is also in charge of importing every single file the user desires to use such as .fbx and images.

## Animation 



## Features

- Resource Manager capable of loading .fbx files and have meshes stored in memory only when used using reference counting. Menus showing currently loaded resources and menu that can navigate imported files.

- Custom File Format: There are custom file formats for all the resources. .scene, .material, .mesh and .tex.

- Serialized Current Scene: Using the save button user can create a serialized Json file of the current scene. It cannot be loaded yet.

- Game Object Hierarchy and transformation: Game Objects' transforms can be modified through inspector window, this includes translation, scale and otation. Gae objects's hierarchy is imported and loaded properly.

- Street Environment is imported and loaded at the start of the engine.

- Camera can be added as a component in a game Object and modify it's planes and the FOV. User can swap to another camera using a button inside the inspector.

- Objects are culled successfully using frustum culling.

- Objects can be picked with the mouse and the selected object is highlighted.

## Additional Features



## License

MIT License

Copyright (c) 2020 [missing.md]

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.