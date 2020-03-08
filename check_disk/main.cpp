#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "SliceId.hpp"

struct chunk_header
{
    char magic[16];
    int32_t version;
    uint64_t chunk_id;
    uint32_t next_inode;
    uint32_t logical_used_space;
    uint32_t actual_used_space;
};

void do_check(char *device)
{
    printf("Checking %s\n", device);
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
    for (decltype(chunk_count) chunk_i = 0; chunk_i < chunk_count; ++chunk_i)
    {
        auto base_offset = chunk_i * chunk_size;
        printf("Chunk #%04lu at offset 0x%016lx ", chunk_i, base_offset);
        chunk_header ch;
        pread64(fd, &ch, sizeof(ch), base_offset + 0);
        if (strcmp(ch.magic, "ALOHA") != 0)
        {
            puts("magic number not correct");
            continue;
        }
        printf("version: %d, hex: 0x%016lx, next_inode: %u, l_used: %u, a_used: %u(4k aligned)\n", 
            ch.version, ch.chunk_id, ch.next_inode, ch.logical_used_space, ch.actual_used_space);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("USAGE: %s <device_path>\n", argv[0]);
        return 0;
    }
    do_check(argv[1]);
}