#!/bin/bash
FILES=(
  "xp-mspaint/global.cpp"
  "xp-mspaint/imgsuprt.cpp"
  "xp-mspaint/imgtools.cpp"
  "xp-mspaint/imgbrush.cpp"
  "xp-mspaint/imgwell.cpp"
  "xp-mspaint/rotate.cpp"
  "xp-mspaint/skew.cpp"
  "xp-mspaint/sprite.cpp"
  "xp-mspaint/tracker.cpp"
  "xp-mspaint/t_poly.cpp"
  "xp-mspaint/t_fhsel.cpp"
  "xp-mspaint/undo.cpp"
  "xp-mspaint/imgcolor.cpp"
  "xp-mspaint/colorsrc.cpp"
  "xp-mspaint/toolbox.cpp"
  "xp-mspaint/bar.cpp"
  "xp-mspaint/imgwnd.cpp"
)

for f in "${FILES[@]}"; do
  echo "=== Compiling $f ==="
  clang++ -c -std=c++17 -Icompat -Ixp-mspaint -Wno-everything "$f" -o /dev/null
  if [ $? -eq 0 ]; then
    echo "SUCCESS: $f"
  else
    echo "FAILED: $f"
    break
  fi
done
