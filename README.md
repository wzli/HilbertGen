# Hilbert Curve Bitstream Generator

This is a small and highly efficient algorithm written in portable C to generate [Hilbert Curves](https://en.wikipedia.org/wiki/Hilbert_curve). It was originally used for adaptive local thresholding within a real-time embedded vision application. As such, performance and resource constrains are the primary considerations.
1) This is an iterative algorithm, as opposed recursive methods that are unsuitable for systems with tightly bounded memory.
2) Consecutive increasing orders of the curve can be generated in a continuous sequential stream without needing to discard or modify previously generated segments.
3) Directional bit representation allow 16 simultaneous rotate/flip operations using a single 32-bit XOR instruction.
4) Compact encoding allows generated curves to be stored and reused repeatedly. 


*Note*:
The intended use case is for traversing the entire curve as fast as possible. Eg. Scanning through every pixel in an image. There are other more efficient algorithms for translating between sparse coordinates.  


## Directional Bit Encoding
Although functions to convert to XY coordinates are provided, understanding the encoding format is very simple:
```C
// every two bits represent movement from previous pixel to next pixel
// in one of four cardinal directions
> = 0b00
^ = 0b01
< = 0b10
v = 0b11

// The following holds true for each direction:
// reflection across y = x maps to inverting first bit
// reflection across y =-x maps to inverting both bits

```
### For example:

![alt text](http://mathworld.wolfram.com/images/eps-gif/HilbertCurve_700.gif)

With (x = 0, y = 0) starting from bottom-left, the first two curves are encoded as **(read from right to left)**:
```
// 1st order
// v > ^
0b110001

// 2nd order
// > v < v v > ^ > v > ^ ^ < ^ >
0b001110111100010011000101100100
```


## Demo App

Todo . . .
