# Raspberry Pi pixel wall preparation guide with web based interface

Make sure the other instructions have been completed as well!

## 1.Install Java 8

```bash
sudo apt-get install openjdk-8-jdk
```

## 2. Build led-image-viewer

```bash
cd ~/rpi-rgb-led-matrix/utils
# In ~/rpi-rgb-led-matrix/utils (the library downloaded in previous instructions)
sudo make led-image-viewer
```

## 3. Make symlink to examples

```bash
cd ~/rpi-rgb-led-matrix
# In ~/rpi-rgb-led-matrix
ln -s bindings/python/ python
```

## 4. Download application

```bash
cd ~
# In ~
wget https://cdn-learn.adafruit.com/assets/assets/000/040/940/original/rpi-rgb-led-matrix-webapp-0.0.1-SNAPSHOT.war
```

## 5. Find ip of RPi

```
ip a
```

Look for an address that looks like `192.168.x.x` and remember it.

## 6. Run it!

```bash
java -jar payara-micro-prerelease-2016-11-24.jar --deploy rpi-rgb-led-matrix-webapp-0.0.1-SNAPSHOT.war
```

## 7. Use it!

Use your personal computer's webbrowser to go to `http://<ip of rpi>:8080/rpi-rgb-led-matrix-webapp-0.0.1-SNAPSHOT`. Your PC must be on the same local network as the RPi.

## 8. Edit panel count (when using more than 1 panel)

Go to the settings tab in the web application (`http://<ip of rpi>:8080/rpi-rgb-led-matrix-webapp-0.0.1-SNAPSHOT/settings/`) and add the `--led-pixel-mapper="U-mapper" --led-chain=4` options to the *Command Line Flags* input.

**Here are some more flags you can add:**

```text
--led-gpio-mapping=<name> : Name of GPIO mapping used. Default "regular"
--led-rows=<rows>         : Panel rows. Typically 8, 16, 32 or 64. (Default: 32).
--led-cols=<cols>         : Panel columns. Typically 32 or 64. (Default: 32).
--led-chain=<chained>     : Number of daisy-chained panels. (Default: 1).
--led-parallel=<parallel> : Parallel chains. range=1..3 (Default: 1).
--led-multiplexing=<0..18> : Mux type: 0=direct; 1=Stripe; 2=Checkered; 3=Spiral; 4=ZStripe; 5=ZnMirrorZStripe; 6=coreman; 7=Kaler2Scan; 8=ZStripeUneven; 9=P10-128x4-Z; 10=QiangLiQ8; 11=InversedZStripe; 12=P10Outdoor1R1G1-1; 13=P10Outdoor1R1G1-2; 14=P10Outdoor1R1G1-3; 15=P10CoremanMapper; 16=P8Outdoor1R1G1; 17=FlippedStripe; 18=P10Outdoor32x16HalfScan (Default: 0)
--led-pixel-mapper        : Semicolon-separated list of pixel-mappers to arrange pixels.
                            Optional params after a colon e.g. "U-mapper;Rotate:90"
                            Available: "Mirror", "Rotate", "U-mapper", "V-mapper". Default: ""
--led-pwm-bits=<1..11>    : PWM bits (Default: 11).
--led-brightness=<percent>: Brightness in percent (Default: 100).
--led-scan-mode=<0..1>    : 0 = progressive; 1 = interlaced (Default: 0).
--led-row-addr-type=<0..4>: 0 = default; 1 = AB-addressed panels; 2 = direct row select; 3 = ABC-addressed panels; 4 = ABC Shift + DE direct (Default: 0).
--led-show-refresh        : Show refresh rate.
--led-limit-refresh=<Hz>  : Limit refresh rate to this frequency in Hz. Useful to keep a
                            constant refresh rate on loaded system. 0=no limit. Default: 0
--led-inverse             : Switch if your matrix has inverse colors on.
--led-rgb-sequence        : Switch if your matrix has led colors swapped (Default: "RGB")
--led-pwm-lsb-nanoseconds : PWM Nanoseconds for LSB (Default: 130)
--led-pwm-dither-bits=<0..2> : Time dithering of lower bits (Default: 0)
--led-no-hardware-pulse   : Don't use hardware pin-pulse generation.
--led-panel-type=<name>   : Needed to initialize special panels. Supported: 'FM6126A', 'FM6127'
--led-slowdown-gpio=<0..4>: Slowdown GPIO. Needed for faster Pis/slower panels (Default: 2 (2 on Pi4, 1 other)).
--led-daemon              : Make the process run in the background as daemon.
--led-no-drop-privs       : Don't drop privileges from 'root' after initializing the hardware.
```