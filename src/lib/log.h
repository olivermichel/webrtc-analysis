#ifndef WEBRTC_ANALYSIS_LOG_H
#define WEBRTC_ANALYSIS_LOG_H

#include <fstream>
#include <string>

class log {

public:

    const std::string HEADER;

    log() = default;

    void open(const std::string& file_name) {

        _fs.open(file_name);

        if (!_fs.is_open()) {
            throw std::runtime_error("csv_log: could not open " + file_name);
        }
    }

    virtual ~log() {

        if (_fs.is_open()) _fs.close();
    }

protected:
    std::ofstream _fs;
    unsigned long _line_count = 0;
};

#endif