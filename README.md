# lpc2103-dumper
Memory dumper for LPC21xx chips with unlocked boot loader. Dumps the contents of the internal flash memory via the In System Programming Interface by uploading a stub (dumper.c / dumper.hex) to the chip and running it. 

To use, just connect the UART port of the LPC21xx to a USB UART adapter.

Enter the In System Programming mode as described in the manual ([UM10114](https://www.nxp.com/docs/en/user-guide/UM10114.pdf) chapter 21.5):

> A LOW level after reset at the P0.14 pin is considered as an external hardware request to start the ISP command handler.

Then run the `upload.py` script from this repository.

```
usage: python3 upload.py [-h] [-p PORT] [-b BAUD] [-o OUTPUT] [-x HEXFILE] [--trace]

Upload dumper file to LPC21xx

optional arguments:
  -h, --help            show this help message and exit
  -p PORT, --port PORT
  -b BAUD, --baud BAUD
  -o OUTPUT, --output OUTPUT
  -x HEXFILE, --hexfile HEXFILE
                        Dumper hex file
  --trace
```

The OUTPUT file will contain a iHEX dump with probably some trash data surrounding it.

