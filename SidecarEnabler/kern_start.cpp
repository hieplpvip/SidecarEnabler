//
//  kern_start.cpp
//  SidecarEnabler
//
//  Copyright © 2020 Le Bao Hiep. All rights reserved.
//

#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>
#include <Headers/kern_file.hpp>

#pragma mark -
#pragma mark Constants
#pragma mark -

static const uint32_t SectionActive = 1;

static const char* SidecarCoreBinaryPath = "/System/Library/PrivateFrameworks/SidecarCore.framework/Versions/A/SidecarCore";

// I found these binaries by running
// find / -perm +111 -type f -exec sh -c 'jtool2 -L "{}" 2>/dev/null | grep SidecarCore && echo "SidecarCore matched at {}"' \;
static UserPatcher::ProcInfo ADDPR(procInfo)[] = {
    { "/usr/libexec/SidecarDisplayAgent", 32, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/usr/libexec/SidecarRelay", 25, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/System/Library/PreferencePanes/Sidecar.prefPane/Contents/MacOS/Sidecar", 71, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/System/Library/PreferencePanes/Sidecar.prefPane/Contents/Resources/sidecarPrefCheck", 84, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/System/Library/PrivateFrameworks/AnnotationKit.framework/Versions/A/AnnotationKit", 82, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/System/Library/PrivateFrameworks/SidecarCore.framework/Versions/A/XPCServices/DisplayMarkup.xpc/Contents/MacOS/DisplayMarkup", 125, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/System/Library/PrivateFrameworks/ReminderKitUI.framework/Versions/A/XPCServices/com.apple.ReminderKitUI.ReminderCreationViewService.xpc/Contents/MacOS/com.apple.ReminderKitUI.ReminderCreationViewService", 203, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/System/Library/PrivateFrameworks/MarkupUI.framework/Versions/A/MarkupUI", 72, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/System/Library/PrivateFrameworks/SidecarUI.framework/Versions/A/SidecarUI", 74, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/System/Applications/Notes.app/Contents/MacOS/Notes", 51, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/System/Applications/Mail.app/Contents/MacOS/Mail", 49, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/System/Applications/Reminders.app/Contents/MacOS/Reminders", 59, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact },
    { "/System/Applications/Reminders.app/Contents/PlugIns/RemindersSharingExtension.appex/Contents/MacOS/RemindersSharingExtension", 124, SectionActive, UserPatcher::ProcInfo::ProcFlags::MatchExact }
};

#pragma mark -
#pragma mark Patch version 1
#pragma mark -

// iPad4,X name patches
static const uint8_t find_iPad4[] = "iPad4";
static const uint8_t repl_iPad4[] = "iXad4";

// iPad5,X name patches
static const uint8_t find_iPad5[] = "iPad5";
static const uint8_t repl_iPad5[] = "iXad5";

// iPad6,X name patches
static const uint8_t find_iPad6[] = "iPad6";
static const uint8_t repl_iPad6[] = "iXad6";

// iMacX,X name patches
static const uint8_t find_iMac[] = "iMac";
static const uint8_t repl_iMac[] = "iXac";

// MacBookX,X MacBookAirX,X MacBookProX,X name patches
static const uint8_t find_MacBook[] = "MacBook";
static const uint8_t repl_MacBook[] = "XacBook";

// MacminiX,X name patches
static const uint8_t find_Macmini[] = "Macmini";
static const uint8_t repl_Macmini[] = "Xacmini";

// MacminiX,X name patches
static const uint8_t find_MacPro[] = "MacPro";
static const uint8_t repl_MacPro[] = "XacPro";

static UserPatcher::BinaryModPatch patches_v1[] {
    { CPU_TYPE_X86_64, 0, find_iPad4, repl_iPad4, 5, 0, 9, UserPatcher::FileSegment::SegmentTextCstring, SectionActive },
    { CPU_TYPE_X86_64, 0, find_iPad5, repl_iPad5, 5, 0, 4, UserPatcher::FileSegment::SegmentTextCstring, SectionActive },
    { CPU_TYPE_X86_64, 0, find_iPad6, repl_iPad6, 5, 0, 2, UserPatcher::FileSegment::SegmentTextCstring, SectionActive },
    { CPU_TYPE_X86_64, 0, find_iMac, repl_iMac, 4, 0, 10, UserPatcher::FileSegment::SegmentTextCstring, SectionActive },
    { CPU_TYPE_X86_64, 0, find_MacBook, repl_MacBook, 7, 0, 17, UserPatcher::FileSegment::SegmentTextCstring, SectionActive },
    { CPU_TYPE_X86_64, 0, find_Macmini, repl_Macmini, 7, 0, 3, UserPatcher::FileSegment::SegmentTextCstring, SectionActive },
    { CPU_TYPE_X86_64, 0, find_MacPro, repl_MacPro, 6, 0, 2, UserPatcher::FileSegment::SegmentTextCstring, SectionActive }
};

static UserPatcher::BinaryModInfo ADDPR(binaryMod_v1)[] {
    { SidecarCoreBinaryPath, patches_v1, arrsize(patches_v1) }
};

#pragma mark -
#pragma mark Patch version 2
#pragma mark -

static const size_t patchBytesCount = 30;
static const size_t gadgetBytesCount = 6;

// Patch _SidecarDisplayIsSupportedMac to return 1
static const uint8_t ref_mac[] = { 0x4c, 0x8d, 0x25, 0x23, 0xd7, 0x00, 0x00 };
static uint8_t find_mac[patchBytesCount] = {};
static uint8_t repl_mac[patchBytesCount] = {
    0xb8, 0x01, 0x00, 0x00, 0x00,  // mov eax, 0x1
    0xc3                           // ret
};

// Patch ___filterSupportedDevices_block_invoke to return 1
static const uint8_t ref_filter[] = { 0x4c, 0x8d, 0x3d, 0x8f, 0xb1, 0x00, 0x00 };
static uint8_t find_filter[patchBytesCount] = {};
static uint8_t repl_filter[patchBytesCount] = {
    0xb8, 0x01, 0x00, 0x00, 0x00,  // mov eax, 0x1
    0xc3                           // ret
};

static UserPatcher::BinaryModPatch patches_v2[] {
    { CPU_TYPE_X86_64, 0, find_mac, repl_mac, patchBytesCount, 0, 1, UserPatcher::FileSegment::SegmentTextText, SectionActive },
    { CPU_TYPE_X86_64, 0, find_filter, repl_filter, patchBytesCount, 0, 1, UserPatcher::FileSegment::SegmentTextText, SectionActive }
};

static UserPatcher::BinaryModInfo ADDPR(binaryMod_v2)[] {
    { SidecarCoreBinaryPath, patches_v2, arrsize(patches_v2) }
};

static void buildPatch(const char *path, const uint8_t *refBuffer, uint8_t *findBuffer, uint8_t *replBuffer, const char *funcName) {
    DBGLOG("SidecarEnabler", "buildPatch %s start", funcName);
    
    // Get contents of binary.
    size_t outSize;
    uint8_t *buffer = FileIO::readFileToBuffer(path, outSize);
    if (buffer == NULL) {
        panic("SidecarEnabler: Failed to read binary %s\n", path);
    }
    
    // Find reference.
    off_t index = -1;
    for (off_t i = 0; i + 6 < outSize; ++i) {
        if (buffer[i    ] == refBuffer[0] &&
            buffer[i + 1] == refBuffer[1] &&
            buffer[i + 2] == refBuffer[2] &&
            buffer[i + 3] == refBuffer[3] &&
            buffer[i + 4] == refBuffer[4] &&
            buffer[i + 5] == refBuffer[5] &&
            buffer[i + 6] == refBuffer[6]) {
            index = i;
            break;
        }
    }
    
    // If we found no match, we can't go on.
    if (index == -1) {
        panic("SidecarEnabler: Failed to find reference for function %s in binary %s\n", funcName, path);
    }
    
    // Go back to the start of the function
    while (index >= 0 && !(buffer[index] == 0x55 && buffer[index + 1] == 0x48 && buffer[index + 2] == 0x89 && buffer[index + 3] == 0xe5)) {
        --index;
    }

    // If we found no match, we can't go on.
    if (index == -1) {
        panic("SidecarEnabler: Failed to find the start of function %s in binary %s\n", funcName, path);
    }

    // Build find pattern.
    uint8_t *bufferOffset = buffer + index;
    for (uint32_t i = 0; i < patchBytesCount; ++i) {
        findBuffer[i] = bufferOffset[i];
        SYSLOG("SidecarEnabler", "%x\n", findBuffer[i]);
    }
    for (uint32_t i = gadgetBytesCount; i < patchBytesCount; ++i) {
        replBuffer[i] = bufferOffset[i];
    }

    // Free buffer.
    Buffer::deleter(buffer);
}

static void buildPatches(void *user, KernelPatcher &patcher) {
    buildPatch(SidecarCoreBinaryPath, ref_mac, find_mac, repl_mac, "_SidecarDisplayIsSupportedMac");
    buildPatch(SidecarCoreBinaryPath, ref_filter, find_filter, repl_filter, "___filterSupportedDevices_block_invoke");
}

#pragma mark -
#pragma mark Patch version 3
#pragma mark -

// Patch _SidecarDisplayIsSupportedMac to return 1
static const uint8_t find_mac_v3[] = {
    0x55, 0x48, 0x89, 0xe5, 0x41, 0x57, 0x41, 0x56, 0x41, 0x55, 0x41, 0x54, 0x53, 0x48, 0x83, 0xec, 0x18, 0x8a, 0x05
};
static const uint8_t repl_mac_v3[] {
    0x31, 0xc0, 0xff, 0xc0, 0xc3, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
};

// Patch ___filtcerSupportedDevices_block_invoke to return 1
static const uint8_t find_filter_v3[] = {
    0x55, 0x48, 0x89, 0xe5, 0x41, 0x57, 0x41, 0x56, 0x41, 0x55, 0x41, 0x54, 0x53, 0x48, 0x83, 0xec, 0x28, 0x48, 0x89, 0xf7, 0x48, 0x8b, 0x1d
};
static const uint8_t repl_filter_v3[] {
    0x31, 0xc0, 0xff, 0xc0, 0xc3, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
};

static UserPatcher::BinaryModPatch patches_v3[] {
    { CPU_TYPE_X86_64, 0, find_mac_v3, repl_mac_v3, arrsize(find_mac_v3), 0, 5, UserPatcher::FileSegment::SegmentTextText, SectionActive },
    { CPU_TYPE_X86_64, 0, find_filter_v3, repl_filter_v3, arrsize(find_filter_v3), 0, 5, UserPatcher::FileSegment::SegmentTextText, SectionActive }
};

static UserPatcher::BinaryModInfo ADDPR(binaryMod_v3)[] {
    { SidecarCoreBinaryPath, patches_v3, arrsize(patches_v3) }
};

#pragma mark -
#pragma mark Main program
#pragma mark -

static void ADDPR(startPlugin)() {
    DBGLOG("SidecarEnabler", "start");
    bool use_old_patch = checkKernelArgument("-scuoldpatch");
    if (use_old_patch) {
        lilu.onProcLoadForce(ADDPR(procInfo), arrsize(ADDPR(procInfo)), nullptr, nullptr, ADDPR(binaryMod_v1), arrsize(ADDPR(binaryMod_v1)));
    } else {
        //lilu.onPatcherLoadForce(buildPatches);
        //lilu.onProcLoadForce(ADDPR(procInfo), arrsize(ADDPR(procInfo)), nullptr, nullptr, ADDPR(binaryMod_v2), arrsize(ADDPR(binaryMod_v2)));
        lilu.onProcLoadForce(ADDPR(procInfo), arrsize(ADDPR(procInfo)), nullptr, nullptr, ADDPR(binaryMod_v3), arrsize(ADDPR(binaryMod_v3)));
    }
}

static const char *bootargOff[] {
    "-scuoff"
};
static const char *bootargDebug[] {
    "-scudbg"
};
static const char *bootargBeta[] {
    "-scubeta"
};

PluginConfiguration ADDPR(config) {
    xStringify(PRODUCT_NAME),
    parseModuleVersion(xStringify(MODULE_VERSION)),
    LiluAPI::AllowNormal,
    bootargOff,
    arrsize(bootargOff),
    bootargDebug,
    arrsize(bootargDebug),
    bootargBeta,
    arrsize(bootargBeta),
    KernelVersion::Catalina,
    KernelVersion::BigSur,
    []() {
        ADDPR(startPlugin)();
    }
};
