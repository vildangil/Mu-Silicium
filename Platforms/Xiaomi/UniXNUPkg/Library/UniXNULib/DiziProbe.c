/** @file
  UniXNU SM7435 safe handoff smoke test.
  Runs only when launched from the dizi firmware menu with 'dizi:probe'.
  No NVRAM changes, no ExitBootServices and no XNU kernel entry.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/SimpleFileSystem.h>

extern VOID UniXNULogInit(VOID);
extern VOID EFIAPI UniXNULog(IN CONST CHAR8 *Format, ...);

EFI_STATUS
EFIAPI
UniXNUProbePlatform(VOID)
{
  EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop = NULL;
  EFI_SIMPLE_TEXT_INPUT_PROTOCOL *Input = NULL;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *Fs = NULL;
  EFI_STATUS GopStatus, InputStatus, FsStatus;
  UniXNULogInit();
  GopStatus = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid,
                                 NULL, (VOID **)&Gop);
  InputStatus = gBS->LocateProtocol(&gEfiSimpleTextInProtocolGuid,
                                   NULL, (VOID **)&Input);
  FsStatus = gBS->LocateProtocol(&gEfiSimpleFileSystemProtocolGuid,
                                NULL, (VOID **)&Fs);
  UniXNULog("DIZI R5 EFI handoff: GOP=%r ConIn=%r FS=%r",
            GopStatus, InputStatus, FsStatus);
  Print(L"UNIXNU R5 / DIZI SM7435\r\n");
  Print(L"EFI app loaded from firmware volume.\r\n");
  Print(L"GOP:%r  ConIn:%r  FS:%r\r\n",
        GopStatus, InputStatus, FsStatus);
  Print(L"Kernel handoff disabled: ADT not ported.\r\n");
  Print(L"Returning to DIZI firmware menu.\r\n");
  gBS->Stall(2000000);
  return EFI_SUCCESS;
}
