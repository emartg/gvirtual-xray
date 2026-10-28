# Diagrams

Each diagram is kept twice, side by side, in a subfolder named after its topic:

```
docs/diagrams/<subfolder>/<name>.mmd   the Mermaid source (the file to edit)
docs/diagrams/<subfolder>/<name>.png   the rendered image (generated from the source)
```

The images are regenerated and reviewed whenever a release is about to come out, so that every `.png` matches its `.mmd`.

## Index

| Diagram | Source | Image | What it shows |
|---|---|---|---|
| The computation of an X-ray image | [sequence/compute_image.mmd](sequence/compute_image.mmd) | [sequence/compute_image.png](sequence/compute_image.png) | The calls of `XRayRenderer::initialise()` and `computeImage()` in order: the shader programs, the five framebuffer objects, the L-buffer (path length through each group of meshes), the sum of mu x dx per energy channel, and the energy fluence. It gives the measured cost of each part (Release and Debug, GTX 1660 SUPER, 256x128 pixels, 4 meshes), what is read live at every call, and what is sized at `initialise()` |
| The classes of the core library | [uml/core_classes.mmd](uml/core_classes.mmd) | [uml/core_classes.png](uml/core_classes.png) | `XRayRenderer`, `XRayDetector`, `XRayBeam`, `SceneGraphNode`, `PolygonMesh` (and its subclasses), `VBO`, `Shader`, `FramebufferObject`, `Context`, and `ResourceManager`, with who owns whom and who only points to whom |
| The jobs of the continuous integration | [ci/jobs.mmd](ci/jobs.mmd) | [ci/jobs.png](ci/jobs.png) | The workflows `ci.yml` (Linux, Windows MinGW, and the consumer test with two CMake versions) and `diagrams.yml`, their steps, the cache of the dependencies (key, who restores it, who saves it), and the measured times |
| The release flow | [release/release_flow.mmd](release/release_flow.mmd) | [release/release_flow.png](release/release_flow.png) | From the release pull request to the tag, the GitHub release, and the pin of the simulator, and the path that removes Git LFS at `v0.2.0` (decided, not built yet) |

Not drawn yet: the flow of data from the scene to the detector and the superbuild and its dependencies.

## Rendering

The images are made by the Mermaid command-line tool, **`@mermaid-js/mermaid-cli` version 11.17.0** (Mermaid 11.17), which needs Node.js and a Chromium-based browser (the tool downloads one the first time it runs). One command renders every diagram:

```
python docs/diagrams/diagrams.py render
```

It runs this command for each `.mmd` (the version is written in the script, `MERMAID_CLI_VERSION`):

```
npx --yes @mermaid-js/mermaid-cli@11.17.0 -i docs/diagrams/<subfolder>/<name>.mmd -o docs/diagrams/<subfolder>/<name>.png -b white -s 2
```

The environment variable `DIAGRAMS_PUPPETEER_CONFIG` names a Puppeteer configuration file (to choose another browser or to pass flags), and `DIAGRAMS_MMDC` names an installed `mmdc` program to use instead of `npx`. The images are not reproducible to the byte: the text depends on the fonts of the browser, and two renders of the same source can place a box a few pixels apart. Commit the images that the review of a release approved, and keep the old image of a diagram whose source did not change.

## Checking

```
python docs/diagrams/diagrams.py check
```

stops with an error when a `.png` has no `.mmd` (a source was deleted or renamed), when a `.mmd` has no `.png` (the image was not rendered), when an image is not a PNG file, or when a source is empty. It needs only Python and runs in the workflow `diagrams.yml` on every pull request and push to `main`, in a few seconds. It does not check that an image is up to date with its source: that is what rendering and reviewing before a release is for.

## Adding a diagram

1. Write `docs/diagrams/<subfolder>/<name>.mmd` (names in lowercase with underscores) from the code, not from memory, and keep the line of the index above.
2. Render it with `python docs/diagrams/diagrams.py render` and look at the image.
3. Commit the `.mmd` and the `.png` together.
