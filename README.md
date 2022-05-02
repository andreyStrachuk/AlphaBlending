# AlphaBlending

This program proceeds the alpha blending of two images. I used SSE iunstructions to reduce the time needed for computing. I managed to get ~60% boost.
All measures are done on AMD Ryzen 7 4800H.

## How to build
```
$ git clone https://github.com/andreyStrachuk/AlphaBlending
$ cd AlphaBlending
$ make
$ ./alpha
```
## Optimizations

|        | SSE | No optimizations |
| :----- | :----- | :-----        |
|FPS| ~130| ~80|

As a result, we have 62.5% boost. As I think, it's worth using intrinsics.

