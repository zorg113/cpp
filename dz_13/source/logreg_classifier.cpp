#include "logreg_classifier.hpp"

#include <cassert>
#include <cmath>
#include <numeric>
#include <stdexcept>

namespace {

template <typename T> auto sigma(T x) { return 1 / (1 + std::exp(-x)); }

} // namespace

logreg_classifier::logreg_classifier(const coefs_type &coef) : m_coef(coef) {
  assert(!m_coef.empty());
}

float logreg_classifier::  predict_probability(const features_type &feat) const {
  if (feat.size() + 1 != m_coef.size())
    throw std::runtime_error("Feature vector size mismatch");

  auto z = std::inner_product(feat.begin(), feat.end(), ++m_coef.begin(),
                              m_coef.front());
  return sigma(z);
}
