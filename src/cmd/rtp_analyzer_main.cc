#include <iostream>

#include "../lib/gcc_sim.h"
#include "../lib/log.h"
#include "../lib/net.h"
#include "../lib/ntp.h"
#include "../lib/pcap_file_reader.h"
#include "../lib/rtcp.h"
#include "../lib/rtp.h"
#include "../lib/stun.h"
#include "../lib/twcc.h"

class rtp_log : public log {

public:
    void write(timeval ts, unsigned ssrc, unsigned pt, unsigned short rtp_seq, unsigned rtp_ts,
               std::optional<unsigned> twcc_seq, std::optional<unsigned> abs_send_time) {

        if (!_line_count++) {
            _fs << "ts_s,ts_us,ssrc,pt,rtp_seq,rtp_ts,trans_cc_seq,trans_cc_send_time_ms" << std::endl;
        }

        if (_fs.is_open()) {
            _fs << ts.tv_sec << "," << ts.tv_usec << "," << ssrc << "," << pt << "," << rtp_seq << ","
                << rtp_ts << "," << (twcc_seq ? std::to_string(*twcc_seq) : "NA") << ","
                << (abs_send_time ? std::to_string(*abs_send_time) : "NA") << std::endl;
        }
    }
};

class rtcp_sr_log : public log {
public:
    void write(timeval ts, unsigned ssrc, unsigned rtp_ts) {

        if (!_line_count++) {
            _fs << "ts_s,ts_us,ssrc,rtp_ts" << std::endl;
        }

        if (_fs.is_open()) {
            _fs << ts.tv_sec << "," << ts.tv_usec << "," << ssrc << rtp_ts << std::endl;
        }
    }
};

class rtcp_rr_log : public log {
public:
    void write(timeval ts, unsigned ssrc, double loss, unsigned jitter, double rtt_ms) {

        if (!_line_count++) {
            _fs << "ts_s,ts_us,ssrc,loss,jitter,rtt_ms" << std::endl;
        }

        if (_fs.is_open()) {
            _fs << ts.tv_sec << "," << ts.tv_usec << "," << loss << "," << jitter << "," << rtt_ms
                << std::endl;
        }
    }
};

class ts_pairs_log : public log {
public:
    void write(unsigned seq, bool rxd, unsigned tx_ts, unsigned rx_ts) {

        if (!_line_count++) {
            _fs << "seq,rxd,tx_ts,rx_ts" << std::endl;
        }

        if (_fs.is_open()) {
            _fs << seq << "," << (rxd ? "1" : "0") << "," << tx_ts << "," << rx_ts << std::endl;
        }
    }
};

class stun_log : public log {
public:
    void write(timeval ts, std::uint16_t msg_type, const std::string& trans_id) {

        if (!_line_count++) {
            _fs << "ts_s,ts_us,msg_type,trans_id" << std::endl;
        }

        if (_fs.is_open()) {
            _fs << std::dec << ts.tv_sec << "," << ts.tv_usec << "," << std::hex << std::setw(4)
                << std::setfill('0') << msg_type << "," << trans_id << std::endl;
        }
    }
};

std::string hex_string_from_bytes(const unsigned char* buf, unsigned len) {

    std::stringstream ss;

    for (auto i = 0; i < len; i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << (unsigned) *(buf + i);

    return ss.str();
}

int main(int argc, char** argv) {

    struct {
        unsigned long processed = 0;
        unsigned long ignored = 0;
    } counters;

    struct {
        rtp_log      rtp;
        rtcp_sr_log  rtcp_sr;
        rtcp_rr_log  rtcp_rr;
        ts_pairs_log ts_pairs;
        stun_log     stun;
    } logs;

    pcap_pkt pkt;
    pcap_file_reader pcap_in("data/webrtc.pcap");

    if (pcap_in.datalink_type() != pcap_link_type::eth
        && pcap_in.datalink_type() != pcap_link_type::null) {
        std::cerr << "error: only ethernet supported right now, exiting." << std::endl;
        exit(1);
    }

    try {
        logs.rtp.open("data/rtp.csv");
        logs.rtcp_sr.open("data/rtcp_sr.csv");
        logs.rtcp_rr.open("data/rtcp_rr.csv");
        logs.ts_pairs.open("data/ts_pairs.csv");
        logs.stun.open("data/stun.csv");
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    gcc_sim gcc;

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

            auto* stun = (stun::hdr*) pl_buf;
            auto trans_id = hex_string_from_bytes(stun->trans_id, 12);

            logs.stun.write(pkt.ts, ntohs(stun->type), trans_id);

        } else if (rtp::contains_rtp(pl_buf, pl_len)) {

            auto* rtp = (rtp::hdr*) pl_buf;

            auto abs_send_time_ms = rtp::get_abs_send_time_ms(rtp, 2);
            auto transport_cc_seq = rtp::get_transport_cc_seq(rtp, 4);

            if (transport_cc_seq && abs_send_time_ms) {
                gcc.add_media_pkt(*transport_cc_seq, *abs_send_time_ms);
            }

            logs.rtp.write(pkt.ts, ntohl(rtp->ssrc), rtp->payload_type(), ntohs(rtp->seq),
                          ntohl(rtp->ts), transport_cc_seq, abs_send_time_ms);

        } else if (rtcp::contains_rtcp(pl_buf, pl_len)) {

            auto* rtcp = (rtcp::hdr*) pl_buf;

            if (rtcp->pt == 200) { // sender report

                auto* sr = (rtcp::sr*) (pl_buf + rtcp::HDR_LEN);
                logs.rtcp_sr.write(pkt.ts, ntohl(rtcp->ssrc), ntohl(sr->rtp_ts));

            } else if (rtcp->pt == 201) { // receiver report

                auto pkt_ntp_ts = ntp::ntp_from_timeval(pkt.ts);

                for (auto i = 0; i < rtcp->recep_rep_count(); i++) {

                    auto* recep_rep = (rtcp::recep_report*) (pl_buf + rtcp::HDR_LEN + i * rtcp::RECEP_REP_LEN);

                    double loss = (double) (ntohl(recep_rep->fraction_lost_cumulative_loss) >> 24) / 256;

                    std::uint32_t a = ntp::middle_bits_from_ntp(pkt_ntp_ts);
                    std::uint32_t dlsr = ntohl(recep_rep->dlsr);
                    std::uint32_t lsr = ntohl(recep_rep->lsr);
                    std::uint32_t rtt = a - dlsr - lsr;
                    auto rtt_ms = ((double)(rtt & 0x0000ffff) / (1 << 16) + (rtt >> 16)) * 1000;

                    unsigned jitter = ntohl(recep_rep->interarrival_jitter);

                    logs.rtcp_rr.write(pkt.ts, ntohl(recep_rep->ssrc), loss, jitter, rtt_ms);
                }

            } else if (rtcp->pt == 205 && rtcp->fmt() == 15) { // transport-cc

                auto* twcc = (rtcp::twcc::hdr*) (pl_buf + rtcp::HDR_LEN);
                auto* twcc_chunks = pl_buf + rtcp::HDR_LEN + rtcp::twcc::HDR_LEN;

                auto pkt_feedback = rtcp::twcc::pkt_feedback(rtcp, twcc, twcc_chunks);

                for (const auto& pkt_fb: pkt_feedback) {
                    gcc.add_twcc_fb(pkt_fb.seq, pkt_fb.received, pkt_fb.timestamp);
                }
            }

        } else {
            counters.ignored++;
            continue;
        }

        counters.processed++;
    }

    for (const auto& p: gcc.data()) {
        logs.ts_pairs.write(p.twcc_seq, p.rxd, p.tx_ts, p.rx_ts);
    }

    std::cout << " - " << counters.processed << " packets processed" << std::endl;
    std::cout << " - " << counters.ignored << " packets ignored" << std::endl;

    return 0;
}
