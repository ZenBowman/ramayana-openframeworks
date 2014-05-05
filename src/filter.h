#ifndef FILTER_H
#define FILTER_H

namespace MatrixOperations {

struct Pixel {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char hue;
  unsigned char saturation;
  unsigned char value;
};

// Defines a per-pixel predicate filter which can be used to create a mask
class PixelFilter {
public:
  virtual bool passesFilter(const Pixel &pixel) = 0;
  virtual ~PixelFilter() {}
};

}

#endif
