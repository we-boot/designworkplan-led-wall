#!/usr/bin/env python

# (This is an example similar to an example from the Adafruit fork
#  to show the similarities. Most important difference currently is, that
#  this library wants RGB mode.)
#
# A more complex RGBMatrix example works with the Python Imaging Library,
# demonstrating a few graphics primitives and image loading.
# Note that PIL graphics do not have an immediate effect on the display --
# image is drawn into a separate buffer, which is then copied to the matrix
# using the SetImage() function (see examples below).
# Requires rgbmatrix.so present in the same directory.

# PIL Image module (create or load images) is explained here:
# http://effbot.org/imagingbook/image.htm
# PIL ImageDraw module (draw shapes to images) explained here:
# http://effbot.org/imagingbook/imagedraw.htm

from PIL import Image, ImageEnhance, ImageDraw
import time
import io
import numpy as np
from picamera import PiCamera
from picamera.array import PiRGBArray
from rgbmatrix import RGBMatrix, RGBMatrixOptions

ENHANCE_CONTRAST = 1.8
MATRIX_WIDTH = 32
MATRIX_HEIGHT = 32

# Configuration for the matrix
options = RGBMatrixOptions()
options.rows = 32
options.chain_length = 1
options.parallel = 1
options.hardware_mapping = 'adafruit-hat' # If you have an Adafruit HAT: 'adafruit-hat'

matrix = RGBMatrix(options=options)

# with PiCamera(resolution=(32,32)) as camera:
#     with PiRGBArray(camera) as stream:
#         for frame in camera.capture_continuous(stream, format="rgb", use_video_port=True):
#             image = Image.frombytes("RGB", (32,32), frame.array, "raw")
#             matrix.SetImage(image, 0, 0)
#             # Reset stream to start at position 0 again
#             stream.truncate(0)

camera = PiCamera(resolution=(MATRIX_WIDTH, MATRIX_HEIGHT))
print("create buffer")
rawCapture = np.empty(MATRIX_WIDTH * MATRIX_HEIGHT * 3, dtype=np.uint8)
while True:  
    print("capture")
    camera.capture(rawCapture, format="rgb", use_video_port=True)
    print("open")
    image = Image.frombytes("RGB", (MATRIX_WIDTH, MATRIX_HEIGHT), rawCapture, "raw")

    print("enhance")
    image_enhancer = ImageEnhance.Contrast(image)
    enhanced_image = image_enhancer.enhance(ENHANCE_CONTRAST)
    print("display")
    matrix.SetImage(enhanced_image, 0, 0)

matrix.Clear()
