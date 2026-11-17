# Galen's Arch Linux installer
I was bored so I decided to learn a bit of C and write this. It sucks really badly.
Notably if you make a typo or happen to live in UTC. Other than that, it works mostly fine. Just installs KDE Plasma and a couple apps I use optionally. also sets up GRUB and users, sudoers, etc.
## How to use
First boot into your Arch Linux install iso, then run
``` 
pacman -Sy wget 
```
in order to download the script.
Then, copy the link to the binary (galensinstaller) from the latest release.
Then run
``` 
wget <binary link here> 
```
make it exectuable with
``` 
chmod +x galensinstaller 
```
and then run it with
``` 
./galensinstaller 
```
and follow the steps.