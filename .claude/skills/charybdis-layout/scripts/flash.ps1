# Wait for an RP2040 in bootloader mode (RPI-RP2 drive), copy the firmware onto it,
# then show what the keyboard reports after it restarts.
param([int]$Minutes = 10)
$uf2 = 'C:\Users\alexa\Documents\vial-qmk\bastardkb_charybdis_3x6_v2_splinky_3_alexandre.uf2'
if (-not (Test-Path $uf2)) { "firmware not built: $uf2"; exit 2 }
$deadline = (Get-Date).AddMinutes($Minutes)
while ((Get-Date) -lt $deadline) {
    $v = Get-Volume | Where-Object { $_.FileSystemLabel -eq 'RPI-RP2' -and $_.DriveLetter }
    if ($v) {
        Start-Sleep -Seconds 1
        $d = "$($v.DriveLetter):\"
        "RPI-RP2 found on $d at $(Get-Date -Format HH:mm:ss), copying"
        Copy-Item $uf2 $d -ErrorAction SilentlyContinue
        Start-Sleep -Seconds 6
        "after flash (expect USB\VID_A8F8&PID_1834\VIAL:...):"
        Get-PnpDevice -PresentOnly | Where-Object { $_.InstanceId -match '^USB\\VID_(2E8A|A8F8)&PID_[0-9A-F]{4}\\' } | Select-Object -ExpandProperty InstanceId
        exit 0
    }
    Start-Sleep -Milliseconds 500
}
"timeout: no RPI-RP2 drive in $Minutes minutes"
exit 1
