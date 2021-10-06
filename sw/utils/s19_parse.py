#!/usr/bin/env python3
import sys

def s19_parse(filename):
    s19_file = open(filename, 'r')
    for line in s19_file:
        rec_field = line[:2]
        prefix    = line[:4]

        if rec_field == "S0" or prefix == "S009" or prefix == "S505" or prefix == "S705" or prefix == "S017" or prefix == "S804" or line == "":
            continue
        data = line[12:20] # extract data byte
        str_addr = line[4:12]

        bswap_data = "{}{}{}{}".format(data[6:8], data[4:6], data[2:4], data[0:2])

        print("{}_{}".format(str_addr, bswap_data))

    s19_file.close()


###############################################################################
# Start of file
###############################################################################
if(len(sys.argv) < 2):
    print("Usage s19_parse.py FILENAME > OUT_FILENAME")
    quit()

s19_parse(sys.argv[1])
