#
# Makefile
#   ROM image for MN1613 Board w/ CELESTE
#

#DEBUG_OPT = -D USE_HALT
#DEBUG_OPT = -D USE_LOOP
DEBUG_OPT =

all:	rom_high.bin rom_low.bin rom.tar.gz

rom.tar.gz: celeste.inc dev_celeste.asm rom.asm rom.lst rom_high.bin rom_low.bin Makefile
	tar zcvf rom.tar.gz celeste.inc dev_celeste.asm rom.asm rom.lst rom_high.bin rom_low.bin Makefile   

rom_high.bin: rom.p
	p2bin -m even rom.p rom_high.bin

rom_low.bin: rom.p
	p2bin -m odd rom.p rom_low.bin

rom.p:	rom.asm celeste.inc dev_celeste.asm
	asl -L $(DEBUG_OPT) rom.asm -olist rom.lst -o rom.p

#config.inc:
#	[ -e config.inc ] || ln -s config/config.inc config.inc
#
clean:
	rm -f *.p *.hex *.sr *.lst *.bin *.tar.gz
