#include <iostream>

#include "../lib/net.h"
#include "../lib/pcap_file_reader.h"
#include "../lib/pcap_util.h"
#include "../lib/rtp.h"
#include "../lib/stun.h"

int main(int argc, char** argv) {

    struct {
        unsigned long processed = 0;
        unsigned long ignored = 0;
    } counters;

    pcap_pkt pkt;
    pcap_file_reader pcap_in("data/webrtc.pcap");

    if (pcap_in.datalink_type() != pcap_link_type::eth
        && pcap_in.datalink_type() != pcap_link_type::null) {
        std::cerr << "error: only ethernet supported right now, exiting." << std::endl;
        exit(1);
    }

    while (pcap_in.next(pkt)) {

        unsigned offset = 0;

        if (pcap_in.datalink_type() == pcap_link_type::null) {

            if (pkt.buf[0] != 0x02) {
                counters.ignored++;
                continue;
            }

            offset += 4;

        } else if (pcap_in.datalink_type() == pcap_link_type::eth) {

            auto* eth = (net::eth::hdr*) pkt.buf;

            if ((net::eth::type) eth->ether_type != net::eth::type::ipv4) {
                counters.ignored++;
                continue;
            }

            offset += net::eth::HDR_LEN;
        }

        auto ipv4 = (net::ipv4::hdr*) (pkt.buf + offset);

        if ((net::ipv4::proto) ipv4->next_proto_id != net::ipv4::proto::udp) {
            counters.ignored++;
            continue;
        }

        offset += ipv4->ihl_bytes();

        auto* udp = (net::udp::hdr*) (pkt.buf + offset);

        if ((ntohl(ipv4->src_addr) == 0x0a081d1c && ntohl(ipv4->dst_addr) == 0x0a081d1c
            && ntohs(udp->src_port) == 60522 && ntohs(udp->dst_port) == 62264) ||
            (ntohl(ipv4->src_addr) == 0x0a081d1c && ntohl(ipv4->dst_addr) == 0x0a081d1c
             && ntohs(udp->src_port) == 62264 && ntohs(udp->dst_port) == 60522)) {

        } else {
            counters.ignored++;
            continue;
        }

        offset += net::udp::HDR_LEN;

        auto* pl_buf = (pkt.buf + offset);
        auto pl_len = ntohs(udp->dgram_len);

        if (stun::contains_stun(pl_buf, pl_len)) {
            std::cout << "STUN" << std::endl;
        } else if (rtp::contains_rtp_or_rtcp(pl_buf, pl_len)) {
            std::cout << "RTP/RTCP" << std::endl;
        }

        counters.processed++;

    }

    std::cout << " - " << counters.processed << " packets processed" << std::endl;
    std::cout << " - " << counters.ignored << " packets ignored" << std::endl;

    return 0;
}
