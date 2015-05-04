cmd_/home/pi/hw/modules.ko := ld -EL -r  -T ./scripts/module-common.lds --build-id  -o /home/pi/hw/modules.ko /home/pi/hw/modules.o /home/pi/hw/modules.mod.o
