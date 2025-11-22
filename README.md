$tempDir = Join-Path $env:TEMP 'chromeinstall'
New-Item -ItemType Directory -Force -Path $tempDir | Out-Null
$installerPath = Join-Path $tempDir 'chrome_installer.exe'
Invoke-WebRequest -Uri 'http://dl.google.com/chrome/install/latest/chrome_installer.exe' -OutFile $installerPath
Start-Process -FilePath $installerPath -ArgumentList '/silent /install' -Wait
Remove-Item -Path $tempDir -Recurse -Force
