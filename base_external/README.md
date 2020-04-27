`post-build.sh` contains the configuration needed to enable wireless functionality on the Raspberry Pi 3B and should replace the original in `/board/raspberrypi/`

The `wpa_supplicant.conf` file in `post-build.sh` contains sensitive information and will not be uploaded to this repo. For further information on how the file should be structured, go to https://www.raspberrypi.org/documentation/configuration/wireless/wireless-cli.md

For buildroot settings to enable wireless netowrking, see https://blog.crysys.hu/2018/06/enabling-wifi-and-converting-the-raspberry-pi-into-a-wifi-ap/
