from bitarray import bitarray
import numpy as np
import os
import PIL.Image
import re
import sys


# acquire inputs
local_file_names = next(os.walk(os.getcwd()))[2]
image_file_names = [x for x in local_file_names if not re.search('\.py$|\.png$', x)]

for file_name in image_file_names:
    # read input
    f = open(file_name, 'rb')
    file_contents = f.read()
    f.close()

    # convert framebuffer contents to binary
    # framebuffer is a stream of 2 byte pixel colors, either 0x0000 or 0xFFFF
    bitstream = bitarray()
    for i_byte, byte in enumerate(file_contents):
        if not i_byte % 2:  # skip every other byte
            bitstream.append(1 if byte else 0)

    # convert bitstream to image with automatic grayscale
    # framebuffer shape is (X by Y) 1680 x 1056 pixels
    img = PIL.Image.frombytes(mode='1', size=(1680, 1056), data=bytes(bitstream))

    # save image data
    img.save(file_name + '.png')
