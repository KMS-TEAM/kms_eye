# libSGM
---
A CUDA implementation performing Semi-Global Matching.

## Introduction
---

libSGM is library that implements in CUDA the Semi-Global Matching algorithm.  
From a pair of appropriately calibrated input images, we can obtain the disparity map.

## Features
---
Because it uses CUDA, we can compute the disparity map at high speed.

## Performance
The libSGM performance obtained from benchmark sample
### Settings
- image size : 1024 x 440
- disparity size : 128
- sgm path : 4 path
- subpixel : enabled

## Author
The "adaskit Team"  

The adaskit is an open-source project created by [Fixstars Corporation](https://www.fixstars.com/) and its subsidiary companies including [Fixstars Autonomous Technologies](https://at.fixstars.com/), aimed at contributing to the ADAS industry by developing high-performance implementations for algorithms with high computational cost.

## License
Apache License 2.0
