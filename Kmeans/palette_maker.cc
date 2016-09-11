#include "palette_maker.hpp"
#include <vector>
#include <random>
#include <algorithm>
#include <unordered_map>
//Re-include if needed - MSVS 2015 seems to not require it
//#include <cstdlib>
//#include <cstddef>
#include <numeric>

  std::vector<unsigned> Paragon::palette(const std::vector<unsigned>& bitmap, unsigned cluster_count)
  {
    PaletteBox pb(cluster_count);
    for (auto& center : pb.clusters)
      //this CAN return a duplicate index, but it's extremely unlikely
      center = create_random<unsigned>(0, bitmap.size());

    int iterations = 5; //this is messy, just for testing
    do {
      //sorting colors in the bitmap into buckets of cluster centers
      for (const auto& color : bitmap) {
        for (auto index = 0; index < pb.clusters.size(); ++index) {
          pb.distances[index] = findColorDist(color, pb.clusters[index]);
        }
        //get the index of the smallest distance, which corresponds to a color cluster
        ptrdiff_t select = std::min_element(pb.distances.begin(), pb.distances.end()) - pb.distances.begin();
        pb.buckets.insert({ pb.clusters[select], color });
      }

      //moving the cluster centers based on the average value of each bucket
      for (auto& key : pb.clusters) {
        auto range = pb.buckets.equal_range(key);
        key = std::accumulate(range.first, range.second, 0U) / pb.buckets.count(key);
      }
    } while (--iterations);

    return pb.clusters;
  }

  unsigned Paragon::findColorDist(unsigned firstColor, unsigned secondColor)
  {
    int
      rdelta = extractARGB(ARGB::red, firstColor) - extractARGB(ARGB::red, secondColor),
      gdelta = extractARGB(ARGB::green, firstColor) - extractARGB(ARGB::green, secondColor),
      bdelta = extractARGB(ARGB::blue, firstColor) - extractARGB(ARGB::blue, secondColor);

    int signedDist =
      std::sqrt(
      (rdelta * rdelta)
        + (gdelta * gdelta)
        + (bdelta * bdelta)
      );

    return (signedDist < 0) ? signedDist * -1 : signedDist;
  }

  unsigned Paragon::extractARGB(ARGB component, unsigned argb_color)
  {
    return
      ((argb_color >> static_cast<unsigned>(component)) & 0xff);
  }

  Paragon::PaletteBox::PaletteBox(unsigned k_value):
    clusters(k_value), distances(k_value)
  {
    buckets.rehash(k_value);
  }