
namespace test::data {

    /*
     * Real-time Transport Control Protocol (Generic RTP Feedback)
     *   10.. .... = Version: RFC 1889 Version (2)
     *   ..1. .... = Padding: True
     *   ...0 1111 = RTCP Feedback message type (FMT): Transport-wide Congestion Control (Transport-cc) (15)
     *   Packet type: Generic RTP Feedback (205)
     *   Length: 7 (32 bytes)
     *   Sender SSRC: 0x00000001 (1)
     *   Media source SSRC: 0xdd586eb6 (3713560246)
     *   Transport-cc
     *     Base Sequence Number: 1 (0x0001)
     *     Packet Status Count: 9 (0x0009)
     *     Reference Time: 6197520
     *     Feedback Packets Count: 0 (0x00)
     *     Packet Chunks
     *     Packet Chunk: 8201 (0x2009) [Run Length Chunk] Small Delta. Length : 9
     *     Recv Delta
     *     Recv Delta: 0x1c Small Delta: [seq: 1] 7.000000 ms
     *       Recv Delta: 0x0c Small Delta: [seq: 2] 3.000000 ms
     *       Recv Delta: 0x2c Small Delta: [seq: 3] 11.000000 ms
     *       Recv Delta: 0x38 Small Delta: [seq: 4] 14.000000 ms
     *       Recv Delta: 0x00 Small Delta: [seq: 5] 0.000000 ms
     *       Recv Delta: 0x04 Small Delta: [seq: 6] 1.000000 ms
     *       Recv Delta: 0x00 Small Delta: [seq: 7] 0.000000 ms
     *       Recv Delta: 0x08 Small Delta: [seq: 8] 2.000000 ms
     *       Recv Delta: 0x18 Small Delta: [seq: 9] 6.000000 ms
     *       Recv Delta Padding: 0x0001
     *   [RTCP frame length check: OK - 32 bytes]
     */

    static unsigned char twcc_pkt1[] = {
            0xaf, 0xcd, 0x00, 0x07, 0x00, 0x00, 0x00, 0x01,
            0xdd, 0x58, 0x6e, 0xb6, 0x00, 0x01, 0x00, 0x09,
            0x5e, 0x91, 0x10, 0x00, 0x20, 0x09, 0x1c, 0x0c,
            0x2c, 0x38, 0x00, 0x04, 0x00, 0x08, 0x18, 0x01
    };
}
