#!/bin/bash

TARGET_DIR="/usr/local/cflora"
SOURCE_DIR=".."

rm -rf $TARGET_DIR

mkdir $TARGET_DIR
mkdir "$TARGET_DIR/bin"
mkdir "$TARGET_DIR/conf"
mkdir "$TARGET_DIR/db"

cp "$SOURCE_DIR/install/cflora-run.sh" "$TARGET_DIR/bin/"
chmod +x "$TARGET_DIR/bin/cflora-run.sh"
cp "$SOURCE_DIR/install/cflora-stop.sh" "$TARGET_DIR/bin/"
chmod +x "$TARGET_DIR/bin/cflora-stop.sh"
cp "$SOURCE_DIR/install/cflora-restart.sh" "$TARGET_DIR/bin/"
chmod +x "$TARGET_DIR/bin/cflora-restart.sh"
cp "$SOURCE_DIR/install/cflora" "$TARGET_DIR/bin/"
chmod +x "$TARGET_DIR/bin/cflora"
cp "$SOURCE_DIR/install/cflora-mon.sh" "$TARGET_DIR/bin/"
chmod +x "$TARGET_DIR/bin/cflora-mon.sh"

update-rc.d -f cflora remove
ln -s "$TARGET_DIR/bin/cflora" /etc/init.d/cflora
update-rc.d cflora defaults

cp "$SOURCE_DIR/gos/bin/gos" "$TARGET_DIR/bin/"
cp "$SOURCE_DIR/gcg/bin/gcg" "$TARGET_DIR/bin/"
cp "$SOURCE_DIR/node/bin/node" "$TARGET_DIR/bin/"

cp "$SOURCE_DIR/conf/cflora-server.ini" "$TARGET_DIR/conf/"
cp "$SOURCE_DIR/conf/cflora-device.ini" "$TARGET_DIR/conf/"



