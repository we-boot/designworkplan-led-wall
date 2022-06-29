# Raspberry Pi pixel wall preparation guide

## 1. Enable camera

Enable camera in raspberry pi settings.

## 2. Install matrix libraries

```
curl https://raw.githubusercontent.com/adafruit/Raspberry-Pi-Installer-Scripts/master/rgb-matrix.sh >rgb-matrix.sh
sudo bash rgb-matrix.sh
```

## 3. Install dependencies

```
sudo apt install python3-pip python3-opencv python3-picamera
```

## 4. Install python dependencies

```
pip3 install numpy pillow
```

## 5. Download webcam script

```
curl -o webcam.py https://raw.githubusercontent.com/we-boot/designworkplan-led-wall/master/raspberry-pi-webcam/webcam.py
```

## 6. Run it!!

```
python3 webcam.py
```

## 7. Edit it!

You can open the webcam.py script to set the matrix width/height and chain length when using multiple panels.