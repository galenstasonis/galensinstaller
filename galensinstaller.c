#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

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
    char parentDisk[64] = "";
    char pkCmd[128];
    snprintf(pkCmd, sizeof(pkCmd), "lsblk -no PKNAME /dev/%s", bootPartition);
    FILE *pk = popen(pkCmd, "r");
    if (pk == NULL || fgets(parentDisk, sizeof(parentDisk), pk) == NULL) {
        printf("Could not determine parent disk of %s\n", bootPartition);
        if (pk) pclose(pk);
        return 1;
    }
    pclose(pk);
    parentDisk[strcspn(parentDisk, "\n")] = '\0';
    char partFile[128];
    snprintf(partFile, sizeof(partFile), "/sys/class/block/%s/partition", bootPartition);
    int partNum;
    FILE *pf = fopen(partFile, "r");
    if (pf == NULL || fscanf(pf, "%d", &partNum) != 1) {
        printf("Could not determine partition number for %s\n", bootPartition);
        if (pf) fclose(pf);
        return 1;
    }
    fclose(pf);

    char efiTypeCmd[256];
    snprintf(efiTypeCmd, sizeof(efiTypeCmd), "sgdisk -t %d:ef00 /dev/%s", partNum, parentDisk);
    if (system(efiTypeCmd) != 0) {
    printf("Failed to set EFI System partition type\n");
    return 1;
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
    system("pacstrap -K /mnt/arch base linux linux-firmware vim nano sudo base-devel plasma-desktop sddm networkmanager git fastfetch reflector btrfs-progs grub efibootmgr os-prober ntfs-3g man-pages man-db texinfo kde-applications plasma-login-manager");
    printf("Base packages installed, proceeding to generate fstab\n");
    system("genfstab -U /mnt/arch >> /mnt/arch/etc/fstab");
    printf("fstab generated\n");
    // lists regions available
    system("arch-chroot /mnt/arch ls /usr/share/zoneinfo/");
    tcflush(STDIN_FILENO, TCIFLUSH);
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
    system("arch-chroot /mnt/arch grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=GRUB --removable");
    system("arch-chroot /mnt/arch grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=GRUB ");
    system("arch-chroot /mnt/arch grub-mkconfig -o /boot/grub/grub.cfg");
    printf("Enable multilib repository? [y/n]\n");
    char multilibAnswer[5];
    scanf("%s", multilibAnswer);
    if (strcmp(multilibAnswer, "y") == 0) {
        system("sed -i '/^#\\[multilib\\]/,/^#Include/ s/^#//' /mnt/arch/etc/pacman.conf");
        system("arch-chroot /mnt/arch pacman -Sy");
        printf("Multilib repository enabled\n");
    }   
    else {
        printf("Multilib repository not enabled\n");
    }
    printf("(Discord, Spotify, Strawberry, Steam, Spotify (flatpak ), Sober (flatpak ), Firefox)\n Install optional packages? [y/n]\n");
    char optionalAnswer[5];
    scanf("%s", optionalAnswer);
    if (strcmp(optionalAnswer, "y") == 0) {
        system("arch-chroot /mnt/arch pacman -S --noconfirm discord strawberry firefox");
        system("arch-chroot /mnt/arch pacman -S steam");
        system("arch-chroot /mnt/arch pacman -S flatpak");
        system("arch-chroot /mnt/arch flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo");
        system("arch-chroot /mnt/arch flatpak install flathub com.spotify.Client -y");
        system("arch-chroot /mnt/arch flatpak install flathub org.vinegarhq.Sober -y");
        printf("Optional packages installed\n");
    }
    else {
        printf("Optional packages not installed\n");
    }  
    printf("Arch installation complete, please reboot and remove the installation media\n");
    return 0;
}