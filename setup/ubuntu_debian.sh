#!/bin/bash
set -e
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' # no color

echo -e "${BLUE}[INFO]${NC} OOP bantai bantai SUNG JIN KATOUU Setup Script for Ubuntu/Debian"
echo -e "${YELLOW}[WARN]${NC} This will install required development packages"
read -p "Continue? [Y/n] " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]] && [[ ! $REPLY == "" ]]; then
    echo -e "${RED}[ABORT]${NC} Setup cancelled"
    exit 1
fi

echo -e "${BLUE}[INFO]${NC} Installing build essentials and tools..."
sudo apt update || { echo -e "${RED}[ERROR]${NC} Failed to update package lists"; exit 1; }
sudo apt install -y build-essential libfreetype6-dev g++ make pkg-config clang-format git cmake || { 
    echo -e "${RED}[ERROR]${NC} Failed to install build tools"; 
    exit 1; 
}
sudo apt install --install-recommends \
    mesa-utils  freeglut3-dev mesa-common-dev \
    mesa-vulkan-drivers \
    libgl1-mesa-dri \
    libgl-dev libglu1-mesa-dev  || { 
    echo -e "${RED}[ERROR]${NC} Failed to install build tools"; 
    exit 1; 
}

echo -e "${BLUE}[INFO]${NC} Installing SFML dependencies..."
sudo apt install -y libx11-dev libxi-dev libxinerama-dev libxrandr-dev libxcursor-dev libudev-dev \
    libopenal-dev libflac-dev libvorbis-dev libgl1-mesa-dev || {
    echo -e "${RED}[ERROR]${NC} Failed to install SFML dependencies"; 
    exit 1;
}

echo -e "${BLUE}[INFO]${NC} Building SFML 3.0 from source..."
SFML_DIR=/tmp/sfml-build
rm -rf $SFML_DIR && mkdir -p $SFML_DIR
git clone https://github.com/SFML/SFML.git -b 3.0.0 $SFML_DIR/SFML || {
    echo -e "${RED}[ERROR]${NC} Failed to clone SFML repository";
    exit 1;
}
cd $SFML_DIR/SFML
mkdir -p build && cd build
cmake .. \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DBUILD_SHARED_LIBS=ON \
    -DCMAKE_BUILD_TYPE=Release || {
    echo -e "${RED}[ERROR]${NC} Failed to configure SFML build";
    exit 1;
}
make -j$(nproc) || {
    echo -e "${RED}[ERROR]${NC} Failed to build SFML";
    exit 1;
}
sudo make install || {
    echo -e "${RED}[ERROR]${NC} Failed to install SFML";
    exit 1;
}
sudo ldconfig

echo -e "${BLUE}[INFO]${NC} Installing ImGui-SFML..."
WORK=/tmp/imgui-sfml-build
rm -rf $WORK && mkdir -p $WORK
git clone https://github.com/ocornut/imgui.git $WORK/imgui || {
    echo -e "${RED}[ERROR]${NC} Failed to clone ImGui repository";
    exit 1;
}
git clone https://github.com/SFML/imgui-sfml.git $WORK/imgui-sfml || {
    echo -e "${RED}[ERROR]${NC} Failed to clone ImGui-SFML repository";
    exit 1;
}
cd $WORK/imgui-sfml
git submodule update --init --recursive
mkdir -p build && cd build
cmake .. \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DBUILD_SHARED_LIBS=ON \
    -DIMGUI_DIR=$WORK/imgui \
    -DSFML_DIR=/usr/local/lib/cmake/SFML || {
    echo -e "${RED}[ERROR]${NC} Failed to configure ImGui-SFML build";
    exit 1;
}
make -j$(nproc) || {
    echo -e "${RED}[ERROR]${NC} Failed to build ImGui-SFML";
    exit 1;
}
sudo make install || {
    echo -e "${RED}[ERROR]${NC} Failed to install ImGui-SFML";
    exit 1;
}
# Clean up build directories
cd /
rm -rf $SFML_DIR $WORK

echo -e "${GREEN}[SUCCESS]${NC} Setup Complete, semangat nubes"
echo "Jalanin 'make help' buat liat bisa ngapain aja."
