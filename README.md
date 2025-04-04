# OnePlus 13R Kernel Source

This repository contains the kernel source for the OnePlus 13R (kalama).

## Device Specifications

| Feature                 | Specification                                                                                                                                                                                      |
| ----------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| CPU                     | Qualcomm® Kryo™ 980 CPU @3.3GHz                                                                                                                                                                    |
| Chipset                 | Snapdragon® 8 Gen 3 Mobile Platform                                                                                                                                                                |
| GPU                     | Adreno™ 750 @903MHz                                                                                                                                                                                |
| Memory                  | 12GB/16GB LPDDR5X                                                                                                                                                                                  |
| Shipped Android Version | OxygenOS 15.0 based on Android™ 15                                                                                                                                                                 |
| Storage                 | 256GB/512GB UFS 4.0                                                                                                                                                                                |
| Battery                 | 6,000 mAh (single-cell, non-removable)                                                                                                                                                             |
| Display                 | 17.22 cm (6.78 inches), 2780×1264 pixels, 450 ppi, 120Hz ProXDR Display with LTPO 4.1, HBM/Peak Brightness: 1600 nits/4500 nits, 100% Display P3, 10-bit Color Depth, Corning® Gorilla® Glass GG7i |
| Camera (Rear)           | 50 MP (wide), Sony LYT-700, 1/1.56", f/1.8, OIS, 50 MP (telephoto), 2X optical zoom, f/2.0, 8 MP (ultrawide), f/2.2, 112° FOV                                                                      |
| Camera (Front)          | 16 MP (wide), 1/3.09", f/2.4, 82° FOV                                                                                                                                                              |
| Charging                | 80W SUPERVOOC™                                                                                                                                                                                     |

## Building the Kernel

1. Set up the build environment:
```bash
source build/envsetup.sh
lunch lineage_13r-userdebug
```

2. Build the kernel:
```bash
make kernel
```

3. Build kernel modules:
```bash
make modules
```

4. Install kernel headers:
```bash
make headers
```

## Kernel Features

- Based on Linux 5.15
- Built with Clang 16.0.1
- Supports Android 15
- Includes Qualcomm Snapdragon 8 Gen 3 optimizations
- Supports dynamic CPU frequency scaling
- Includes power management optimizations
- Supports display and camera features
- Includes security enhancements

## Directory Structure

```
android_kernel_oneplus_13r/
├── arch/arm64/configs/vendor/kalama_defconfig
├── Android.mk
├── build_kernel.mk
└── README.md
```

## Dependencies

- Android NDK
- Clang 16.0.1
- LLVM tools
- Qualcomm Snapdragon 8 Gen 3 BSP

## Contributing

1. Fork this repository
2. Create a new branch for your changes
3. Make your changes
4. Submit a pull request

## License

This project is licensed under the GPL-2.0 License - see the LICENSE file for details. 