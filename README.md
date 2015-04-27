# CanisMajor

####HOW TO USE .init ON NEW MODELS


Geometry.init now has 4 fields, only the first two of which are required. Here is the new init:

    init(ID3D10Device* device, std::string objFile, LPCWSTR texFile = L".\\textures\\grey.dds", 
    bool comp = false, LPCWSTR specFile = L".\\textures\\defaultspec.dds");
 
the first two are the same. Like before, you pass it the device and the filename of the object, nothing changed here.

'LPCWSTR texFile' is the new way to set either a texture or a color. It is defaulted to a plain white color if you just want to init the object and run, this way we don't need to make any changes to old objects for this to work.

if you want to do a basic color like we had before, you now need to have that color saved as a .dds file.
This is easily done using paint .net, just create a 256x256 image(it can probably be smaller, I just default chose that), fill it with your color, and save it as a .dds in the 'textures' folder. 
Then pass it in in as  L".\\textures\\yourcolor.dds". Make sure to have the L before the string.

'bool comp' means whether or not the model is completed. If your object is an incomplete model, 
IE it doesn't have a textures exported into the .obj file, then you want to leave this as false.
The issue is that if no textures are exported then the faces are exported as 'f 1//1' instead of 'f 1/1/1',
which will screw up any untextured models. By setting it to false it compensates for the // and 
adds false texture coordinates instead. Thus an incomplete model would look like this:

    mBox.init(md3dDevice,L".\\geometry\\cardboardBox.geo", L".\\textures\\cardboard.dds");

Now, if your model is complete, you have a texture that correctly wraps the model and 
you exported the texture coordinates with the model, then you simply pass the full 
texture and 'true' to comp, like this:

    mCube.init(md3dDevice,L".\\geometry\\cube.geo", L".\\textures\\metal.dds", true);
    
If you have a specular map for the model, you can include the file location after the bool.
Leaving this empty will default to a blank specular map. Example:

    mBook.init(md3dDevice,L".\\geometry\\book2.geo",L".\\textures\\book_tex.dds", true,
                          L".\\textures\\book_spec.dds");
    
####WHAT TO DO .init FAILS ON LOADING A MODEL

1. Check the model
 * Is the string using the right name?
 * Is the file in the correct folder?
 * Did you export the model using only triangulate faces, normals, and UV coordinates?

2. Check texture
 * Did you unpack the model from the .blend?
 * Did make sure to name change the extension to .dds?

3. Complete model bool
 * If your blender model was exported with textures, make sure to set this value to true.

