# Canis Major
Our Games 2 final project, made in C++ with Direct3D 10 and using resources from Frank Luna's *Introduction to 3D Game Programming with Direct3D 10.0*

## Playing the Game

### Cheat codes
While in the main menu, hold down the keys D + R + B. This will put the game into Birmingham mode causing the monster to roam around passively and ignore the player.

## Development
To be able to build the project in Visual Studio 2019, the legacy [DirectX 10 SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812) is required, alongside the C++ tooling for VS 2019. 

Before installing the DirectX SDK ensure that VS is closed. If during the installation it displays an S1023 error for the C++ runtime, you will need to uninstall Microsoft Visual C++ 2010 (x86 & x64) Redistributable then run the installer again.

From here, open the project in VS, and set the mode to Release. The project should now build succesfully.

### How to setup models

Geometry.init now has 5 arguments, only the first two of which are required. Here is the new init signature:
```c++
init(ID3D10Device* device, std::string objFile, LPCWSTR texFile = L".\\textures\\grey.dds", 
bool comp = false, LPCWSTR specFile = L".\\textures\\defaultspec.dds");
```
 
the first two arguments are the D3D device and the filename of the geometry object file. To test a basic model exported without textures, all that is required is to pass in these two arguments and the model can be used in the game.

`LPCWSTR texFile` is the new way to set either a texture or a color. It is defaulted to a plain white color if you just want to init the object and run.

if you want to do a basic color you now need to have that color saved as a .dds file.
This is easily done using Paint.Net, just create a 256x256 image (it can probably be smaller, I just chose the default), fill it with your color, and save it as a .dds in the 'textures' folder. 
Then pass it in as  `L".\\textures\\yourcolor.dds"`. Make sure to have the L before the string.

`bool comp` signals whether or not the model is completed. A model is imcomplete if it was not exported with textures.
This is necessary because when no textures are exported the faces are are written as `f 1//1` instead of `f 1/1/1`,
which will screw up any untextured models. By setting it to false it compensates for the // and 
adds false texture coordinates instead. Thus an incomplete model would look like this:
```c++
mBox.init(md3dDevice, L".\\geometry\\cardboardBox.geo", L".\\textures\\cardboardcolor.dds");
```
Now, if your model is complete, IE you have a texture that correctly wraps the model and 
exported the texture coordinates with the model, then you simply pass the full 
texture and 'true' to comp, like this:
```c++
mCube.init(md3dDevice, L".\\geometry\\cube.geo", L".\\textures\\metal.dds", true);
```
If you have a specular map for the model, you can include the file location after the bool.
Leaving this empty will default to a blank specular map. Example:
```c++
mBook.init(md3dDevice, L".\\geometry\\book2.geo",L".\\textures\\book_tex.dds", true,
                       L".\\textures\\book_spec.dds");
```
### what to do if .init fails on loading
1. Check the model
 * Is the string using the right name?
 * Is the file in the correct folder?
 * Did you export the model using only triangulate faces, normals, and UV coordinates?

2. Check texture
 * Did you unpack the model from the .blend?
 * Did make sure to change the extension to .dds?

3. Complete model bool
 * If your blender model was exported with textures, make sure to set this value to true.