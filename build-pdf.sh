#!/bin/bash
set -euo pipefail

# Re:VIEW Template PDF ビルドスクリプト
# Docker Compose を使用して PDF を生成します

cd "$(dirname "$0")"

echo "==> PDF ビルドを開始します..."
docker compose run --rm review

PDF_FILE="articles/ReVIEW-Template.pdf"

if [ -f "$PDF_FILE" ]; then
  echo "==> ビルド完了: $PDF_FILE"
  open "$PDF_FILE"
else
  echo "==> エラー: PDF ファイルが生成されませんでした" >&2
  exit 1
fi
