#pragma once

#include <vector>

class ibinary_classcifier {
public:
  using features_type = std::vector<float>;

  virtual ~ibinary_classcifier() {}

  virtual float predict_probability(const features_type &) const = 0;
};
