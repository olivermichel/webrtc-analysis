
#include "av1.h"
#include "util.h"

av1::dependency_descriptor::mandatory_fields::mandatory_fields(const unsigned char* buf)
    : _buf(buf) { }

bool av1::dependency_descriptor::mandatory_fields::start_of_frame() const {
    return ((_buf[0] >> 7) & 1) == 1;
}

bool av1::dependency_descriptor::mandatory_fields::end_of_frame() const {
    return ((_buf[0] >> 6) & 1) == 1;
}

unsigned av1::dependency_descriptor::mandatory_fields::template_id() const {
    return (_buf[0]) & 0x3f;
}

unsigned av1::dependency_descriptor::mandatory_fields::frame_number() const {
    return (_buf[1] << 8) + _buf[2];
}

av1::dependency_descriptor::dependency_descriptor(const unsigned char* bytes, unsigned len)
    : _buf(bytes), _len(len), _mandatory_fields(bytes) {

    if (len < MANDATORY_FIELDS_LEN)
        throw std::logic_error("av1::dependency_descriptor: len must be at least 3");

    if (len > MANDATORY_FIELDS_LEN)
        _parse_bytes();
}

const class av1::dependency_descriptor::mandatory_fields& av1::dependency_descriptor::mandatory_fields() const {

    return _mandatory_fields;
}

bool av1::dependency_descriptor::template_dependency_structure_present_flag() const {

    return _template_dependency_structure_present_flag;
}

bool av1::dependency_descriptor::active_decode_targets_present_flag() const {

    return _active_decode_targets_present_flag;
}

bool av1::dependency_descriptor::custom_dtis_flag() const {

    return _custom_dtis_flag;
}

bool av1::dependency_descriptor::custom_fdiffs_flag() const {

    return _custom_fdiffs_flag;
}

bool av1::dependency_descriptor::custom_chains_flag() const {

    return _custom_chains_flag;
}

void av1::dependency_descriptor::_parse_bytes() {

    auto bits = _bit_vector_from_bytes(_buf, _len);

    unsigned total_consumed_bits = MANDATORY_FIELDS_LEN * 8;

    total_consumed_bits = _parse_extended_descriptor_fields(bits, total_consumed_bits);
    total_consumed_bits = _parse_dependency_definition(bits, total_consumed_bits);
}

unsigned av1::dependency_descriptor::_parse_extended_descriptor_fields(
        const std::vector<bool>& bits, unsigned total_consumed_bits) {

    auto i = total_consumed_bits;

    _template_dependency_structure_present_flag = bits[i++];
    _active_decode_targets_present_flag = bits[i++];
    _custom_dtis_flag = bits[i++];
    _custom_fdiffs_flag = bits[i++];
    _custom_chains_flag = bits[i++];

    if (_template_dependency_structure_present_flag) {
        i = _parse_template_dependency_structure(bits, i);
    }


    return i;
}

unsigned av1::dependency_descriptor::_parse_dependency_definition(const std::vector<bool>& bits,
                                                                  unsigned total_consumed_bits) {

    auto i = total_consumed_bits;
    return i;
}

unsigned av1::dependency_descriptor::_parse_template_dependency_structure(
        const std::vector<bool>& bits, unsigned total_consumed_bits) {

    auto i = total_consumed_bits;

    auto template_id_offset = util::extract_bits(bits, i, 6);
    i += 6;
    auto dt_cnt_minus_one = util::extract_bits(bits, i, 5);
    i += 5;

    std::cout << "template_id_offset: " << template_id_offset << std::endl;
    std::cout << "dt_cnt_minus_one: " << dt_cnt_minus_one << std::endl;

    auto dt_cnt = dt_cnt_minus_one + 1;

    unsigned max_spatial_id = 0;

    i = _parse_template_layers(bits, i, max_spatial_id);







    return i;
}

unsigned av1::dependency_descriptor::_parse_template_layers(const std::vector<bool>& bits,
                                                            unsigned i, unsigned& max_spatial_id) {
    /*
     * template_layers() {
     *   temporalId = 0
     *   spatialId = 0
     *   TemplateCnt = 0;
     *   MaxTemporalId = 0
     *   do {
     *     TemplateSpatialId[TemplateCnt] = spatialId
     *     TemplateTemporalId[TemplateCnt] = temporalId
     *     TemplateCnt++
     *     next_layer_idc = f(2)
     *     // next_layer_idc == 0 - same sid and tid
     *     if (next_layer_idc == 1) {
     *       temporalId++
     *       if (temporalId > MaxTemporalId) {
     *         MaxTemporalId = temporalId
     *       }
     *     } else if (next_layer_idc == 2) {
     *       temporalId = 0
     *       spatialId++
     *     }
     *   } while (next_layer_idc != 3)
     *   MaxSpatialId = spatialId
     * }
     */

    unsigned temporal_id = 0, spatial_id = 0, template_cnt = 0, max_temporal_id = 0;


}

std::vector<bool> av1::dependency_descriptor::_bit_vector_from_bytes(const unsigned char* buf,
                                                                     unsigned len) {

    std::vector<bool> bits(len * 8);

    for (auto i = 0; i < len; i++)
        for (auto j = 7; j >= 0; j--)
            bits[(i + 1) * 8 - j - 1] = ((buf[i] >> j) & 1);

    return bits;
}
