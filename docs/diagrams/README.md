# Diagrams

Each diagram is kept twice, side by side, in a subfolder named after its topic:

```
docs/diagrams/<subfolder>/<name>.mmd   the Mermaid source (the file to edit)
docs/diagrams/<subfolder>/<name>.png   the rendered image (generated from the source)
```

The images are regenerated and reviewed whenever a release is about to come out, so that every `.png` matches its `.mmd`.
The version of the Mermaid command-line tool and the command that renders the images will be written here with the first diagrams.

Planned set (none is drawn yet): the computation of an image as a sequence (framebuffers, L-buffer, path lengths, energy fluence);
the classes of the core library (`XRayRenderer`, `PolygonMesh`, `XRayDetector`, `XRayBeam`, `Context`, `ResourceManager`, `VBO`);
the flow of data from the scene to the detector; the superbuild and its dependencies; the jobs of the continuous integration;
the release flow; and the path that removes Git LFS.
