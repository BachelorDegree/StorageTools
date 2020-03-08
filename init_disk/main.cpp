#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "SliceId.hpp"
int cluster, machine, disk;
char device[128];
Storage::SliceId msid;

struct chunk_header
{
    char magic[16];
    int32_t version;
    uint64_t chunk_id;
    uint32_t current_inode;
    uint32_t logical_used_space;
    uint32_t actual_used_space;
    chunk_header(uint64_t chunk_id):
        version(1), chunk_id(chunk_id),
        current_inode(0), logical_used_space(0), actual_used_space(0)
    {
        strcpy(magic, "ALOHA");
    }
};

void do_input(void)
{
    puts("===== DISK INITIALIZER =====");
    printf("%s", "Cluster ID: ");
    scanf("%d", &cluster);
    printf("%s", "Machine ID: ");
    scanf("%d", &machine);
    printf("%s", "Disk    ID: ");
    scanf("%d", &disk);
    printf("%s", "Device (/dev/sda): ");
    scanf("%s", device);
    puts("========= END DATA =========");
    printf("Cluster: %d, Machine: %d, Disk: %d\n", cluster, machine, disk);
    msid.SetCluster(cluster);
    msid.SetMachine(machine);
    msid.SetDisk(disk);
    printf("Device: %s\nSlice ID for this machine is 0x%016lx\n", device, msid.UInt());
    printf("%s", "Confirm? [input \"YES\"] ");
    char buf[128];
    scanf("%s", buf);
    if (strcmp(buf, "YES") != 0)
        exit(0);
}

void do_init(void)
{
    int fd = open(device, O_RDWR);
    if (fd == -1)
    {
        puts("open() failed");
        return;
    }
    auto disk_size = lseek(fd, 0, SEEK_END);
    if (disk_size == -1)
    {
        puts("lseek() failed");
        return;
    }
    constexpr auto chunk_size = 1LU << 31; // 2 GiB
    auto chunk_count = disk_size / chunk_size;
    printf("Disk %s %lu bytes, contains %lu chunks\n", device, disk_size, chunk_count);

    for (decltype(chunk_count) chunk_i = 0; chunk_i < chunk_count; ++chunk_i)
    {
        auto base_offset = chunk_i * chunk_size;
        auto csid = msid;
        csid.SetChunk(chunk_i);
        chunk_header ch(csid.UInt());
        printf("version: %d, hex: 0x%016lx, cur_inode: %u, l_used: %u, a_used: %u(4k aligned)\n", 
            ch.version, ch.chunk_id, ch.current_inode, ch.logical_used_space, ch.actual_used_space);
        pwrite64(fd, &ch, sizeof(ch), base_offset + 0); // header
    }
}

int main(int argc, char *argv[])
{
    do_input();
    do_init();
}