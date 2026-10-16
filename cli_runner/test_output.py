#!/usr/bin/env python3
"""Check PNG output and failure status using a caller-selected small scene.

Usage: python3 cli_runner/test_output.py /path/to/PSRayTracing SCENE_ID
All output is confined to a temporary directory; renders use 8x8, one sample,
one render thread and a maximum ray depth of three.
"""
import os
from pathlib import Path
import struct
import subprocess
import sys
import tempfile


def main():
    executable = str(Path(sys.argv[1]).resolve())
    scene = sys.argv[2]
    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        home = root / "home"
        home.mkdir()
        env = os.environ.copy()
        env.update(HOME=str(home), XDG_CONFIG_HOME=str(home), XDG_CACHE_HOME=str(home))
        command = [executable, "--scene", scene, "-s", "8x8", "-n", "1",
                   "-j", "1", "-d", "3", "--no-progress-bar"]
        result = subprocess.run(command + ["-o", "render.png"], cwd=root, env=env,
                                capture_output=True, text=True, timeout=10)
        assert result.returncode == 0, result.stderr
        image = (root / "render.png").read_bytes()
        assert image[:8] == b"\x89PNG\r\n\x1a\n"
        assert struct.unpack(">II", image[16:24]) == (8, 8)

        result = subprocess.run(command + ["-o", "missing/render.png"], cwd=root, env=env,
                                capture_output=True, text=True, timeout=10)
        assert result.returncode != 0, "Missing output directory must fail"
        assert "Failed to save render to: missing/render.png" in result.stderr
        assert not (root / "missing/render.png").exists()
    print("PNG output checks passed")


if __name__ == "__main__":
    main()
