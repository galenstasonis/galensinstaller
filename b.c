#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(void) {
    char buffer[1024];
    printf("Welcome, \n This program is intended to make your arch installation quicker/easier.\n");
    FILE *fp = popen("lsblk", "r");
    if (fp == NULL) {
        printf("lsblk not found, please install.");
        return 1;
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }
    pclose(fp);
    char diskName[30];
    printf("Please Select the disk you would like to partition (enter n to skip):\n");
    scanf("%s", diskName);
    if (strcmp(diskName, "n") == 0) {
        printf("Skipping disk partitioning.\n");
    } else {
    printf("/dev/%s selected\n", diskName);
    char cfdiskCommand[] = "cfdisk /dev/";
    strcat(cfdiskCommand, diskName);
    int status = system(cfdiskCommand);
    printf("returned to program with status %d\n", status);
        FILE *fd = popen("lsblk", "r");
    if (fd == NULL) {
        printf("lsblk not found, please install.");
        return 1;
    }
    while (fgets(buffer, sizeof(buffer), fd) != NULL) {
        printf("%s", buffer);
    }
    pclose(fd);
    }
    printf("Disk partitioning complete, please select the partition you would like to format for boot (fat32) (enter n to skip):\n");
    char bootPartition[30];
    scanf("%s", bootPartition);
    if (strcmp(bootPartition, "n") == 0) {
        printf("Skipping boot partition formatting.\n");
    } else {
    printf("/dev/%s selected\n", bootPartition);
    char mkfsCommand[50] = "sudo mkfs.fat -F32 /dev/";
    strcat(mkfsCommand, bootPartition);
    int mkfsStatus = system(mkfsCommand);
    printf("returned to program with status %d\n", mkfsStatus);
    }
    printf("Disk formatting complete, please select the partition you would like to format for root (btrfs) (enter n to skip):\n");
    char rootPartition[30];
    scanf("%s", rootPartition);
    if (strcmp(rootPartition, "n") == 0) {
        printf("Skipping root partition formatting.\n");
    } else {
    printf("/dev/%s selected\n", rootPartition);
    char mkfsRootCommand[50] = "sudo mkfs.btrfs /dev/";
    strcat(mkfsRootCommand, rootPartition);
    int mkfsRootStatus = system(mkfsRootCommand);
    printf("returned to program with status %d\n", mkfsRootStatus);
    }
    if (strcmp(rootPartition, "n") == 0) {
        printf("User skipped root partition formatting\n Please enter root partition name");
        scanf("%s", rootPartition);
    }
    if (strcmp(bootPartition, "n") == 0) {
        printf("User skipped boot partition formatting\n Please enter boot partition name");
        scanf("%s", bootPartition);
    }
    system("mkdir /mnt/arch");
    char mountRootCommand[50] = "mount /dev/";
    strcat(mountRootCommand, rootPartition);
    strcat(mountRootCommand, " /mnt/arch"); 
    system(mountRootCommand);
    system("mkdir /mnt/arch/boot");
    char mountBootCommand[50] = "mount /dev/";
    strcat(mountBootCommand, bootPartition);
    strcat(mountBootCommand, " /mnt/arch/boot");
    system(mountBootCommand);
    printf("Root and boot partitions mounted, proceeding to install base packages\n");
    printf("Continue? [y/n]\n");
    char answer[5];
    scanf ("%s", answer);
    if (strcmp(answer, "n") == 0) {
        return 0;
    }
    system("pacstrap -K /mnt/arch base linux linux-firmware vim nano sudo base-devel plasma-desktop sddm networkmanager git reflector btrfs-progs grub efibootmgr os-prober ntfs-3g man-pages man-db texinfo kde-applications plasma-login-manager");
    printf("Base packages installed, proceeding to generate fstab\n");
    system("genfstab -U /mnt/arch >> /mnt/arch/etc/fstab");
    printf("fstab generated");
    // lists regions available
    system("arch-chroot /mnt/arch ls /usr/share/zoneinfo/");
    printf("Please select your timezone:\n");
    // then lists the timezones available in that region
    char timezone[50];
    scanf("%s", timezone);
    char timezoneCommand[100] = "arch-chroot /mnt/arch ls /usr/share/zoneinfo/"; 
    strcat(timezoneCommand, timezone);
    int timezoneStatus = system(timezoneCommand);
    printf("returned to program with status %d\n", timezoneStatus);
    printf("Please select your timezone:\n");
    char timezone2[50];
    scanf("%s", timezone2);  
    if (strstr(timezone, "..") || strstr(timezone2, "..")) {
    printf("Invalid input.\n");
    return 1;
    }
    char tzPath[256];
    snprintf(tzPath, sizeof(tzPath), "/mnt/arch/usr/share/zoneinfo/%s/%s", timezone, timezone2);
    struct stat st;
    if (stat(tzPath, &st) != 0 || !S_ISREG(st.st_mode)) {
        printf("Timezone not found: %s/%s\n", timezone, timezone2);
        return 1;
    } 
    char linkCommand[256];
    snprintf(linkCommand, sizeof(linkCommand),
         "arch-chroot /mnt/arch ln -sf /usr/share/zoneinfo/%s/%s /etc/localtime",
         timezone, timezone2);
    if (system(linkCommand) != 0) return 1;
    if (system("arch-chroot /mnt/arch hwclock --systohc") != 0) return 1;   
    system("arch-chroot /mnt/arch hwclock --systohc");
    printf("Timezone set to %s/%s\n", timezone, timezone2);
    printf("Please enter your hostname:\n");
    char hostname[50];
    scanf("%s", hostname);
    char hostnameCommand[100] = "arch-chroot /mnt/arch echo ";
    strcat(hostnameCommand, hostname);
    strcat(hostnameCommand, " > /etc/hostname");
    int hostnameStatus = system(hostnameCommand);
    printf("returned to program with status %d\n", hostnameStatus);
    printf("Please enter your username:\n");
    char username[50];
    scanf("%s", username);
    char useraddCommand[100] = "arch-chroot /mnt/arch useradd -m -G wheel -s /bin/bash ";
    strcat(useraddCommand, username);
    int useraddStatus = system(useraddCommand);
    printf("returned to program with status %d\n", useraddStatus);
    printf("Please enter your password:\n");
    char password[50];
    scanf("%s", password);
    char passwdCommand[100] = "arch-chroot /mnt/arch bash -c \"echo '";
    strcat(passwdCommand, username);
    strcat(passwdCommand, ":");
    strcat(passwdCommand, password);
    strcat(passwdCommand, "' | chpasswd\"");
    int passwdStatus = system(passwdCommand);
    printf("returned to program with status %d\n", passwdStatus);
    printf("Please enter your root password:\n");
    char rootPassword[50];
    scanf("%s", rootPassword);
    char rootPasswdCommand[100] = "arch-chroot /mnt/arch bash -c \"echo 'root:";
    strcat(rootPasswdCommand, rootPassword);
    strcat(rootPasswdCommand, "' | chpasswd\"");
    int rootPasswdStatus = system(rootPasswdCommand);
    printf("returned to program with status %d\n", rootPasswdStatus);
    system("arch-chroot /mnt/arch sed -i 's/# %wheel ALL=(ALL:ALL) ALL/%wheel ALL=(ALL:ALL) ALL/' /etc/sudoers");
    printf("Please enter your locale (en_US.UTF-8):\n");
    char locale[50];
    scanf("%s", locale);
    char localeCommand[100] = "arch-chroot /mnt/arch sed -i 's/#";
    strcat(localeCommand, locale);
    strcat(localeCommand, " ");
    strcat(localeCommand, locale);
    strcat(localeCommand, "/");
    int localeStatus = system(localeCommand);
    printf("returned to program with status %d\n", localeStatus);
    system("arch-chroot /mnt/arch systemctl enable plasmalogin.service");
    system("arch-chroot /mnt/arch systemctl enable NetworkManager.service");
    system("arch-chroot /mnt/arch grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=GRUB");
    system("arch-chroot /mnt/arch grub-mkconfig -o /boot/grub/grub.cfg");
    printf("Install yay? [y/n]\n");
    char yayAnswer[5];
    scanf("%s", yayAnswer);
    if (strcmp(yayAnswer, "y") == 0) {
        system("arch-chroot /mnt/arch git clone https://aur.archlinux.org/yay.git /tmp/yay");
        system("arch-chroot /mnt/arch chown -R $(whoami):$(whoami) /tmp/yay");
        system("arch-chroot /mnt/arch bash -c \"cd /tmp/yay && makepkg -si --noconfirm\"");
        printf("Yay installed\n");
    }
    else {
        printf("Yay not installed\n");
    }
    printf("Arch installation complete, please reboot and remove the installation media\n");
    return 0;
}