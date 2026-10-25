# dizi UniXNU R5 experimental EFI probe

Upstream code: UniXNU/UniXNU commit `16e592abee8a550dec8154ed1f38b5ebc4f16213`
(2026). The UEFI application and library in `Platforms/Xiaomi/UniXNUPkg`
are copied from this commit with a small local `dizi:probe` handshake.

This branch adds **no** `UniXNUDxe`: that DXE driver would write Boot####
and BootOrder, which is inappropriate for reversible experimental boot.
The dizi menu alone uses the upstream application's real FFS GUID,
loads its PE32 bytes through FirmwareVolume2, and starts it after the
user explicitly selects `UNIXNU PROBE [EFI]`.

The probe executes code linked into the *actual UniXNULib* and reports
GOP, SimpleTextIn and SimpleFileSystem presence, then returns to the
existing DIZI selector. It does not execute `UniXNURun`, DarwinBoot or XNU.
There is no kernel, ADT, Apple firmware or APFS root. This is a strict
**firmware-to-application handoff test**, not a usable dualboot build.

Physical testing must remain `fastboot boot` of the generated dizi
Android boot-v4 image. Never flash Android partitions.
