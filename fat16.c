#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    unsigned char jmp[3];
    char oem[8];
    uint16_t sector_size;
    unsigned char sectors_per_cluster;
    uint16_t reserved_sectors;
    unsigned char number_of_fats;
    uint16_t root_dir_entries;
    uint16_t total_sectors_short; // if zero, later field is used
    unsigned char media_descriptor;
    uint16_t fat_size_sectors;
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_long;

    unsigned char drive_number;
    unsigned char current_head;
    unsigned char boot_signature;
    uint32_t volume_id;
    char volume_label[11];
    char fs_type[8];
    char boot_code[448];
    uint16_t boot_sector_signature;
} __attribute((packed)) Fat16BootSector;

int main(int argc, char *argv[]) {
    Fat16BootSector bs;

    uint32_t num_sectors = 0;
    uint16_t num_sectors_short = 0;
    uint32_t num_sectors_long = 0;

    if (num_sectors < 65536) {
        num_sec
    } else {
    }


    bs.jmp = { 0xeb, 0x3c, 0x90 };
    memcpy((char *)bs.oem, "cgi", strlen("cgi"));
    bs.sector_size = 512;
    bs.sectors_per_cluster = 4;
    bs.reserved_sectors = 1;
    bs.number_of_fats = 2;
    bs.root_dir_entries = 512;
    bs.total_sectors_short = 



    printf("length = %ld\n", sizeof bs);
    printf("sector_size = %d\n", bs.sector_size);
    printf("sectors_per_cluster = %d\n", bs.sectors_per_cluster);
    printf("reserved_sectors = %d\n", bs.reserved_sectors);
    printf("number_of_fats = %d\n", bs.number_of_fats);
    printf("root_dir_entries = %d\n", bs.root_dir_entries);
    printf("total_sectors_short = %d\n", bs.total_sectors_short);
    printf("media_descriptor = %d\n", bs.media_descriptor);
    printf("fat_size_sectors = %d\n", bs.fat_size_sectors);
    printf("sectors_per_track = %d\n", bs.sectors_per_track);
    printf("number_of_heads = %d\n", bs.number_of_heads);
    printf("hidden_sectors = %d\n", bs.hidden_sectors);
    printf("total_sectors_long = %d\n", bs.total_sectors_long);
    printf("drive_number = %d\n", bs.drive_number);
    printf("current_head = %d\n", bs.current_head);
    printf("boot_signature = %d\n", bs.boot_signature);
    printf("volume_id = %x\n", bs.volume_id);
/*    printf("volume_label = %s\n", bs.volume_label);*/
/*    printf("fs_type = %s\n", bs.fs_type);*/
/*    printf("boot_code = %s\n", bs.boot_code);*/
    printf("boot_sector_signature = 0x%x\n", bs.boot_sector_signature);

    return 0;
}
