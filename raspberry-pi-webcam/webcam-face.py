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

from PIL import Image
from PIL import ImageDraw
import cv2
import time
import io
import numpy as np
import cv2, sys, numpy, os
import numpy as np
from picamera import PiCamera
from picamera.array import PiRGBArray
from rgbmatrix import RGBMatrix, RGBMatrixOptions

WIDTH = 800
HEIGHT = 608

# Configuration for the matrix
options = RGBMatrixOptions()
options.rows = 32
options.chain_length = 1
options.parallel = 1
options.hardware_mapping = 'adafruit-hat' # If you have an Adafruit HAT: 'adafruit-hat'

matrix = RGBMatrix(options=options)

print("open haar")
face_cascade = cv2.CascadeClassifier("haar.xml")
print("open webcam")
webcam = PiCamera(resolution=(WIDTH, HEIGHT))
 
# The program loops until it has 30 images of the face.
raw_capture = np.empty(WIDTH * HEIGHT * 3, dtype=np.uint8)
# raw_capture = PiRGBArray(webcam)
while 1:
    print("reading webcam")

    webcam.capture(raw_capture, format="rgb", use_video_port=True)

    interesting_x = 0
    interesting_y = 0
    interesting_w = WIDTH
    interesting_h = HEIGHT
    interesting_area = 0

    reshaped_capture = np.reshape(raw_capture, (HEIGHT, WIDTH, 3))
    gray_capture = cv2.cvtColor(reshaped_capture, cv2.COLOR_RGB2GRAY)

    print("detecting")
    faces = face_cascade.detectMultiScale(gray_capture, minNeighbors=3, minSize=(20, 20))

    print("detected", len(faces))
    for (x, y, w, h) in faces:
        print("face", (x, y, w, h))
        area = w * h
        if area > interesting_area:
            interesting_x = x
            interesting_y = y
            interesting_w = w
            interesting_h = h
            interesting_area = w * h

    image = Image.frombytes("RGB", (WIDTH, HEIGHT), raw_capture, "raw")
    rect = (interesting_x, interesting_y, (interesting_x + interesting_w), (interesting_y + interesting_h))
    print("display", rect)
    matrix.SetImage(image.crop(rect).resize((32,32), Image.NEAREST), 0, 0)

matrix.Clear()
