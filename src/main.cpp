#include <cstdint>
#include <iostream>

#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "qrcodegen.hpp"
namespace qr = qrcodegen;

const int MAX_QR_SIZE = 2953;
const uint8_t BLACK = 0;
const uint8_t WHITE = 255;

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: drawQR <text to encode in QR> <output file path/name> "
               "<size in px>");
        exit(0);
    }
    std::string qr_text = argv[1];
    std::string output_file = argv[2];
    int size = std::stoi(argv[3]);

    std::vector<qr::QrSegment> qr_segments =
        qr::QrSegment::makeSegments(qr_text.c_str());
    qr::QrCode qr = qr::QrCode::encodeSegments(
        qr_segments, qr::QrCode::Ecc::MEDIUM, qr::QrCode::MIN_VERSION,
        qr::QrCode::MAX_VERSION, -1, true);

    /*
        At this point we have

        1. An image of size x size
        2. A qr code of qr.getSize() x qr.getSize()

        so we should redraw a module for size / qr.getSize() pixels basically
    */

    const int qr_size = qr.getSize();
    const float pixels_per_module = static_cast<float>(size) / qr_size;

    std::vector<uint8_t> pixels_flattened(size * size);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            // Get the module associated with this range of pixels
            bool cur_module_color =
                qr.getModule(x / pixels_per_module, y / pixels_per_module);
            pixels_flattened[y * size + x] = cur_module_color ? BLACK : WHITE;
        }
    }

    // The *data pointer points to the first byte of the top-left-most pixel.
    const int NUM_CHANNELS = 1;
    int rc = stbi_write_png(output_file.c_str(), size, size, NUM_CHANNELS,
                            pixels_flattened.data(), size * NUM_CHANNELS);
    if (rc == 0) {
        printf("stbi_write_png failed!\n");
        exit(1);
    }
}
