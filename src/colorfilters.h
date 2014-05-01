#ifndef COLOR_FILTERS_H
#define COLOR_FILTERS_H

#include "filter.h"

namespace MatrixOperations {

  class RednessFilter : public PixelFilter {
  public:
    RednessFilter(unsigned char _minRed, float _colorDominationFactor);
    RednessFilter();

    unsigned char minRed;
    float colorDominationFactor;
    bool passesFilter(const Pixel &pixel) override;
  };

  class YellownessFilter : public PixelFilter {
  public:
    bool passesFilter(const Pixel &pixel) override;
  };


  class BluenessFilter : public PixelFilter {
  public:
    bool passesFilter(const Pixel &pixel) override;
  };
}

#endif
