#!/usr/bin/env python3
"""Static checks of the actual UniXNU package integrated into dizi EDK2."""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[1]
X=ROOT/"Platforms/Xiaomi"
dsc=(X/"diziPkg/dizi.dsc").read_text()
fdf=(X/"diziPkg/dizi.fdf").read_text()
menu=(X/"DiziBootPkg/BootSelector/BootSelector.c").read_text()
app=(X/"UniXNUPkg/Application/UniXNU/UniXNU.c").read_text()
lib=(X/"UniXNUPkg/Library/UniXNULib/UniXNULib.inf").read_text()
ver=(ROOT/"tools/verify_dizi_native_fv.py").read_text()

class UniXnuDiziTests(unittest.TestCase):
    def test_compiled_from_pinned_real_unixnu_sources(self):
        self.assertIn("UniXNUPkg/Application/UniXNU/UniXNU.inf",dsc)
        self.assertIn("INF UniXNUPkg/Application/UniXNU/UniXNU.inf",fdf)
        self.assertIn("UniXNULib|UniXNUPkg/Library/UniXNULib/UniXNULib.inf",dsc)
        self.assertIn("FdtLib|MdePkg/Library/BaseFdtLib/BaseFdtLib.inf",dsc)
        self.assertIn("DiziProbe.c",lib)
        self.assertIn("UniXNURun (ImageHandle, Host)",app)
        self.assertIn("UniXNUProbePlatform()",app)
        self.assertIn("R5-UNIXNU-PROBE",menu)

    def test_physically_selected_and_reversible(self):
        self.assertIn("StartEmbeddedUniXnu(ImageHandle)",menu)
        self.assertIn("ReadSection(Fv, &mUniXnuAppGuid, EFI_SECTION_PE32",menu)
        self.assertIn("Loaded->LoadOptions = mDiziUniXnuProbeOptions",menu)
        self.assertIn("Render(Selected, &Input)",menu)
        self.assertIn("dizi:probe",app)
        self.assertNotIn("UniXNUPkg/Driver/UniXNUDxe/UniXNUDxe.inf",dsc+fdf)
        self.assertNotIn("UniXNURamDiskDxe.inf",dsc+fdf)
        self.assertIn("UNIXNU_GUID",ver)
        self.assertIn("--require-unixnu",ver)
        self.assertIn('"dizi:probe".encode("utf-16le")',ver)
        self.assertNotIn("BootOrder",app)

if __name__=="__main__":
    unittest.main()
