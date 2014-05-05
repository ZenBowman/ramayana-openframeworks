#include "colorfilters.h"

namespace MatrixOperations {

RednessFilter::RednessFilter() : RednessFilter(100, 1.25) {}

RednessFilter::RednessFilter(unsigned char _minRed,
                             float _colorDominationFactor)
    : minRed(_minRed), colorDominationFactor(_colorDominationFactor) {}

bool RednessFilter::passesFilter(const Pixel &pixel) {
  // TODO: convert to HSI color space and see if you can improve this
  //return (pixel.hue < 100 && pixel.saturation > 100 and pixel.value > 50 );
  // Note: Didn't work as expected, retry later
  return ((pixel.red > minRed) &&
          (pixel.red > (pixel.blue * colorDominationFactor)) &&
          (pixel.red > (pixel.green * colorDominationFactor)));
}

bool BluenessFilter::passesFilter(const Pixel &pixel) {
  const double colorDominationFactor = 1.2;
  return ((pixel.blue > (pixel.red * colorDominationFactor)) &&
          (pixel.blue > (pixel.green * colorDominationFactor)));
}

bool YellownessFilter::passesFilter(const Pixel &pixel) {
  return ((pixel.red > 200) && (pixel.green > 200) && (pixel.blue < 100));
}

}
