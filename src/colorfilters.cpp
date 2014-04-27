#include "colorfilters.h"

namespace MatrixOperations {

    RednessFilter::RednessFilter() : RednessFilter(100, 1.2)
    {
    }

    RednessFilter::RednessFilter(unsigned char _minRed, float _colorDominationFactor)
        : minRed(_minRed), colorDominationFactor(_colorDominationFactor)
    {
    }

  bool RednessFilter::passesFilter(const Pixel &pixel) {
    return ((pixel.red > minRed) &&
            (pixel.red > (pixel.blue * colorDominationFactor)) &&
            (pixel.red > (pixel.green * colorDominationFactor)));
  }

  bool BluenessFilter::passesFilter(const Pixel &pixel) {
    const double colorDominationFactor = 1.2;
    return ((pixel.blue > (pixel.red * colorDominationFactor)) &&
            (pixel.blue > (pixel.green * colorDominationFactor)));
  }

}
