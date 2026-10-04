#!/bin/bash
# Se ejecuta una sola vez al crear el contenedor: instala agbcc en el repo y genera la primera ROM.
set -e

cd "$(dirname "$0")/.."
REPO="$(pwd)"

echo "==> Instalando agbcc en tools/agbcc"
(cd /opt/agbcc && ./install.sh "$REPO")

echo "==> Generando la primera ROM (la primera vez tarda unos minutos)"
make -j"$(nproc)"
cp pokefirered.gba room.gba

echo ""
echo "✅ Todo listo. ROM generada: room.gba"
echo "   Para volver a generarla tras cambiar un texto: Ctrl+Shift+B"
