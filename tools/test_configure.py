import hashlib, importlib.util, sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parent.parent
spec = importlib.util.spec_from_file_location("configure", ROOT / "configure.py")
configure = importlib.util.module_from_spec(spec)
spec.loader.exec_module(configure)


def _fake_iso(sector_size, data_offset):
    """Minimal ISO: PVD at LBA 16, one root dir entry for SLPS_009.12;1."""
    def sector(payload):
        s = bytearray(sector_size)
        s[data_offset:data_offset + len(payload)] = payload
        return bytes(s)

    pvd = bytearray(2048)
    pvd[0] = 1
    pvd[1:6] = b"CD001"
    root = bytearray(34)
    root[0] = 34
    root[2:6] = (20).to_bytes(4, "little")
    root[10:14] = (2048).to_bytes(4, "little")
    root[25] = 0x02
    root[32] = 1
    pvd[156:190] = root

    name = b"SLPS_009.12;1"
    rec = bytearray(33 + len(name))
    rec[0] = len(rec)
    rec[2:6] = (30).to_bytes(4, "little")
    rec[10:14] = (622592).to_bytes(4, "little")
    rec[25] = 0x00
    rec[32] = len(name)
    rec[33:] = name
    rootdir = bytearray(2048)
    rootdir[0:len(rec)] = rec

    out = bytearray()
    for lba in range(40):
        if lba == 16: out += sector(bytes(pvd))
        elif lba == 20: out += sector(bytes(rootdir))
        else: out += sector(b"")
    return bytes(out)


def test_detect_cooked_2048():
    assert configure.detect_layout(_fake_iso(2048, 0)) == (2048, 0)


def test_detect_raw_mode2_2352():
    assert configure.detect_layout(_fake_iso(2352, 24)) == (2352, 24)


def test_detect_raw_mode1_2352():
    assert configure.detect_layout(_fake_iso(2352, 16)) == (2352, 16)


def test_find_file_returns_lba_and_size():
    img = _fake_iso(2352, 24)
    lba, size = configure.find_file(img, "SLPS_009.12")
    assert (lba, size) == (30, 622592)


def test_find_file_missing_raises():
    img = _fake_iso(2048, 0)
    try:
        configure.find_file(img, "NOSUCH.EXE")
    except FileNotFoundError:
        return
    raise AssertionError("expected FileNotFoundError")


def _fake_iso_split_dir():
    """A 2048/0 ISO whose root directory spans two sectors: sector 0 holds
    one filler record then zero padding to the sector boundary, sector 1
    holds the SLPS_009.12 record."""
    sector_size, data_offset = 2048, 0

    def sector(payload):
        s = bytearray(sector_size)
        s[data_offset:data_offset + len(payload)] = payload
        return bytes(s)

    pvd = bytearray(2048)
    pvd[0] = 1
    pvd[1:6] = b"CD001"
    root = bytearray(34)
    root[0] = 34
    root[2:6] = (20).to_bytes(4, "little")
    root[10:14] = (4096).to_bytes(4, "little")  # spans LBA 20 and 21
    root[25] = 0x02
    root[32] = 1
    pvd[156:190] = root

    filler_name = b"AAAAAAAA.TXT;1"
    filler = bytearray(33 + len(filler_name))
    filler[0] = len(filler)
    filler[2:6] = (25).to_bytes(4, "little")
    filler[10:14] = (100).to_bytes(4, "little")
    filler[25] = 0x00
    filler[32] = len(filler_name)
    filler[33:] = filler_name
    dir0 = bytearray(2048)
    dir0[0:len(filler)] = filler
    # remainder of dir0 stays zero-padded to the sector boundary

    name = b"SLPS_009.12;1"
    rec = bytearray(33 + len(name))
    rec[0] = len(rec)
    rec[2:6] = (30).to_bytes(4, "little")
    rec[10:14] = (622592).to_bytes(4, "little")
    rec[25] = 0x00
    rec[32] = len(name)
    rec[33:] = name
    dir1 = bytearray(2048)
    dir1[0:len(rec)] = rec

    out = bytearray()
    for lba in range(40):
        if lba == 16: out += sector(bytes(pvd))
        elif lba == 20: out += sector(bytes(dir0))
        elif lba == 21: out += sector(bytes(dir1))
        else: out += sector(b"")
    return bytes(out)


def test_records_skips_sector_padding():
    img = _fake_iso_split_dir()
    lba, size = configure.find_file(img, "SLPS_009.12")
    assert (lba, size) == (30, 622592)


def test_correct_size_wrong_hash_reports_sha1(tmp_path, monkeypatch):
    bad = tmp_path / "SLPS_009.12"
    bad.write_bytes(b"\x00" * configure.EXE_SIZE)
    monkeypatch.setattr(sys, "argv", ["configure.py", str(bad)])
    with pytest.raises(SystemExit) as excinfo:
        configure.main()
    msg = str(excinfo.value)
    assert "sha1 mismatch" in msg
    assert "Traceback" not in msg


def test_unreadable_image_reports_cleanly(tmp_path, monkeypatch):
    bogus = tmp_path / "garbage.iso"
    # A genuinely tiny, wrong-size buffer with no CD001 signature. Once
    # detect_layout bounds-checks each candidate offset against len(data),
    # this cleanly exhausts CANDIDATE_LAYOUTS instead of raising IndexError.
    bogus.write_bytes(b"\xab" * 300)
    monkeypatch.setattr(sys, "argv", ["configure.py", str(bogus)])
    with pytest.raises(SystemExit) as excinfo:
        configure.main()
    msg = str(excinfo.value)
    assert "cannot read" in msg
    assert "Traceback" not in msg


def test_cue_missing_bin_reports_cleanly(tmp_path, monkeypatch):
    cue = tmp_path / "game.cue"
    cue.write_text('FILE "missing.bin" BINARY\n  TRACK 01 MODE2/2352\n    INDEX 01 00:00:00\n')
    monkeypatch.setattr(sys, "argv", ["configure.py", str(cue)])
    with pytest.raises(SystemExit) as excinfo:
        configure.main()
    msg = str(excinfo.value)
    assert "missing" in msg
    assert "Traceback" not in msg


def test_tiny_input_reports_cleanly():
    """A severely truncated dump must not raise IndexError."""
    try:
        configure.detect_layout(b"\x00" * 4096)
    except ValueError:
        return
    raise AssertionError("expected ValueError for a truncated image")
