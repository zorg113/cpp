#include <istream>
#include <vector>

namespace reader {

bool readCoefs(std::istream &stream, std::vector<float> &coefs);
bool readFeatures(std::istream &stream, std::vector<float> &features,
                  int &targetClass);
bool readFeaturesTest(std::istream &stream, std::vector<float> &features,
                      int &targetClass);

} // namespace reader
