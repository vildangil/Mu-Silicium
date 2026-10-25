/** @file
  UniXNU UEFI application entry point.

  Copyright (c) 2026, UniXNU contributors. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UniXNULib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LoadedImage.h>

//
// Optional data of the boot option UniXNUDxe registers. Keep in sync with
// UniXNUDxe.c.
//
STATIC CONST CHAR16  mFirmwareMarker[] = L"unixnu:firmware";
STATIC CONST CHAR16  mDiziProbeMarker[] = L"dizi:probe";

EFI_STATUS
EFIAPI
UniXNUApplicationEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_LOADED_IMAGE_PROTOCOL  *Loaded;
  UNIXNU_HOST             Host;
  EFI_STATUS                 Status;

  Loaded = NULL;
  Host = UniXNUHostApplication;
  if (!EFI_ERROR (gBS->HandleProtocol (ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID **)&Loaded)) &&
      (Loaded->LoadOptions != NULL) && (Loaded->LoadOptionsSize >= sizeof (mFirmwareMarker)) &&
      (CompareMem (Loaded->LoadOptions, mFirmwareMarker, sizeof (mFirmwareMarker)) == 0))
  {
    Host = UniXNUHostFirmware;
  }

  // Early diagnostic handoff for SM7435: link the real UniXNU library,
  // report firmware protocols, then return without reading a payload or
  // entering XNU. The full loader remains disabled until ADT is ported.
  if ((Loaded != NULL) &&
      (Loaded->LoadOptions != NULL) &&
      (Loaded->LoadOptionsSize == sizeof(mDiziProbeMarker)) &&
      (CompareMem(Loaded->LoadOptions, mDiziProbeMarker,
                  sizeof(mDiziProbeMarker)) == 0)) {
    return UniXNUProbePlatform();
  }

  Status = UniXNURun (ImageHandle, Host);

  //
  // "Return to firmware" from a firmware boot option: report failure so BDS
  // moves on to its next option instead of restarting UniXNU.
  //
  if ((Host == UniXNUHostFirmware) && !EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }

  return Status;
}
