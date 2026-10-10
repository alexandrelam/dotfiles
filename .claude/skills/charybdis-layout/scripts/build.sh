# Build the Charybdis Mini Vial firmware from the dotfiles keymap.
# Run inside QMK MSYS: MSYSTEM=MINGW64 C:\QMK_MSYS\usr\bin\bash.exe -l <this file>
# -j: compile on all CPU cores. cp -p: keep file dates so make only rebuilds what changed.
cd /c/Users/alexa/Documents/vial-qmk || exit 1
rm -rf keyboards/bastardkb/charybdis/3x6/keymaps/alexandre
cp -rp /c/Users/alexa/Documents/dotfiles/charybdis-mini/vial keyboards/bastardkb/charybdis/3x6/keymaps/alexandre
make -j"$(nproc)" bastardkb/charybdis/3x6/v2/splinky_3:alexandre > build.log 2>&1
status=$?
grep -iE "error|warning:|\[ERRORS\]|\[WARNINGS\]" build.log | tail -20
tail -2 build.log
rm -f build.log
ls -la bastardkb_charybdis_3x6_v2_splinky_3_alexandre.uf2
exit $status
