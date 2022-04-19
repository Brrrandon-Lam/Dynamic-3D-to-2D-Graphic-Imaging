This set of instructions is up to date as of 19 April 2022.

At the time, there are six API calls that can be made:

TransformPosX
void TransformPosX(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents)

TransformPosY
void TransformPosY(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents)

TransformPosZ
void TransformPosZ(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents)

TransformNegX
void TransformNegX(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents)

TransformNegY
void TransformNegY(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents)

TransformNegZ
void TransformNegZ(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents)

These six functions will be rewritten into a single function call. However, for now each of the function calls does as the name suggests.
It takes an array of vertex data for a three dimensional object and then transforms it into a two dimensional object to the best of its ability.
At the moment, this simply means that it iterates through all of the object's position coordinates and removes any duplicates with respect to
native axes, +-X, +-Y, and +-Z.

The first argument is a string, which is passed into the function and lets the program know which function is being called. This
argument is intended for a later update where you pass a string "+X, "+Y", "+Z", "-X", "-Y", "-Z" into an overhead API call and it
makes a call to one of these six functions in turn for user convenience. 

The second argument, axis_align takes in a boolean value. When set to true, the program will attempt to flatten the vertex data.
The function will make subroutine calls to find the minimum or maximum vertex coordinate in the X, Y or Z plane depending on which is called, and
transform the vertex data's respective components to that dimension. For example, if you wanted to flatten something in the positive X direction,
the function would search for the vertex coordinate furthest in the positive X direction, and after performing the 2 dimensional transformation, push
all vertices to be in-line with that maximum positive X value.

The third argument is the vertex data. This simply takes in the vertex data in the form of a GLfloat array. At the moment, the program is only compatible
with C++ as the underlying function uses vectors to process the data and make modifications to the vertex data that is passed in.

The fourth argument is verticesSize, which takes in the number of vertices that are present in the vertex data.

The fifth argument, numComponents, is important because it lets the program know how many additional components there are to each set of vertex data.
At the time, the program does not support inconsistent vertex data size, meaning that if one piece of data has position, rgb, texture coordinates,
ambient/diffuse/specular values and surface normals, all pieces of vertex data must contain it.
