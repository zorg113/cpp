#pragma once

#include "interface_binary_classifier.hpp"

class logreg_classifier : public ibinary_classcifier {
public:
  using coefs_type = features_type;

  logreg_classifier(const coefs_type &coef);

  float predict_probability(const features_type &feat) const override;

protected:
  coefs_type m_coef;
};
