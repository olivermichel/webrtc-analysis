
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

    /*
     * Real-Time Transport Protocol
     *   10.. .... = Version: RFC 1889 Version (2)
     *   ..0. .... = Padding: False
     *   ...1 .... = Extension: True
     *   .... 0000 = Contributing source identifiers count: 0
     *   0... .... = Marker: False
     *   Payload type: Unassigned (45)
     *   Sequence number: 6751
     *   Timestamp: 3557360969
     *   Synchronization Source identifier: 0xf7b16e83 (4155600515)
     *   Defined by profile: Unknown (0xbede)
     *   Extension length: 3
     *   Header extensions
     *     RFC 5285 Header Extension (One-Byte Header)
     *       Identifier: 2
     *       Length: 3
     *       Extension Data: f34350
     *     RFC 5285 Header Extension (One-Byte Header)
     *       Identifier: 4
     *       Length: 2
     *       Extension Data: 009b
     *     RFC 5285 Header Extension (One-Byte Header)
     *       Identifier: 13
     *       Length: 3
     *       Extension Data: 810045
     */

    static unsigned char rtp_pkt1[] = {
        0x90, 0x2d, 0x1a, 0x5f, 0xd4, 0x09, 0x05, 0x49,
        0xf7, 0xb1, 0x6e, 0x83, 0xbe, 0xde, 0x00, 0x03,

        0x22, 0xf3, 0x43, 0x50, // 4
        0x41, 0x00, 0x9b, // 3
        0xd2,0x81, 0x00, 0x45, // 4
        0x00, // 1 - padding
    };

    /*
     * Real-Time Transport Protocol
     * 10.. .... = Version: RFC 1889 Version (2)
     * ..0. .... = Padding: False
     * ...1 .... = Extension: True
     * .... 0000 = Contributing source identifiers count: 0
     * 1... .... = Marker: True
     * Payload type: Unassigned (45)
     * Sequence number: 6612
     * Timestamp: 3557156759
     * Synchronization Source identifier: 0xf7b16e83 (4155600515)
     * Defined by profile: Unknown (0xbede)
     * Extension length: 8
     * Header extensions
     *   RFC 5285 Header Extension (One-Byte Header)
     *     Identifier: 2
     *     Length: 3
     *     Extension Data: ea432b
     *   RFC 5285 Header Extension (One-Byte Header)
     *     Identifier: 4
     *     Length: 2
     *     Extension Data: 0004
     *   RFC 5285 Header Extension (One-Byte Header)
     *     Identifier: 9
     *     Length: 1
     *     Extension Data: 30
     *   RFC 5285 Header Extension (One-Byte Header)
     *     Identifier: 3
     *     Length: 1
     *     Extension Data: 00
     *   RFC 5285 Header Extension (One-Byte Header)
     *     Identifier: 6
     *     Length: 1
     *     Extension Data: 02
     *   RFC 5285 Header Extension (One-Byte Header)
     *     Identifier: 7
     *     Length: 13
     *     Extension Data: 0100030009000a002300000000
     *   RFC 5285 Header Extension (One-Byte Header)
     *     Identifier: 13
     *     Length: 3
     *     Extension Data: 400001
     */

    static unsigned char rtp_pkt2[] = {
        0x90, 0xad, 0x19, 0xd4, 0xd4, 0x05, 0xe7, 0x97,
        0xf7, 0xb1, 0x6e, 0x83, 0xbe, 0xde, 0x00, 0x08,

        0x22, 0xea, 0x43, 0x2b,

        0x41, 0x00, 0x04,

        0x90,0x30,

        0x30, 0x00,

        0x60, 0x02,

        0x7c, 0x01, 0x00,0x03, 0x00, 0x09, 0x00, 0x0a,
        0x00, 0x23, 0x00,0x00, 0x00, 0x00,

        0xd2, 0x40, 0x00, 0x01,
        0x00 // padding
    };

    /*
     * Real-Time Transport Protocol
     * 10.. .... = Version: RFC 1889 Version (2)
     * ..0. .... = Padding: False
     * ...1 .... = Extension: True
     * .... 0000 = Contributing source identifiers count: 0
     * 0... .... = Marker: False
     * Payload type: Unassigned (45)
     * Sequence number: 25029
     * Timestamp: 2500464622
     * Synchronization Source identifier: 0x8d3814e5 (2369262821)
     * Defined by profile: Unknown (0x1000)
     * Extension length: 17
     * Header extensions
     *   RFC 5285 Header Extension (Two-Byte Header)
     *     Identifier: 2
     *     Length: 3
     *     Extension Data: 03f7ae
     *   RFC 5285 Header Extension (Two-Byte Header)
     *     Identifier: 4
     *     Length: 2
     *     Extension Data: 0002
     *   RFC 5285 Header Extension (Two-Byte Header)
     *     Identifier: 9
     *     Length: 1
     *     Extension Data: 30
     *   RFC 5285 Header Extension (Two-Byte Header)
     *     Identifier: 3
     *     Length: 1
     *     Extension Data: 00
     *   RFC 5285 Header Extension (Two-Byte Header)
     *     Identifier: 6
     *     Length: 1
     *     Extension Data: 02
     *   RFC 5285 Header Extension (Two-Byte Header)
     *     Identifier: 7
     *     Length: 13
     *     Extension Data: 01000100070007000a00000000
     *   RFC 5285 Header Extension (Two-Byte Header)
     *     Identifier: 13
     *     Length: 33
     *     Extension Data: c000018003187aaaf130a0a014d1413823046008642222265009f0077013f00ef0

     */

    static unsigned char rtp_pkt3[] = {
        0x90, 0x2d, 0x61, 0xc5, 0x95, 0x0a, 0x0f, 0xee,
        0x8d, 0x38, 0x14, 0xe5, 0x10, 0x00, 0x00, 0x11,

        // ext 2
        0x02, 0x03, 0x03, 0xf7, 0xae,

        // ext 4
        0x04, 0x02, 0x00, 0x02,

        // ext 9
        0x09, 0x01, 0x30,

        // ext 3
        0x03, 0x01, 0x00,

        // ext 6
        0x06,0x01, 0x02,

        // ext 7
        0x07, 0x0d,0x01, 0x00, 0x01, 0x00, 0x07, 0x00,
        0x07, 0x00, 0x0a, 0x00, 0x00, 0x00,0x00,

        // ext 13
        0x0d, 0x21, 0xc0, 0x00, 0x01, 0x80, 0x03, 0x18,
        0x7a, 0xaa, 0xf1, 0x30, 0xa0, 0xa0, 0x14,0xd1,
        0x41, 0x38, 0x23, 0x04, 0x60, 0x08, 0x64, 0x22,
        0x22, 0x26, 0x50, 0x09, 0xf0, 0x07, 0x70,0x13,
        0xf0, 0x0e, 0xf0
    };


}
