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

from PIL import Image, ImageDraw, ImageEnhance
import cv2
import time
import io
import numpy as np
import cv2, sys, numpy, os
import numpy as np
import threading
from picamera import PiCamera
from picamera.array import PiRGBArray
from rgbmatrix import RGBMatrix, RGBMatrixOptions

MATRIX_WIDTH = 32
MATRIX_HEIGHT = 32
WIDTH = 800
HEIGHT = 608
FACE_MARGIN = 50
POL_SPEED = 0.1
ENHANCE_CONTRAST = 1.8

# Configuration for the matrix
options = RGBMatrixOptions()
options.rows = MATRIX_HEIGHT
options.chain_length = 1
options.parallel = 1
options.hardware_mapping = "adafruit-hat" # If you have an Adafruit HAT: 'adafruit-hat'

matrix = RGBMatrix(options=options)

print("open haar")
face_cascade = cv2.CascadeClassifier("haar.xml")
print("open webcam")
webcam = PiCamera(resolution=(WIDTH, HEIGHT))
 
interesting_x = 0
interesting_y = 0
interesting_w = WIDTH
interesting_h = HEIGHT

pol_interesting_x = 0
pol_interesting_y = 0
pol_interesting_w = WIDTH
pol_interesting_h = HEIGHT

detect_thread = None

def lerp(a, b, x):
    return a + (b - a) * x

def detect_faces(reshaped_capture):
    global face_cascade
    global interesting_x
    global interesting_y
    global interesting_w
    global interesting_h
  
    gray_capture = cv2.cvtColor(reshaped_capture, cv2.COLOR_RGB2GRAY)
    faces = face_cascade.detectMultiScale(gray_capture, scaleFactor=1.3, minNeighbors=3, minSize=(20, 20))

    current_interesting_x = 0
    current_interesting_y = 0
    current_interesting_w = WIDTH
    current_interesting_h = HEIGHT
    current_interesting_area = 0

    for (x, y, w, h) in faces:
        print("face", (x, y, w, h))
        area = w * h
        if area > current_interesting_area:
            current_interesting_x = x
            current_interesting_y = y
            current_interesting_w = w
            current_interesting_h = h
            current_interesting_area = w * h

    interesting_x = max(current_interesting_x - FACE_MARGIN, 0)
    interesting_y = max(current_interesting_y - FACE_MARGIN, 0)
    interesting_w = min(current_interesting_w + FACE_MARGIN, WIDTH)
    interesting_h = min(current_interesting_h + FACE_MARGIN, HEIGHT)


raw_capture = np.empty(WIDTH * HEIGHT * 3, dtype=np.uint8)
while 1:
    print("reading webcam")

    webcam.capture(raw_capture, format="rgb", use_video_port=True)

    if not detect_thread or not detect_thread.is_alive():
        print("start detection thread")
        reshaped_capture = np.reshape(raw_capture, (HEIGHT, WIDTH, 3))
        detect_thread = threading.Thread(target=detect_faces, args=(reshaped_capture,))
        detect_thread.start()

    pol_interesting_x = lerp(pol_interesting_x, interesting_x, POL_SPEED)
    pol_interesting_y = lerp(pol_interesting_y, interesting_y, POL_SPEED)
    pol_interesting_w = lerp(pol_interesting_w, interesting_w, POL_SPEED)
    pol_interesting_h = lerp(pol_interesting_h, interesting_h, POL_SPEED)

    image = Image.frombytes("RGB", (WIDTH, HEIGHT), raw_capture, "raw")
    rect = (pol_interesting_x, pol_interesting_y, (pol_interesting_x + pol_interesting_w), (pol_interesting_y + pol_interesting_h))
  
    cropped_image = image.crop(rect).resize((MATRIX_WIDTH, MATRIX_HEIGHT), Image.NEAREST)

    if ENHANCE_CONTRAST != 1:
        image_enhancer = ImageEnhance.Contrast(cropped_image)
        enhanced_image = image_enhancer.enhance(ENHANCE_CONTRAST)
    else:
        enhanced_image = cropped_image

    print("display", rect)
    matrix.SetImage(enhanced_image, 0, 0)

matrix.Clear()
