#!/usr/bin/env python3

import sys
import binascii
from serial import Serial
import time
import argparse

parser = argparse.ArgumentParser(description='Upload dumper file to LPC21xx')
parser.add_argument('-p', '--port')
parser.add_argument('-b', '--baud', type=int)
parser.add_argument('-o', '--output')
parser.add_argument('-x', '--hexfile', default='dumper.hex', help='Dumper hex file')
parser.add_argument('--trace', action='store_true')
args = parser.parse_args()

def checksum(val):
    byte_sum = 0
    byte = 0
    i = 0
    #if the input isn't an integer number of bytes, you're doing something wrong
    if len(val) % 2 != 0:
        print ("input not of even length")
    else:
        #step through the bytes, summing them
        while i < len(val):
            byte = val[i:i+2]
            byte_val = int(byte, 16)
            byte_sum += byte_val
            i += 2	
        #limit to the least significant byte
        byte_sum = byte_sum & 0xff 
        #subtract least-significant-byte of the sum from 0x100
        byte_sum = 0x100 - byte_sum
        #print hex(byte_sum)
        return byte_sum&0xff 


lines = open(args.hexfile,"r").readlines()
data = []

linum=0
mem=[]

for n in range(len(lines)):
    line = lines[n].strip()
    #the data-length is contained in the 1st-3rd nibbles
    length=int(line[1:3],16)
    #the device address is contained in the 3rd-7th nibbles
    #this corresponds directly to the flash address of our MCU
    address=int(line[3:7],16)
    #the data type can be one of the following:
    ##00 - data record
    ##01 - end of file record
    ##02 - extended segment address record; segment base address if 2 bytes isn't enough 
    #	to address all locations
    ##03 - start segment address record; initial segment base address
    ##04 - extended linear address record; allows 4-byte addressing
    ##05 - start linear address record
    data_type=int(line[7:9],16)
    data_start = 9
    data_end = 9 + length*2
    #print("input line: "+line)
    data_hex = line[data_start:data_end]
    data = bytearray.fromhex(data_hex)
    calculated_checksum = checksum(line[1:-2])
    read_checksum = int(line[-2:],16)
    if read_checksum != calculated_checksum:
        print("checksum error, line %d, read %x, expected %x"%(n,read_checksum,calculated_checksum))
        sys.exit(1)
    if data_type == 0x04:
        addr_offset = data[0] << 24 | data[1] << 16
        print("Offset: %x" % (addr_offset))
    if data_type == 0x00:
        mem.append((addr_offset + address, data))


ser=Serial(args.port, args.baud)
def wr(string):
    if args.trace: print("to uC: "+string.strip())
    ser.write(string.encode("ascii"))
    #s=ser.readline().decode("ascii")
    #print("from uC: "+s)
    #return s
def re():
    s=ser.readline().decode("ascii").strip()
    if args.trace: print("from uC: "+s)
    return s

ser.rts=1
ser.dtr=1
time.sleep(0.1)
ser.dtr=0
time.sleep(0.1)

#handshake
wr("?\r\n")
re()
wr("Synchronized\r\n")
re()
re()

time.sleep(1)
# echo off doesn't work...
wr("A 0\r\n")
re() #receive (last) echo
re() #receive success code

#wr("B "+sys.argv[2]+" 1\r\n")
#re()
#if re()!="0": print("error setting baud rate"); sys.exit(1)

time.sleep(2)

for (addr, data) in mem:
    print("sending addr %08x" % (addr))
    u=binascii.b2a_uu(data).decode("ascii")
    write_cmd = "W %d %d\r\n" % (addr, len(data))
    wr(write_cmd)
    re() #echo

    rescode=re()
    if rescode != "0":
        print("error result %s"%rescode)
        sys.exit(1)

    cksum=sum(b for b in data)
    wr(u)
    re()
    wr("%d\r\n" % (cksum))
    re()

    #res=""
    #success=False
    #while True:
    res=re()
    
    if res == "OK":
        success=True
        #break
    else: #if res == "RESEND":
        #print("resend not implemented yet")
        print("Error result "+res)
        sys.exit(1)


print("Will run code now")
time.sleep(2)

print("Unlocking")
wr("U 23130\r\n")
re()
re()

run_addr, _ = mem[0]
print("Start at 0x%x"%run_addr)
wr("G "+str(run_addr)+" A\r\n")
re()
re()
if args.output:
    output=open(args.output,"w")
else:
    output=False
    import threading
    def myloop():
        while True:
            inp = input(" > ")
            wr(inp.strip()+"\n")
    threading.Thread(target=myloop).start()
while True:
    s=re()
    if output: output.write(s+"\r\n")
    if s=="end": sys.exit(0)





