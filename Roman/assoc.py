"""
register_roman_script.py

Registers the .roman file extension with a "RomanScript" file type in the
Windows registry, equivalent to the .reg file:

    [HKEY_CLASSES_ROOT\\.roman]
    @="RomanScript"
    [HKEY_CLASSES_ROOT\\RomanScript]
    @="Roman Script"
    [HKEY_CLASSES_ROOT\\RomanScript\\shell]
    [HKEY_CLASSES_ROOT\\RomanScript\\shell\\open]
    [HKEY_CLASSES_ROOT\\RomanScript\\shell\\open\\command]
    @="\"C:\\Path\\To\\interpreter.exe\" \"%1\""

Must be run on Windows with administrator privileges (HKEY_CLASSES_ROOT
is writable by non-admins for the current user in some setups, but
admin rights are recommended for a system-wide association).

Usage:
    python register_roman_script.py "C:\\Path\\To\\interpreter.exe"

If no interpreter path is given, it defaults to C:\\Path\\To\\interpreter.exe
(same placeholder as the original .reg file) — edit accordingly.
"""

import sys
import winreg  # only available on Windows


def set_key_default_value(root, path, value):
    """Create (or open) a registry key and set its default (@) value."""
    key = winreg.CreateKey(root, path)
    winreg.SetValueEx(key, None, 0, winreg.REG_SZ, value)
    winreg.CloseKey(key)


def register_roman_script(interpreter_path: str):
    command = f'"{interpreter_path}" "%1"'

    # HKEY_CLASSES_ROOT\.roman -> "RomanScript"
    set_key_default_value(winreg.HKEY_CLASSES_ROOT, r".roman", "RomanScript")

    # HKEY_CLASSES_ROOT\RomanScript -> "Roman Script"
    set_key_default_value(winreg.HKEY_CLASSES_ROOT, r"RomanScript", "Roman Script")

    # HKEY_CLASSES_ROOT\RomanScript\shell (no default value needed)
    winreg.CloseKey(winreg.CreateKey(winreg.HKEY_CLASSES_ROOT, r"RomanScript\shell"))

    # HKEY_CLASSES_ROOT\RomanScript\shell\open (no default value needed)
    winreg.CloseKey(winreg.CreateKey(winreg.HKEY_CLASSES_ROOT, r"RomanScript\shell\open"))

    # HKEY_CLASSES_ROOT\RomanScript\shell\open\command -> "<interpreter>" "%1"
    set_key_default_value(
        winreg.HKEY_CLASSES_ROOT, r"Roman\Roman.exe", command
    )

    print("Registry entries created successfully:")
    print(r"  HKEY_CLASSES_ROOT\.roman = RomanScript")
    print(r"  HKEY_CLASSES_ROOT\RomanScript = Roman Script")
    print(r"  HKEY_CLASSES_ROOT\RomanScript\shell\open\command = " + command)


if __name__ == "__main__":
    if sys.platform != "win32":
        print("This script only works on Windows (it uses the winreg module).")
        sys.exit(1)

    interpreter = sys.argv[1] if len(sys.argv) > 1 else r"C:\Path\To\interpreter.exe"

    try:
        register_roman_script(interpreter)
    except PermissionError:
        print(
            "Permission denied. Try running this script as Administrator "
            "(right-click -> Run as administrator)."
        )
        sys.exit(1)
