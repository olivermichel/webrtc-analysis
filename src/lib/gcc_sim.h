
#ifndef WEBRTC_ANALYSIS_GCC_SIM_H
#define WEBRTC_ANALYSIS_GCC_SIM_H

#include <iostream>

class gcc_sim {

    struct ts_pair {
        std::uint16_t twcc_seq = 0;
        bool rxd = false;
        unsigned tx_ts = 0, rx_ts = 0;
    };

public:

    void add_media_pkt(std::uint16_t twcc_seq, unsigned ts) {

        if (_pkts.empty()) {
            _lowest_twcc_seq = twcc_seq;
        }

        _pkts.push_back({twcc_seq, false, ts, 0});
    }

    void add_twcc_fb(std::uint16_t twcc_seq, bool rxd, unsigned ts) {

        if (twcc_seq >= _lowest_twcc_seq) {

            unsigned idx = twcc_seq - _lowest_twcc_seq;

            if (idx < _pkts.size()) {
                _pkts[idx].rxd = true;
                _pkts[idx].rx_ts = ts;
            }
        }
    }

    [[nodiscard]] const std::vector<ts_pair>& data() const {
        return _pkts;
    }

private:
    std::uint16_t _lowest_twcc_seq;
    std::vector<ts_pair> _pkts;
};

#endif
