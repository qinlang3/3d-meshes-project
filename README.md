# Computer Graphics – Meshes


### `src/write_obj.cpp`

Write a pure-triangle or pure-quad mesh with 3D vertex positions `V` and faces
`F`, 2D parametrization positions `UV` and faces `UF`, 3D normal vectors `NV`
and faces `NF` to a `.obj` file.

### `src/cube.cpp`

Construct the quad mesh of a cube including parameterization and per-face
normals.

![Running `./obj` will first pop up a viewer with your generated cube textured
with a [Rubik's
cube](https://en.wikipedia.org/wiki/Rubik%27s_Cube)](images/rubiks-cube.gif)

### `src/sphere.cpp`


Construct a quad mesh of a sphere with `num_faces_u` × `num_faces_v` faces.

![After closing that window, another viewer will pop up with your generated sphere textured
with the earth.](images/earth.gif)

### `src/triangle_area_normal.cpp`
Compute the normal vector of a 3D triangle given its corner locations. The
output vector should have length equal to the area of the triangle.

### `src/per_face_normals.cpp`
Compute per-face normals for a triangle mesh.

### `src/per_vertex_normals.cpp`
Compute per-vertex normals for a triangle mesh.

### `src/vertex_triangle_adjacency.cpp`
Compute a vertex-triangle adjacency list. For each vertex store a list of all
incident faces.

### `src/per_corner_normals.cpp`
Compute per corner normals for a triangle mesh by computing the area-weighted
average of normals at incident faces whose normals deviate less than the
provided threshold.




![`./normals` should open a viewing window. Toggling `1`,`2`,`3` should switch
between normal types. Notice that per-face has sharp corners, but a faceted
appearance in the curved regions; the per-vertex has nice smooth regions but
ugly corners where averaging acts up; and per-corner is the best of both
worlds.](images/fandisk-normals.png)







### Subdivision Surfaces

A [subdivision surface](https://en.wikipedia.org/wiki/Subdivision_surface) is a
natural generalization of a [spline
curve](https://en.wikipedia.org/wiki/Spline_(mathematics)). A smooth spline can
be defined as the [limit](https://en.wikipedia.org/wiki/Limit_(mathematics)) of
a [recursive
process](https://en.wikipedia.org/wiki/Recursion_(computer_science)) applied to
a polygon: each edge of the polygon is split with a new vertex and the vertices
are smoothed toward eachother. If you've drawn smooth curves using Adobe
Illustrator, PowerPoint or Inkscape, then you've used splines.

At a high-level, subdivision surfaces work the same way. We start with a
polyhedral mesh and subdivide each face. This adds new vertices on the faces
and/or edges of the mesh. Then we smooth vertices toward each other.

The first and still (most) popular subdivision scheme was invented by
[Catmull](https://en.wikipedia.org/wiki/Edwin_Catmull) (who went on to co-found
[Pixar](https://en.wikipedia.org/wiki/Pixar)) and
[Clark](https://en.wikipedia.org/wiki/James_H._Clark) (founder of
[Silicon Graphics](https://en.wikipedia.org/wiki/Silicon_Graphics) and
[Netscape](https://en.wikipedia.org/wiki/Netscape)). [Catmull-Clark
subdivision](https://en.wikipedia.org/wiki/Catmull–Clark_subdivision_surface) is
defined for inputs meshes with arbitrary polygonal faces (triangles, quads,
pentagons, etc.) but always produces a pure-quad mesh as output (i.e., all faces
have 4 sides).



![Running `./quad_subdivision` and repeated pressing _space_ will show this
[Bob](http://www.cs.cmu.edu/~kmcrane/Projects/ModelRepository/#bob) converging
toward a smooth surface.](images/bob-subdivision.gif)



 










### `src/catmull_clark.cpp`

Conduct `num_iters` iterations of [Catmull-Clark
subdivision](https://en.wikipedia.org/wiki/Catmull–Clark_subdivision_surface) on
a **pure quad** mesh (`V`,`F`).

