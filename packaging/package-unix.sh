#!/usr/bin/env bash
set -euo pipefail

system="$1"
version="$2"
artifacts="build/JunkyNoize_artefacts/Release"
package_root="build/package"
stage="$package_root/JunkyNoize-${version}-${system}"
mkdir -p "$stage"

if [[ "$system" == "macOS" ]]; then
  mkdir -p "$stage/VST3" "$stage/AU"
  cp -R "$artifacts/VST3/Junky Noize.vst3" "$stage/VST3/"
  cp -R "$artifacts/AU/Junky Noize.component" "$stage/AU/"
  archive="$package_root/JunkyNoize-${version}-macOS-universal.zip"
else
  mkdir -p "$stage/VST3"
  cp -R "$artifacts/VST3/Junky Noize.vst3" "$stage/VST3/"
  archive="$package_root/JunkyNoize-${version}-Linux-x64.tar.gz"
fi

cat > "$stage/INSTALL.txt" <<EOF
Junky Noize ${version} - ${system}

VST3 installation:
  macOS: Copy VST3/Junky Noize.vst3 to ~/Library/Audio/Plug-Ins/VST3/
         Optional Audio Unit: copy AU/Junky Noize.component to ~/Library/Audio/Plug-Ins/Components/
  Linux:  Copy VST3/Junky Noize.vst3 to ~/.vst3/

Create the destination folder if needed, then rescan plugins in your DAW.
EOF

if [[ "$system" == "macOS" ]]; then
  ditto -c -k --sequesterRsrc --keepParent "$stage" "$archive"
else
  tar -czf "$archive" -C "$package_root" "JunkyNoize-${version}-${system}"
fi
