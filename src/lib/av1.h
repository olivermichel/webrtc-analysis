#ifndef WEBRTC_ANALYSIS_AV1_H
#define WEBRTC_ANALYSIS_AV1_H

#include <iostream>
#include <vector>
#include <map>

namespace av1 {

    class dependency_descriptor {

    public:
        enum class dti : unsigned {
            not_present_indication = 0,
            discardable_indication = 1,
            switch_indication      = 2,
            required_indication    = 3
        };

        struct frame_dependency_template {
            unsigned spatial_layer_id  = 0;
            unsigned temporal_layer_id = 0;
            std::vector<dti> dtis = {};
            std::vector<unsigned> fdiffs = {};
        };

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

        [[nodiscard]] const std::map<unsigned, struct frame_dependency_template>& templates() const;

        inline unsigned len() const {
            return _len;
        }

        inline const unsigned char* bytes() const {
            return _buf;
        }

    private:

        void _parse_bytes();
        unsigned _parse_extended_descriptor_fields(const std::vector<bool>& bits, unsigned total_consumed_bits);
        // unsigned _parse_frame_dependency_definition(const std::vector<bool>& bits, unsigned total_consumed_bits);
        unsigned _parse_template_dependency_structure(const std::vector<bool>& bits, unsigned total_consumed_bits);

        unsigned _parse_template_layers(const std::vector<bool>& bits, unsigned i, unsigned& max_spatial_id);
        unsigned _parse_template_dtis(const std::vector<bool>& bits, unsigned it);
        unsigned _parse_template_fdiffs(const std::vector<bool>& bits, unsigned i);

        unsigned _parse_template_chains(const std::vector<bool>& bits, unsigned i, unsigned& chain_cnt);
        unsigned _parse_decode_target_layers(const std::vector<bool>& bits, unsigned i);
        unsigned _parse_render_resolutions(const std::vector<bool>& bits, unsigned i, const unsigned& max_spatial_id);
        unsigned _parse_frame_chains(const std::vector<bool>& bits, unsigned i, unsigned& chain_cnt);
        unsigned _parse_frame_dependency_definition(const std::vector<bool>& bits, unsigned i);

        [[nodiscard]] static std::vector<bool> _bit_vector_from_bytes(const unsigned char* buf, unsigned len);

        const unsigned char* _buf;
        unsigned _len;

        class mandatory_fields _mandatory_fields;

        bool _template_dependency_structure_present_flag = false;
        bool _active_decode_targets_present_flag = false;
        bool _custom_dtis_flag = false;
        bool _custom_fdiffs_flag = false;
        bool _custom_chains_flag = false;

        unsigned _dt_cnt = 0;
        unsigned _template_cnt = 0;
        unsigned _template_id_offset = 0;
        std::uint32_t _active_decode_targets_bitmask = 0;
        bool _resolutions_present_flag = false;

        std::vector<unsigned> _template_spatial_id = {};
        std::vector<unsigned> _template_temporal_id = {};

        std::vector<std::vector<unsigned>> _template_dti = {};

        std::vector<unsigned> _decode_target_spatial_id = {};
        std::vector<unsigned> _decode_target_temporal_id = {};

        std::vector<unsigned> _max_render_width_minus_one = {};
        std::vector<unsigned> _max_render_height_minus_one = {};

        std::map<unsigned, struct frame_dependency_template> _templates;

        static const unsigned MANDATORY_FIELDS_LEN = 3;
    };
}

#endif