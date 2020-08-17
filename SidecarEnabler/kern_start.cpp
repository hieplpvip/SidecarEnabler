/*
 * Copyright (c) 2020 Le Bao Hiep
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>

static const uint32_t SectionActive = 1;

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

static UserPatcher::BinaryModPatch patches[] {
    { CPU_TYPE_X86_64, 0, find_iPad4, repl_iPad4, 5, 0, 9, UserPatcher::FileSegment::SegmentTextCstring, SectionActive },
    { CPU_TYPE_X86_64, 0, find_iPad5, repl_iPad5, 5, 0, 4, UserPatcher::FileSegment::SegmentTextCstring, SectionActive },
    { CPU_TYPE_X86_64, 0, find_iPad6, repl_iPad6, 5, 0, 2, UserPatcher::FileSegment::SegmentTextCstring, SectionActive },
    { CPU_TYPE_X86_64, 0, find_iMac, repl_iMac, 4, 0, 10, UserPatcher::FileSegment::SegmentTextCstring, SectionActive },
    { CPU_TYPE_X86_64, 0, find_MacBook, repl_MacBook, 7, 0, 17, UserPatcher::FileSegment::SegmentTextCstring, SectionActive },
    { CPU_TYPE_X86_64, 0, find_Macmini, repl_Macmini, 7, 0, 3, UserPatcher::FileSegment::SegmentTextCstring, SectionActive },
    { CPU_TYPE_X86_64, 0, find_MacPro, repl_MacPro, 6, 0, 2, UserPatcher::FileSegment::SegmentTextCstring, SectionActive }
};

static const char* SidecarCoreBinaryPath = "/System/Library/PrivateFrameworks/SidecarCore.framework/Versions/A/SidecarCore";

static UserPatcher::BinaryModInfo ADDPR(binaryMod)[] {
    { SidecarCoreBinaryPath, patches, 7 }
};

const size_t ADDPR(binaryModSize) {1};

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

const size_t ADDPR(procInfoSize) {13};

static void startPlugin() {
    DBGLOG("SidecarEnabler", "start");
    lilu.onProcLoadForce(ADDPR(procInfo), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryMod), ADDPR(binaryModSize));
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
        startPlugin();
    }
};
