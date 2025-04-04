# OnePlus 13R Kernel Source

This repository contains the kernel source code for the OnePlus 13R, optimized for LineageOS compatibility.

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