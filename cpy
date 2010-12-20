#!/bin/sh
cp /work/adm-my/bin/nandloader.img /media/Flash/Bloader/nrom.img
cp /work/adm-my/bin/nandloader_ram.img /media/Flash/Bloader/nram.img
sudo umount /media/Flash
cp /work/adm-my/bin/nandloader.img /media/tftpboot/b
