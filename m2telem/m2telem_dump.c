#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <libgen.h>
#include "m2telem.h"

int main(int argc, char** argv)
{
    FILE* infile;
    FILE* outfiles[256] = {0};

    char* infile_bn;

    size_t infile_size;
    size_t npackets;

    uint64_t this_timestamp = 0;
    uint64_t last_timestamp = 0;
    uint64_t t_correction = 0;
    double this_t_s;

    size_t i, j;

    char outname[128];
    char linebuf[256];

    int linebuf_len;

    TelemPacket pkt;

    if(argc != 2) {
        printf("Usage: %s <logfile>", argv[0]);
        return 1;
    }

    infile = fopen(argv[1], "r");
    if(infile == NULL) {
        printf("Error opening log file\n");
        return 1;
    }

    infile_bn = basename(argv[1]);

    fseek(infile, 0, SEEK_END);
    infile_size = ftell(infile);
    npackets = infile_size / 16;
    rewind(infile);

    for(i = 0; i < npackets; i++) {
        j = fread(&pkt, sizeof(TelemPacket), 1, infile);
        if(j != 1) {
            printf("Could not read a complete packet from log\n");
            return 1;
        }
        if(pkt.timestamp < last_timestamp && last_timestamp - pkt.timestamp > 168000000) {
            t_correction += 0xFFFFFFFF;
        }
        last_timestamp = pkt.timestamp;
        this_timestamp = pkt.timestamp + t_correction;
        this_t_s = this_timestamp / 168e6f;

        if(outfiles[pkt.channel] == NULL) {
            sprintf(outname, "%s-%s.csv", infile_bn, m2telem_channel_names[pkt.channel]);
            outfiles[pkt.channel] = fopen(outname, "w");
        }

        linebuf_len = sprintf(linebuf, "%f,%u,%s,", this_t_s, pkt.timestamp,
                              m2telem_origin_names[pkt.metadata & 0x0F]);

        switch(m2telem_channel_formats[pkt.channel]) {
            case M2TELEM_C:
                linebuf_len += sprintf(
                    linebuf+linebuf_len, "%c%c%c%c%c%c%c%c",
                    pkt.c[0], pkt.c[1], pkt.c[2], pkt.c[3],
                    pkt.c[4], pkt.c[5], pkt.c[6], pkt.c[7]
                    );
                break;
            case M2TELEM_I64:
                linebuf_len += sprintf(
                    linebuf+linebuf_len, "%ld", pkt.i64);
                break;
            case M2TELEM_U64:
                linebuf_len += sprintf(
                    linebuf+linebuf_len, "%lu", pkt.u64);
                break;
            case M2TELEM_I32:
                linebuf_len += sprintf(
                    linebuf+linebuf_len, "%d,%d", pkt.i32[0], pkt.i32[1]);
                break;
            case M2TELEM_U32:
                linebuf_len += sprintf(
                    linebuf+linebuf_len, "%u,%u", pkt.u32[0], pkt.u32[1]);
                break;
            case M2TELEM_I16:
                linebuf_len += sprintf(
                    linebuf+linebuf_len, "%d,%d,%d,%d",
                    pkt.i16[0], pkt.i16[1], pkt.i16[2], pkt.i16[3]);
                break;
            case M2TELEM_U16:
                linebuf_len += sprintf(
                    linebuf+linebuf_len, "%u,%u,%u,%u",
                    pkt.u16[0], pkt.u16[1], pkt.u16[2], pkt.u16[3]);
                break;
            case M2TELEM_I8:
                linebuf_len += sprintf(
                    linebuf+linebuf_len, "%d,%d,%d,%d,%d,%d,%d,%d",
                    pkt.i8[0], pkt.i8[1], pkt.i8[2], pkt.i8[3],
                    pkt.i8[4], pkt.i8[5], pkt.i8[6], pkt.i8[7]);
                break;
            case M2TELEM_U8:
                linebuf_len += sprintf(
                    linebuf+linebuf_len, "%u,%u,%u,%u,%u,%u,%u,%u",
                    pkt.u8[0], pkt.u8[1], pkt.u8[2], pkt.u8[3],
                    pkt.u8[4], pkt.u8[5], pkt.u8[6], pkt.u8[7]);
                break;
            case M2TELEM_F:
                linebuf_len += sprintf(
                    linebuf+linebuf_len, "%f,%f", pkt.f[0], pkt.f[1]);
                break;
            case M2TELEM_D:
                linebuf_len += sprintf(
                    linebuf+linebuf_len, "%f", pkt.d);
                break;
        }

        linebuf_len += sprintf(linebuf+linebuf_len, "\n");
        fwrite(linebuf, 1, linebuf_len, outfiles[pkt.channel]);
    }

    for(i=0; i<256; i++) {
        if(outfiles[i] != NULL)
            fclose(outfiles[i]);
    }

    fclose(infile);

    return 0;
}
