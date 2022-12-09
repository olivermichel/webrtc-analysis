#ifndef WEBRTC_ANALYSIS_AV1_H
#define WEBRTC_ANALYSIS_AV1_H

#include <iostream>
#include <vector>

namespace av1 {

    class dependency_descriptor {


    public:

        class mandatory_fields {

        public:
            mandatory_fields() = delete;
            explicit mandatory_fields(const unsigned char* buf);
            [[nodiscard]] bool start_of_frame() const;
            [[nodiscard]] bool end_of_frame() const;
            [[nodiscard]] unsigned template_id() const;
            [[nodiscard]] unsigned frame_number() const;

        private:
            const unsigned char* _buf;
        };

        dependency_descriptor() = delete;
        explicit dependency_descriptor(const unsigned char* bytes, unsigned len);

        [[nodiscard]] const mandatory_fields& mandatory_fields() const;

        [[nodiscard]] bool template_dependency_structure_present_flag() const;
        [[nodiscard]] bool active_decode_targets_present_flag() const;
        [[nodiscard]] bool custom_dtis_flag() const;
        [[nodiscard]] bool custom_fdiffs_flag() const;
        [[nodiscard]] bool custom_chains_flag() const;

    private:

        void _parse_bytes();
        unsigned _parse_extended_descriptor_fields(const std::vector<bool>& bits, unsigned total_consumed_bits);
        unsigned _parse_dependency_definition(const std::vector<bool>& bits, unsigned total_consumed_bits);
        unsigned _parse_template_dependency_structure(const std::vector<bool>& bits, unsigned total_consumed_bits);

        unsigned _parse_template_layers(const std::vector<bool>&bits, unsigned i, unsigned& max_spatial_id);

        [[nodiscard]] static std::vector<bool> _bit_vector_from_bytes(const unsigned char* buf, unsigned len);

        const unsigned char* _buf;
        unsigned _len;

        class mandatory_fields _mandatory_fields;

        bool _template_dependency_structure_present_flag;
        bool _active_decode_targets_present_flag;
        bool _custom_dtis_flag;
        bool _custom_fdiffs_flag;
        bool _custom_chains_flag;

        static const unsigned MANDATORY_FIELDS_LEN = 3;
    };
}

#endif