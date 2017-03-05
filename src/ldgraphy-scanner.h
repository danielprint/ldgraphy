/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil; -*-
 * (c) 2017 Henner Zeller <h.zeller@acm.org>
 *
 * This file is part of LDGraphy http://github.com/hzeller/ldgraphy
 *
 * LDGraphy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LDGraphy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LDGraphy.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LDGRAPHY_IMAGE_SCANNER_H
#define LDGRAPHY_IMAGE_SCANNER_H

class ScanLineSender;
class BitmapImage;
class SimpleImage;

#include <memory>
#include <functional>

// Laser Lithography scanner facade taking an image and operating the machinery
// to expose it by scanning.
class LDGraphyScanner {
public:
    ~LDGraphyScanner();

    // Create an image scanner. Takes ownership of the ScanLineSender
    LDGraphyScanner(ScanLineSender *sink);

    // Set new image. Image is a grayscale image that needs to already
    // be quantized to black/white.
    // Takes ownership of the image.
    void SetImage(SimpleImage *img, float dpi);

    // Give an estimation how long a ScanExpose() would take with current image.
    float estimated_time_seconds() const;

    // Scan expose the image until done or progress_out() returns false.
    // If do_move is false, then do not move sled.
    //
    // The "progress_out" callback is called with the current state of how
    // much is done from all. This progress_out() callback should return a
    // boolean indicating if we should continue.
    void ScanExpose(bool do_move,
                    std::function<bool(int done, int total)> progress_out);

private:
    std::unique_ptr<ScanLineSender> const backend_;
    std::unique_ptr<BitmapImage> scan_image_;  // preprocessed.
    int scanlines_;
    float sled_step_per_image_pixel_;
};

#endif  // LDGRAPHY_IMAGE_SCANNER_H