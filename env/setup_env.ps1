[Environment]::SetEnvironmentVariable("", "", "")

if( get-windowsoptionalfeature -online -featurename "Windows Sub-Systemm For Linux" )
{
  enable-windowsoptionalfeature -online -featurename "Windows Sub-Systemm For Linux" )
  Write-Output "Enabled optional windows feature: 'Windows Sub-System for Linux'"
}
else
{
  Write-Output "The optional feature 'Windows Sub-System For Linux' could not be found on this device"
  Write-Output "Please install Ubuntu-LTS via the microsoft store to and try again..."
  exit 1
}
