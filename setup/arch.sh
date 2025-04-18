#!/bin/bash
set -e

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

echo -e "${BLUE}[INFO]${NC} DITT TOLONG DITT AJARIN PWN / Setup Script for Arch (btw)"
echo -e "${YELLOW}[WARN]${NC} This will install required development packages"
read -p "Continue? [Y/n] " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]] && [[ ! $REPLY == "" ]]; then
    echo -e "${RED}[ABORT]${NC} Setup cancelled"
    exit 1
fi

echo -e "${BLUE}[INFO]${NC} Updating system packages..."
sudo pacman -Syu --noconfirm || { 
    echo -e "${RED}[ERROR]${NC} Failed to update system packages"; 
    exit 1; 
}

echo -e "${BLUE}[INFO]${NC} Installing build tools and dependencies..."
sudo pacman -S --noconfirm base-devel gcc clang git cmake pkgconf || {
    echo -e "${RED}[ERROR]${NC} Failed to install build tools";
    exit 1;
}

echo -e "${BLUE}[INFO]${NC} Installing SFML and graphics dependencies..."
sudo pacman -S --noconfirm sfml mesa xorg-server-devel || {
    echo -e "${RED}[ERROR]${NC} Failed to install SFML dependencies";
    exit 1;
}

echo -e "${BLUE}[INFO]${NC} Installing ImGui-SFML..."
# — prepare a workspace for both ImGui and ImGui‑SFML —
WORK=/tmp/imgui-sfml-build
rm -rf $WORK && mkdir -p $WORK

# 1) clone Dear ImGui itself
git clone https://github.com/ocornut/imgui.git $WORK/imgui

# 2) clone the SFML wrapper
git clone https://github.com/SFML/imgui-sfml.git $WORK/imgui-sfml
cd $WORK/imgui-sfml
git submodule update --init --recursive  # still bring in backends etc.

# 3) configure & build against shared SFML + our imgui
mkdir build && cd build
cmake .. \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DBUILD_SHARED_LIBS=ON \
    -DIMGUI_DIR=$WORK/imgui
make -j$(nproc)
sudo make install

cd /
rm -rf $IMGUI_DIR

echo -e "${GREEN}[SUCCESS]${NC} Setup complete, saatnya bantai bantai"
echo "Jalanin 'make help' buat liat bisa ngapain aja."
