#!/bin/bash
set -euo pipefail

# Install wasixcc to /tmp/wasixcc and then install executables to /usr/local/bin

echo "Installing wasixcc..."

# Ensure sed and curl are available
if ! command -v curl >/dev/null 2>&1; then
  echo "curl not found, attempting to install..."
  brew install curl
fi

if ! command -v sed >/dev/null 2>&1; then
  echo "sed not found, attempting to install..."
  brew install gnu-sed
fi

# Detect platform
OS=$(uname -s | tr '[:upper:]' '[:lower:]')
ARCH=$(uname -m)

# Normalize OS
case "$OS" in
linux*) OS="linux";;
darwin*) OS="apple";;
mingw*|msys*|cygwin*|windows*) OS="windows";;
*) echo "Unsupported OS: $OS"; exit 1;;
esac

# Normalize architecture
case "$ARCH" in
x86_64|amd64) ARCH="x86_64";;
aarch64|arm64) ARCH="aarch64";;
*) echo "Unsupported architecture: $ARCH"; exit 1;;
esac

# Construct target triple
if [ "$OS" = "linux" ]; then
  if [ -f /lib/ld-musl-x86_64.so.1 ] || [ -f /lib/ld-musl-aarch64.so.1 ] || (ldd --version 2>&1 | grep -qi musl); then
    TARGET="${ARCH}-unknown-linux-musl"
  else
    TARGET="${ARCH}-unknown-linux-gnu"
  fi
elif [ "$OS" = "apple" ]; then
  TARGET="${ARCH}-apple-darwin"
elif [ "$OS" = "windows" ]; then
  echo "Warning: Windows builds are not yet available. Please use WSL or a Linux/macOS environment."
  exit 1
fi

echo "Detected platform: $TARGET"

# Resolve wasixcc version (default to latest)
VERSION="latest"
if [ "$VERSION" = "latest" ]; then
  VERSION=$(curl -s https://api.github.com/repos/wasix-org/wasixcc/releases/latest | grep '"tag_name"' | sed -E 's/.*"([^"]+)".*/\1/')
  if [ -z "$VERSION" ]; then
    echo "Failed to fetch latest version"
    exit 1
  fi
fi
echo "Using version: $VERSION"

# Download wasixcc binary
DOWNLOAD_URL="https://github.com/wasix-org/wasixcc/releases/download/${VERSION}/wasixcc-${TARGET}.tar.gz"
curl -L -o wasixcc.tar.gz "$DOWNLOAD_URL"

if [ ! -f wasixcc.tar.gz ]; then
  echo "Failed to download wasixcc"
  exit 1
fi

# Install wasixcc
tar -xzf wasixcc.tar.gz
rm wasixcc.tar.gz

# Clean /tmp/wasixcc if exists
if [ -d /tmp/wasixcc ]; then
  rm -rf /tmp/wasixcc
fi

mv wasixcc /tmp/wasixcc
chmod +x /tmp/wasixcc

# Install executables to /usr/local/bin
sudo /tmp/wasixcc --install-executables /usr/local/bin

echo "wasixcc version: $(wasixcc --version)"
echo "wasixcc executables installed to /usr/local/bin"