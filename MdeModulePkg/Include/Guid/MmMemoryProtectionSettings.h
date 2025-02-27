/** @file
Defines memory protection settings guid and struct

For information on the available settings, see:
https://github.com/microsoft/mu_basecore/tree/HEAD/Docs/feature_memory_protection.md

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __MM_MEMORY_PROTECTION_SETTINGS_H__
#define __MM_MEMORY_PROTECTION_SETTINGS_H__

typedef union {
  UINT8    Data;
  struct {
    UINT8    MmPageGuard : 1;
    UINT8    MmPoolGuard : 1;
    UINT8    Direction   : 1;
  } Fields;
} MM_HEAP_GUARD_POLICY;

typedef union {
  UINT32    Data;
  struct {
    UINT8    EfiReservedMemoryType      : 1;
    UINT8    EfiLoaderCode              : 1;
    UINT8    EfiLoaderData              : 1;
    UINT8    EfiBootServicesCode        : 1;
    UINT8    EfiBootServicesData        : 1;
    UINT8    EfiRuntimeServicesCode     : 1;
    UINT8    EfiRuntimeServicesData     : 1;
    UINT8    EfiConventionalMemory      : 1;
    UINT8    EfiUnusableMemory          : 1;
    UINT8    EfiACPIReclaimMemory       : 1;
    UINT8    EfiACPIMemoryNVS           : 1;
    UINT8    EfiMemoryMappedIO          : 1;
    UINT8    EfiMemoryMappedIOPortSpace : 1;
    UINT8    EfiPalCode                 : 1;
    UINT8    EfiPersistentMemory        : 1;
    UINT8    EfiUnacceptedMemoryType    : 1;
    UINT8    OEMReserved                : 1;
    UINT8    OSReserved                 : 1;
  } Fields;
} MM_HEAP_GUARD_MEMORY_TYPES;

typedef UINT8 MM_MEMORY_PROTECTION_SETTINGS_VERSION;

#define MM_MEMORY_PROTECTION_SETTINGS_CURRENT_VERSION  2 // Current iteration of MM_MEMORY_PROTECTION_SETTINGS

//
// Memory Protection Settings struct
//
typedef struct {
  // The current version of the structure definition. This is used to ensure there isn't a definition mismatch
  // if modules have differing iterations of this header. When creating this struct, use the
  // MM_MEMORY_PROTECTION_SETTINGS_CURRENT_VERSION macro.
  MM_MEMORY_PROTECTION_SETTINGS_VERSION    StructVersion;

  // If enabled, accessing NULL address in UEFI or SMM code can be caught by marking
  // the NULL page as not present.
  BOOLEAN                                  NullPointerDetectionPolicy;

  // Bitfield to control Heap Guard behavior.
  //
  // Note:
  //  a) Due to the limit of pool memory implementation and the alignment
  //     requirement of UEFI spec, HeapGuardPolicy.Direction is a try-best
  //     setting which cannot guarantee that the returned pool is exactly
  //     adjacent to head guard page or tail guard page.
  //
  //  .MmPageGuard          : Enable SMM page guard.
  //  .MmPoolGuard          : Enable SMM pool guard.
  //  .Direction             : The direction of Guard Page for Pool Guard.
  //                           0 - The returned pool is near the tail guard page.
  //                           1 - The returned pool is near the head guard page.
  MM_HEAP_GUARD_POLICY    HeapGuardPolicy;

  // Indicates which type allocation need guard page.
  //
  // If bit is set, a head guard page and a tail guard page will be added just
  // before and after corresponding type of pages which the allocated pool occupies,
  // if there's enough free memory for all of them. The pool allocation for the
  // type related to cleared bits keeps the same as ususal.
  //
  // This bitfield is only valid if UefiPoolGuard and/or MmPoolGuard are set in HeapGuardPolicy.
  MM_HEAP_GUARD_MEMORY_TYPES    HeapGuardPoolType;

  // Indicates which type allocation need guard page.
  //
  // If a bit is set, a head guard page and a tail guard page will be added just
  // before and after corresponding type of pages allocated if there's enough
  // free pages for all of them. The page allocation for the type related to
  // cleared bits keeps the same as ususal.
  //
  // This bitfield is only valid if MmPageGuard is set in HeapGuardPolicy.
  MM_HEAP_GUARD_MEMORY_TYPES    HeapGuardPageType;
} MM_MEMORY_PROTECTION_SETTINGS;

#define HOB_MM_MEMORY_PROTECTION_SETTINGS_GUID \
  { \
    { 0x0CF445DD, 0xA67C, 0x4F8C, { 0x81, 0x9B, 0xB7, 0xB6, 0x86, 0xED, 0x7C, 0x75 } } \
  }

extern GUID  gMmMemoryProtectionSettingsGuid;

// HeapGuardPolicy.Fields.Direction value indicating tail alignment
#define HEAP_GUARD_ALIGNED_TO_TAIL  0

// HeapGuardPolicy.Fields.Direction value indicating head alignment
#define HEAP_GUARD_ALIGNED_TO_HEAD  1

//
//  An SMM memory profile with strict settings. This will likely add to the
//  total boot time but will catch more configuration and memory errors.
//
#define MM_MEMORY_PROTECTION_SETTINGS_DEBUG                     \
          {                                                     \
            MM_MEMORY_PROTECTION_SETTINGS_CURRENT_VERSION,      \
            TRUE,                                               \
            {                                                   \
              .Fields.MmPageGuard                  = 1,         \
              .Fields.MmPoolGuard                  = 1,         \
              .Fields.Direction                    = 0          \
            },                                                  \
            {                                                   \
              .Fields.EfiReservedMemoryType        = 0,         \
              .Fields.EfiLoaderCode                = 0,         \
              .Fields.EfiLoaderData                = 0,         \
              .Fields.EfiBootServicesCode          = 0,         \
              .Fields.EfiBootServicesData          = 1,         \
              .Fields.EfiRuntimeServicesCode       = 0,         \
              .Fields.EfiRuntimeServicesData       = 1,         \
              .Fields.EfiConventionalMemory        = 0,         \
              .Fields.EfiUnusableMemory            = 0,         \
              .Fields.EfiACPIReclaimMemory         = 0,         \
              .Fields.EfiACPIMemoryNVS             = 0,         \
              .Fields.EfiMemoryMappedIO            = 0,         \
              .Fields.EfiMemoryMappedIOPortSpace   = 0,         \
              .Fields.EfiPalCode                   = 0,         \
              .Fields.EfiPersistentMemory          = 0,         \
              .Fields.EfiUnacceptedMemoryType      = 0,         \
              .Fields.OEMReserved                  = 0,         \
              .Fields.OSReserved                   = 0          \
            },                                                  \
            {                                                   \
              .Fields.EfiReservedMemoryType        = 0,         \
              .Fields.EfiLoaderCode                = 0,         \
              .Fields.EfiLoaderData                = 0,         \
              .Fields.EfiBootServicesCode          = 0,         \
              .Fields.EfiBootServicesData          = 1,         \
              .Fields.EfiRuntimeServicesCode       = 0,         \
              .Fields.EfiRuntimeServicesData       = 1,         \
              .Fields.EfiConventionalMemory        = 0,         \
              .Fields.EfiUnusableMemory            = 0,         \
              .Fields.EfiACPIReclaimMemory         = 0,         \
              .Fields.EfiACPIMemoryNVS             = 0,         \
              .Fields.EfiMemoryMappedIO            = 0,         \
              .Fields.EfiMemoryMappedIOPortSpace   = 0,         \
              .Fields.EfiPalCode                   = 0,         \
              .Fields.EfiPersistentMemory          = 0,         \
              .Fields.EfiUnacceptedMemoryType      = 0,         \
              .Fields.OEMReserved                  = 0,         \
              .Fields.OSReserved                   = 0          \
            }                                                   \
          }

//
//  An SMM memory profile with all settings off.
//
#define MM_MEMORY_PROTECTION_SETTINGS_OFF                       \
          {                                                     \
            MM_MEMORY_PROTECTION_SETTINGS_CURRENT_VERSION,      \
            FALSE,                                              \
            {                                                   \
              .Fields.MmPageGuard                  = 0,         \
              .Fields.MmPoolGuard                  = 0,         \
              .Fields.Direction                    = 0          \
            },                                                  \
            {                                                   \
              .Fields.EfiReservedMemoryType        = 0,         \
              .Fields.EfiLoaderCode                = 0,         \
              .Fields.EfiLoaderData                = 0,         \
              .Fields.EfiBootServicesCode          = 0,         \
              .Fields.EfiBootServicesData          = 0,         \
              .Fields.EfiRuntimeServicesCode       = 0,         \
              .Fields.EfiRuntimeServicesData       = 0,         \
              .Fields.EfiConventionalMemory        = 0,         \
              .Fields.EfiUnusableMemory            = 0,         \
              .Fields.EfiACPIReclaimMemory         = 0,         \
              .Fields.EfiACPIMemoryNVS             = 0,         \
              .Fields.EfiMemoryMappedIO            = 0,         \
              .Fields.EfiMemoryMappedIOPortSpace   = 0,         \
              .Fields.EfiPalCode                   = 0,         \
              .Fields.EfiPersistentMemory          = 0,         \
              .Fields.EfiUnacceptedMemoryType      = 0,         \
              .Fields.OEMReserved                  = 0,         \
              .Fields.OSReserved                   = 0          \
            },                                                  \
            {                                                   \
              .Fields.EfiReservedMemoryType        = 0,         \
              .Fields.EfiLoaderCode                = 0,         \
              .Fields.EfiLoaderData                = 0,         \
              .Fields.EfiBootServicesCode          = 0,         \
              .Fields.EfiBootServicesData          = 0,         \
              .Fields.EfiRuntimeServicesCode       = 0,         \
              .Fields.EfiRuntimeServicesData       = 0,         \
              .Fields.EfiConventionalMemory        = 0,         \
              .Fields.EfiUnusableMemory            = 0,         \
              .Fields.EfiACPIReclaimMemory         = 0,         \
              .Fields.EfiACPIMemoryNVS             = 0,         \
              .Fields.EfiMemoryMappedIO            = 0,         \
              .Fields.EfiMemoryMappedIOPortSpace   = 0,         \
              .Fields.EfiPalCode                   = 0,         \
              .Fields.EfiPersistentMemory          = 0,         \
              .Fields.EfiUnacceptedMemoryType      = 0,         \
              .Fields.OEMReserved                  = 0,         \
              .Fields.OSReserved                   = 0          \
            }                                                   \
          }

#endif
