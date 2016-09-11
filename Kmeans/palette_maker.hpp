#ifndef PALETTE_MAKER_HPP
#define PALETTE_MAKER_HPP

#include <vector>
#include <random>
#include <unordered_map>

namespace Paragon
{
  //constants and enumerations

  enum class ARGB : unsigned
  {
    blue = 0,
    green = 8,
    red = 16,
    alpha = 24,
  };

  //structs and classes

  struct PaletteBox
  {
    std::vector<unsigned>
      clusters,   //randomized points to form pixel cluster
      distances;  //distances between a color and corresponding cluster centers

    std::unordered_multimap<unsigned/*key: cluster*/, unsigned/*values: color pool*/>
      buckets;

    PaletteBox(unsigned k_value);
  };

  //functions

  std::vector<unsigned> palette(const std::vector<unsigned>& bitmap, unsigned cluster_count);

  unsigned findColorDist(unsigned firstColor, unsigned secondColor);

  unsigned extractARGB(ARGB component, unsigned argb_color);


  //template functions and methods

  //change this to not create new objects on every call; Try static locals?
  template <typename Integral>
  Integral create_random(Integral inclusiveLower, Integral inclusiveUpper)
  {
    static std::random_device rd;
    static std::mt19937_64 mersenneGen(rd());
    static std::uniform_int_distribution<Integral> distribute(inclusiveLower, inclusiveUpper);
    return distribute(mersenneGen);
  }

}

#endif