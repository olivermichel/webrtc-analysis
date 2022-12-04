#include <iostream>
#include <fstream>

#include "../lib/net.h"
#include "../lib/pcap_file_reader.h"
#include "../lib/rtcp.h"
#include "../lib/rtp.h"
#include "../lib/stun.h"
#include "../lib/twcc.h"

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

    std::ofstream rtp_out("data/rtp.csv");

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

            // std::cout << "stun" << std::endl;

        } else if (rtp::contains_rtp(pl_buf, pl_len)) {


            auto* rtp = (rtp::hdr*) pl_buf;
            auto abs_send_time = rtp::get_abs_send_time(rtp, 2);
            auto transport_cc_seq = rtp::get_transport_cc_seq(rtp, 4);

            rtp_out << pkt.ts.tv_sec << "," << pkt.ts.tv_usec << "," << ntohl(rtp->ssrc) << ","
                    << rtp->payload_type() << "," << ntohs(rtp->seq) << "," << ntohl(rtp->ts) << ","
                    << (transport_cc_seq ? std::to_string(*transport_cc_seq) : "NA") << ","
                    << (abs_send_time ? std::to_string(*abs_send_time) : "NA") << std::endl;

        } else if (rtcp::contains_rtcp(pl_buf, pl_len)) {

            auto* rtcp = (rtcp::hdr*) pl_buf;

            if (rtcp->pt == 200) { // sender report

                /*
                std::cout << "rtcp: sr" << std::endl;

                // auto* sr = (rtcp::sr*) (pl_buf + rtcp::HDR_LEN);

                std::cout << " - report from: 0x" << std::hex << std::setw(8)
                          << std::setfill('0') << ntohl(rtcp->ssrc) << std::dec << std::endl;
                */

            } else if (rtcp->pt == 201) { // receiver report

                /*
                std::cout << "rtcp: rr" << std::endl;

                for (auto i = 0; i < rtcp->recep_rep_count(); i++) {

                    auto* recep_rep = (rtcp::recep_report*) (pl_buf + rtcp::HDR_LEN + i * rtcp::RECEP_REP_LEN);

                    std::cout << " - report for: 0x" << std::hex << std::setw(8)
                              << std::setfill('0') << ntohl(recep_rep->ssrc) << std::dec << std::endl;
                }
                */

            } else if (rtcp->pt == 205 && rtcp->fmt() == 15) { // transport-cc
                /*
                std::cout << "rtcp: transport-cc" << std::endl;

                auto* twcc = (rtcp::twcc::hdr*) (pl_buf + rtcp::HDR_LEN);
                std::cout << " - base_seq: " << ntohs(twcc->base_seq) << std::endl;
                std::cout << " - ref_time: " << std::dec << twcc->ref_time() * 64 << std::endl;
                */
            }

        } else {
            counters.ignored++;
            continue;
        }

        counters.processed++;
    }

    rtp_out.close();

    std::cout << " - " << counters.processed << " packets processed" << std::endl;
    std::cout << " - " << counters.ignored << " packets ignored" << std::endl;

    return 0;
}
