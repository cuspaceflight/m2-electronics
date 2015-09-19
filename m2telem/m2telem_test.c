#include "m2telem.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    TelemPacket pkt = {.raw={0}};
    memset(pkt.raw, 0x33, 16);
    pkt.i64 = 0xDEADBEEF11223344;
    pkt.timestamp = 0xCAFEBABE;
    pkt.metadata = 0x7E;
    pkt.channel = 0x11;
    pkt.checksum = 0x7D22;

    printf("Made packet:        ");
    size_t i;
    for(i=0; i<16; i++)
        printf("%02X ", pkt.raw[i]);
    printf("\n");

    uint8_t enframe_buf[33];
    memset(enframe_buf, 0x55, 33);
    size_t length = 0;
    m2telem_enframe(&pkt, enframe_buf, &length);

    printf("Enframed packet: ");
    for(i=0; i<length; i++)
        printf("%02X ", enframe_buf[i]);
    printf("|| ");
    for(i=length; i<33; i++)
        printf("%02X ", enframe_buf[i]);
    printf("\n");

    TelemPacket pkt2 = {.raw={0}};
    uint8_t deframe_buf[100] = {0};
    memcpy(&deframe_buf[12], enframe_buf, length);
    memcpy(&deframe_buf[67], enframe_buf, length);
    DeframeState s = {.buf={0}, .idx=0, .s=0};

    bool result = false;
    uint8_t *buf_ptr = deframe_buf;
    int inc = 4;

    while(buf_ptr <= &deframe_buf[99]) {
        result = m2telem_deframe(buf_ptr, inc, &s, &pkt2);
        printf("Decoding, result=%d\n", result);
        printf("Deframer buffer:    ");
        for(i=0; i<32; i++)
            printf("%02X ", s.buf[i]);
        printf("\n");
        buf_ptr += inc;

        if(result) {
            printf("Deframed packet:    ");
            for(i=0; i<16; i++)
                printf("%02X ", pkt2.raw[i]);
            printf("\n");
        }
    }

    return 0;
}
