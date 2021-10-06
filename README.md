# SidecarEnabler

***This kernel extension is no longer maintained! Please use [FeatureUnlock](https://github.com/acidanthera/FeatureUnlock) instead.***

[![Github release](https://img.shields.io/github/release/hieplpvip/SidecarEnabler.svg?color=blue)](https://github.com/hieplpvip/SidecarEnabler/releases/latest)
[![Github downloads](https://img.shields.io/github/downloads/hieplpvip/SidecarEnabler/total.svg?color=blue)](https://github.com/hieplpvip/SidecarEnabler/releases)
[![Build Status](https://travis-ci.com/hieplpvip/SidecarEnabler.svg?branch=master)](https://travis-ci.com/hieplpvip/SidecarEnabler)
[![Scan Status](https://scan.coverity.com/projects/21608/badge.svg)](https://scan.coverity.com/projects/21608)
[![Donate with PayPal](https://img.shields.io/badge/paypal-donate-red.svg)](https://paypal.me/lebhiep)

A [Lilu](https://github.com/acidanthera/Lilu) plugin that remove any SMBIOS checks used by Sidecar (for both Macs and iPads).

Tested with iPad mini 4.

### Requirement
- macOS Catalina or later
- [Lilu](https://github.com/vit9696/Lilu) 1.4.6 or later.

### Boot arguments
- `-scuoff` disables kext loading
- `-scudbg` turns on debugging output (available in DEBUG binaries)
- `-scubeta` enables loading on unsupported macOS versions

### Background

Sidecar is disabled on these devices by Apple:
- iPad: `iPad4,1; iPad4,2; iPad4,3; iPad4,4; iPad4,5; iPad4,6; iPad4,7; iPad4,8; iPad4,9; iPad5,1; iPad5,2; iPad5,3; iPad5,4; iPad6,11; iPad6,12`
- iMac: `iMac13,1; iMac13,2; iMac13,3; iMac14,1; iMac14,2; iMac14,3; iMac14,4; iMac15,1; iMac16,1; iMac16,2`
- MacBook: `MacBook8,1`
- MacBook Air: `MacBookAir5,1; MacBookAir5,2; MacBookAir6,1; MacBookAir6,2; MacBookAir7,1; MacBookAir7,2`
- MacBook Pro: `MacBookPro9,1; MacBookPro9,2; MacBookPro10,1; MacBookPro10,2; MacBookPro11,1; MacBookPro11,2; MacBookPro11,3; MacBookPro11,4; MacBookPro11,5; MacBookPro12,1`
- Mac mini: `Macmini6,1; Macmini6,2; Macmini7,1`
- Mac Pro: `MacPro5,1; MacPro6,1`

These identifiers are hardcoded in `/System/Library/PrivateFrameworks/SidecarCore.framework/Versions/A/SidecarCore`.

SidecarEnabler works by patching these identifiers so that they won't match any real devices (e.g. patching `iPad` to `iXad`, `MacBook` to `XacBook`).

As the check is done only on macOS side, there's no need to patch iPadOS (i.e. jailbreaking is not required).

### Notes

- Use a cable if wireless connection doesn't work for you (error 32002).
- SidecarEnabler only remove the check. It doesn't fix any hardware-related problems:
  + Apple disables Sidecar on Broadwell and older CPUs because those CPUs doesn't have built-in support for HEVC. Sidecar can still work but you will get some graphical glitches or stutter.
  + Sidecar may not work on iGPU-less systems (see [this](https://github.com/AMD-OSX/bugtracker/issues/1)).
- If you have problems with Sidecar, [Duet Display](https://www.duetdisplay.com/) may be a better choice.

### Credits
- [Apple](https://www.apple.com) for macOS
- [vit9696](https://github.com/vit9696) for [Lilu](https://github.com/acidanthera/Lilu)
- [pookjw](https://github.com/pookjw) for [SidecarPatcher](https://github.com/pookjw/SidecarPatcher)
