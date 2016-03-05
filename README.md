# Computer Graphics

An example of usage:

Stage 1

.) Generator is a program that draws planes,boxes,spheres & cones from inputs given by users through terminal interaction.

- generator sphere 1 2 3 sphere will create a sphere with radius=1, slices=2 and stacks=3 in sphere.3d (last argument is filename)
or
- generator Sphere 1 2 3 sphere will create a sphere with radius=1, slices=2 and stacks=3 in sphere.3d (last argument is filename)
or
- Generator sphere 1 2 3 sphere will create a sphere with radius=1, slices=2 and stacks=3 in sphere.3d (last argument is filename)
or
- Generator Sphere 1 2 3 sphere will create a sphere with radius=1, slices=2 and stacks=3 in sphere.3d (last argument is filename)


-> creation of .3d files
	.generator plane 2 4 plane.3d
	.generator box 2 4 3 box.3d
	.generator sphere 1 30 30 sphere.3d
	.generator cone 1 2 30 30 cone.3d

.) Read is a program that draws the vertex contained in .3d file given as input

- read plane.3d or Read plane.3d
- read box.3d or Read box.3d
- read sphere.3d or Read sphere.3d
- read cone.3d or Read cone.3d