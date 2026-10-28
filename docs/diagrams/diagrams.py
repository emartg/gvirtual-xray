#!/usr/bin/env python3
"""Check and render the diagrams of docs/diagrams/.

Every diagram is a pair in a subfolder: <name>.mmd (the Mermaid source, the file to edit) and <name>.png (the rendered image).

    python docs/diagrams/diagrams.py check    every .png has its .mmd and every .mmd has its .png (no tools needed; used by the CI)
    python docs/diagrams/diagrams.py render   regenerate every .png with the pinned Mermaid command-line tool (needs Node.js)

The environment variable DIAGRAMS_MMDC names an installed mmdc program to use instead of npx, and DIAGRAMS_PUPPETEER_CONFIG names
a Puppeteer configuration file (for example to choose the browser).
"""

import os
import shutil
import subprocess
import sys

MERMAID_CLI_VERSION = "11.17.0"
SCALE = "2"
BACKGROUND = "white"
PNG_SIGNATURE = b"\x89PNG\r\n\x1a\n"

ROOT = os.path.dirname(os.path.abspath(__file__))


def find_files(extension):
    """The paths, relative to the diagrams folder, of the files with the extension that are in a subfolder."""
    found = set()
    for folder, _, names in os.walk(ROOT):
        if os.path.abspath(folder) == ROOT:
            continue
        for name in names:
            if name.endswith(extension):
                path = os.path.relpath(os.path.join(folder, name), ROOT)
                found.add(path[: -len(extension)].replace(os.sep, "/"))
    return found


def check():
    sources = find_files(".mmd")
    images = find_files(".png")
    problems = []
    for name in sorted(sources - images):
        problems.append("%s.mmd has no %s.png" % (name, name))
    for name in sorted(images - sources):
        problems.append("%s.png has no %s.mmd" % (name, name))
    for name in sorted(sources & images):
        with open(os.path.join(ROOT, name + ".png"), "rb") as image:
            if image.read(len(PNG_SIGNATURE)) != PNG_SIGNATURE:
                problems.append("%s.png is not a PNG image" % name)
        if not os.path.getsize(os.path.join(ROOT, name + ".mmd")):
            problems.append("%s.mmd is empty" % name)
    for problem in problems:
        print("ERROR: " + problem)
    if problems:
        return 1
    print("OK: %d diagrams, each with its .mmd and its .png" % len(sources))
    return 0


def render():
    mmdc = os.environ.get("DIAGRAMS_MMDC")
    if mmdc:
        command = [mmdc]
    else:
        npx = shutil.which("npx")
        if not npx:
            print("ERROR: npx was not found, install Node.js or set DIAGRAMS_MMDC")
            return 1
        command = [npx, "--yes", "@mermaid-js/mermaid-cli@" + MERMAID_CLI_VERSION]
    config = os.environ.get("DIAGRAMS_PUPPETEER_CONFIG")
    sources = sorted(find_files(".mmd"))
    for name in sources:
        arguments = command + ["-i", os.path.join(ROOT, name + ".mmd"), "-o", os.path.join(ROOT, name + ".png"),
                               "-b", BACKGROUND, "-s", SCALE]
        if config:
            arguments += ["-p", config]
        print("Rendering " + name)
        if subprocess.call(arguments) != 0:
            print("ERROR: " + name + ".mmd was not rendered")
            return 1
    print("Rendered %d diagrams with Mermaid CLI %s" % (len(sources), MERMAID_CLI_VERSION))
    return check()


if __name__ == "__main__":
    if len(sys.argv) != 2 or sys.argv[1] not in ("check", "render"):
        print(__doc__)
        sys.exit(2)
    sys.exit(check() if sys.argv[1] == "check" else render())
