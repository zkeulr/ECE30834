# README

The triangle mesh will load as soon as any of the below buttons
are pressed. Ensure the GUI window is in focus before using the keys.

## TODO

- Draw axis-aligned bounding box for the loaded triangle meshes, rather than setting the framebuffer edges.
- Create screenshot of camera visualization

## GUI

- **W**: Move forward
- **S**: Move backward
- **A**: Move left
- **D**: Move right
- **Q**: Roll left
- **E**: Roll right
- **Z**: Zoom in
- **X**: Zoom out

- **Up Arrow**: Tilt up.
- **Down Arrow**: Tilt down.
- **Left Arrow**: Pan left.
- **Right Arrow**: Pan right.

## Extra credit

- Nonlinear camera interpolation

## Resources Used

[Claude Sonnet](claude.ai), to rewrite `LoadBin()` to work on my machine, generate Doxygen comments, generate `Visualize()`,
explain nonlinear interpolation, and rewrite `SaveTiff()` with
proper headers for FFmpeg compatibility.
