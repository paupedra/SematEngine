# SematEngine

## Description
Semat Engine is a small Game Engine developed in C++ by one student in Bachelor's degree in Video Game Design and Development in CITM Barcelona.

[Bachelor's degree in Video Game Design and Development](<https://www.citm.upc.edu/ing/estudis/graus-videojocs/>)

[CITM] (<https://www.citm.upc.edu/>)

[Tech Talent Center](<https://www.talent.upc.edu/cat/school/ttc/>)

## Members

- Pau Pedra - GitHub: [@paupedra](https://github.com/paupedra)

## How to use

### Controls:

#### General:

- Escape: Exit App.

#### Camera:

- WASD: FPS-like controls.

- Shift: Move camera faster.

- Left Click: Rotates camera without moving.

- Alt + Left Click + move mouse: Orbit arround selected Game Object. (This feature has been lost for this release)

- F: Focus on the selected Game Object. (This feature has been lost for this release)

- Mouse Wheel: Zoom In / Out. (This feature has been lost for this release)

#### Windows:

- Console: Shows LOG and general console output of the engine.

- Hierarchy: Shows all game objects inside the scene with their respective children. User can perform the following action:

Create Empty: Using right click a pop-up will appear letting the user create an empty child as a child of the currently selected Game Object.

Delete: Using right click a pop-up will appear letting the user delete the selected game object as well as all of it's children.

Reparent: User can drag game objects in the scene and set them as child of another game object. This cannot be done with dragged object's children.

- Inspector: Shows the components and their values of the selected game object.

- Configuration: Allows user to change various modules variables as well as to see crucial information about the engine performance and hardware.

- Assets: Allows user to see the folders inside Assets of this project. When selecting a folder to the right there will appear the loaded resources inside it.

- Resources: Displays resources in library and the resources loaded in memory and the amount of references they have. User can press a scene file to have it loaded on scene. References will react accordingly as well as when the game objects containing them are deleted.

- Play: Allows the user to start and stop "Game" mode activating game time.

#### Main Menu Bar:

- File: Option to Exit App. Save option serializes the current scene inside "Library/Scenes/SerializedCurrentScene.scene".

- Windows: Allows user to open / close engine windows.

- Primitives: Allows users to create primitives shapes as game objects.

- Help: Open About Window showing more info about the engine and License.

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