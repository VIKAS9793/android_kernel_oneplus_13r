# OnePlus 13R Kernel

Linux kernel source code for the OnePlus 13R (CPH2691) device.

## Device Specifications

* **Dimensions**: 161.7 x 75.8 x 8.0 mm, 206g
* **SoC**: Qualcomm SM8650 (Snapdragon® 8 Gen 3)
* **CPU**: 
  * Qualcomm® Kryo™ 980 CPU @3.3GHz
  * 1x Prime + 3x Performance + 4x Efficiency cores
* **GPU**: Adreno™ 750 @903MHz
* **Memory**: 12GB/16GB LPDDR5X
* **Storage**: 256GB/512GB UFS 4.0
* **Display**: 
  * 6.78" (17.22 cm) 120Hz ProXDR Display with LTPO 4.1
  * Resolution: 2780 x 1264 pixels, 450 ppi
  * Aspect Ratio: 19.8:9
  * Peak Brightness: 4500 nits (HBM: 1600 nits)
  * 100% Display P3, 10-bit Color Depth
  * Cover Glass: Corning® Gorilla® Glass GG7i
* **Battery**: 6,000 mAh (single-cell) with 80W SUPERVOOC™
* **Camera**:
  * Main: 50MP Sony LYT-700 (f/1.8, OIS+EIS)
    * Sensor Size: 1/1.56"
    * Pixel Size: 1.0 µm
    * FOV: 84°
  * Telephoto: 50MP S5KJN5 (f/2.0, 2X optical zoom)
    * Sensor Size: 1/2.75"
    * Pixel Size: 0.64 µm
  * Ultra-wide: 8MP (f/2.2, 112° FoV)
    * Sensor Size: 1/4"
    * Pixel Size: 1.12 µm
  * Front: 16MP (f/2.4)
    * Sensor Size: 1/3.09"
    * FOV: 82°
* **Video Capabilities**:
  * 4K@60fps/30fps
  * 1080P@60fps/30fps
  * SLO-MO: 1080P@240fps, 720P@480fps
  * Movie Mode: 3840x1644@30fps
* **Connectivity**:
  * 2G GSM: 850/900/1800MHz
  * 3G WCDMA: Bands 1/4/5/8
  * 4G LTE FDD: Bands 1/3/4/5/8/28
  * 4G LTE TDD: Bands 38/39/40/41
  * 5G NR: n1/n3/n5/n8/n28/n40/n41/n77/n78
  * Wi-Fi 7, 2×2 MIMO, 8 Spatial-stream sounding MU-MIMO
  * Bluetooth 5.4 with aptX, aptX-HD, LDAC, LHDC 5.0
  * NFC
  * GPS(L1+L5), GLONASS(G1), BDS(B1I+B1C+B2a), Galileo(E1+E5a), QZSS(L1+L5), NavIC(L5)
* **Sensors**:
  * In-display optical fingerprint sensor
  * Proximity sensor
  * Ambient light sensor
  * Color temperature sensor
  * E-compass
  * Accelerometer
  * Gyroscope
  * Hall Sensor
  * Infrared remote control
* **Ports & Buttons**:
  * USB Type-C (USB 2.0)
  * Dual nano-SIM slot
  * Alert Slider
* **Audio**:
  * OReality Audio
  * 3 mics
  * Support standard Type-C earphones
* **Multimedia Support**:
  * HDR: Dolby Vision®, HDR10+, HDRViVid
  * Audio: MP3, AAC, AMR, APE, OGG, FLAC, WAV, MIDI
  * Video: MP4, 3GP, MKV, MOV, HEVC, AVC, VP9, VP8, AV1
  * Image: JPEG, BMP, PNG, DNG, HEIF, AVIF, WEBP, GIF, WBMP, ICO
* **OS**: OxygenOS 15.0 based on Android™ 15

## Features

* Based on Linux 6.1.75 LTS
* Full SM8650/Kalama platform support
* OnePlus specific features:
  * Alert Slider support
  * Goodix in-display optical fingerprint sensor
  * 80W SUPERVOOC™ charging
  * 120Hz LTPO 4.1 display with ProXDR
  * Advanced camera features (Nightscape, Hi-Res, Pro Mode)
  * Goodix GTX8 touchscreen with gestures
  * Advanced power management
  * OReality Audio with 3 mics
  * Dolby Vision®, HDR10+, HDRViVid support
  * IR remote control

## Building the Kernel

### Requirements

* Linux build environment
* Required packages:
  ```bash
  build-essential
  libncurses5-dev
  libssl-dev
  flex
  bison
  gcc-aarch64-linux-gnu
  ```

### Build Instructions

1. Set up the build environment:
```bash
export ARCH=arm64
export SUBARCH=arm64
export CROSS_COMPILE=aarch64-linux-gnu-
```

2. Clone the repository:
```bash
git clone https://github.com/VIKAS9793/android_kernel_oneplus_13r.git
cd android_kernel_oneplus_13r
```

3. Configure the kernel:
```bash
make vendor/sm8650_defconfig
```

4. Build the kernel:
```bash
./build.sh
```

The compiled kernel image will be available at `arch/arm64/boot/Image.gz-dtb`

## Directory Structure

```
.
├── arch/arm64/
│   ├── configs/vendor/
│   │   ├── sm8650_defconfig
│   │   └── Kconfig.oneplus
│   └── boot/dts/vendor/qcom/
│       └── oneplus-13r.dts
├── drivers/oneplus/
│   ├── board/
│   │   └── board-13r.c
│   ├── fingerprint/
│   ├── tri-state-key/
│   ├── power/
│   └── touchscreen/
└── build.sh
```

## Device Tree

The device tree is located at `arch/arm64/boot/dts/vendor/qcom/oneplus-13r.dts` and includes:
* Platform base configuration (Kalama/SM8650)
* Display configuration
* Fingerprint sensor setup
* Alert Slider GPIO configuration
* SUPERVOOC charging parameters
* Touch panel configuration
* Audio setup
* WiFi/Bluetooth/NFC configurations

## Contributing

1. Fork the repository
2. Create a branch for your changes
3. Submit a pull request

## License

This kernel is released under the GPL v2 license. See the LICENSE file for more details.

## Copyright

```
/*
 * Copyright (C) 2024 OnePlus, Inc.
 * Author: VIKAS9793 <vikassahani17@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
```

## Device Information

- **Device**: OnePlus 13R
- **Manufacturer**: OnePlus
- **Chipset**: Qualcomm Snapdragon 8 Gen 3
- **CPU**: Qualcomm® Kryo™ 980 CPU @3.3GHz
- **GPU**: Adreno™ 750 @903MHz
- **Memory**: 12GB/16GB LPDDR5X
- **Storage**: 256GB/512GB UFS 4.0
- **Android Version**: 15.0.0.0 (OxygenOS 15.0)
- **Kernel Version**: 6.1.75 (Based on latest Linux 6.1 LTS)
- **Display**: 6.78" 2780×1264 pixels, 120Hz LTPO 4.1
- **Battery**: 6,000 mAh with 80W SUPERVOOC™
- **Cameras**: 
  - Rear: 50MP (main) + 50MP (telephoto) + 8MP (ultrawide)
  - Front: 16MP

## Features

- Optimized for LineageOS compatibility
- Enhanced audio configuration
- Improved power management
- USB-C audio support
- Moisture protection
- Multiple audio codec support (WCD938x, WSA883x)
- Proper sleep state handling
- Enhanced microphone and speaker configurations

## Directory Structure

```
android_kernel_oneplus_13r/
├── arch/
│   └── arm64/
│       ├── boot/
│       │   └── dts/
│       │       └── qcom/
│       │           ├── kalama-oneplus-13r-audio.dtsi
│       │           ├── kalama-oneplus-13r-audio-platform.dtsi
│       │           ├── kalama-oneplus-13r-audio-pinctrl.dtsi
│       │           ├── kalama-oneplus-13r-pinctrl.dtsi
│       │           └── kalama-oneplus-13r.dts
│       └── configs/
│           └── vendor/
│               └── kalama_defconfig
├── drivers/
│   └── oneplus/
│       └── 13r/
│           ├── Kconfig
│           └── Makefile
├── Android.mk
└── README.md
```

## Building Instructions

1. Set up the build environment:
```bash
export ARCH=arm64
export SUBARCH=arm64
export CROSS_COMPILE=aarch64-linux-gnu-
```

2. Configure the kernel:
```bash
make kalama_defconfig
```

3. Build the kernel:
```bash
make -j$(nproc)
```

## Audio Configuration

The kernel includes comprehensive audio configurations:

- **Main Audio Configuration**: `kalama-oneplus-13r-audio.dtsi`
  - Audio routing configurations
  - Microphone and speaker settings
  - PDM inputs support
  - ADC configurations
  - WCD features

- **Audio Platform**: `kalama-oneplus-13r-audio-platform.dtsi`
  - MI2S slave and external MCLK configurations
  - Pin control for active/sleep states
  - Device-specific configurations

- **Audio Pin Control**: `kalama-oneplus-13r-audio-pinctrl.dtsi`
  - I/O configurations
  - Power management
  - Drive strength and bias settings

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the GNU General Public License v2.0 - see the LICENSE file for details.

## Credits

- LineageOS Team
- OnePlus
- Qualcomm
- The Android Open Source Project

## Support

For support, please open an issue in this repository or contact the maintainers.

## Disclaimer

This kernel is provided as-is, with no warranties. Use at your own risk. 