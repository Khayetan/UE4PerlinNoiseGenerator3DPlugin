# UE4PerlinNoiseGenerator3DPlugin
An Unreal Engine 4 plugin that adds a 3d Perlin noise generator object. Written based on this article: https://adrianb.io/2014/08/09/perlinnoise.html
Exposed to Blueprint.

How to use:
* Construct the PerlinNoiseGenerator3D object.
* Call SetSeed with the desired seed.
* Call Perlin3D to generate values.

*Can be used to generate continents on a sphere or Minecraft-like caves:*
![Screenshot 2021-06-04 195110](https://user-images.githubusercontent.com/11838635/120846716-d1b25100-c572-11eb-876d-c07ad59d3db3.png)
![Screenshot 2021-06-04 202349](https://user-images.githubusercontent.com/11838635/120846722-d414ab00-c572-11eb-9def-09dce79e596a.png)
