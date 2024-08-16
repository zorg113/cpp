#include "../logreg_classifier.hpp"
#include "../reader.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <fstream>
#include <iostream>

TEST_CASE("Test_CV") {
  std::vector<logreg_classifier> classifiers;
  logreg_classifier::coefs_type coefs;

  std::ifstream coefInput("./../test/logreg_coef.txt");
  while (true) {
    if (!reader::readCoefs(coefInput, coefs))
      break;
    classifiers.emplace_back(logreg_classifier(coefs));
  }
  coefInput.close();
  logreg_classifier::features_type features;
  std::ifstream test_data{"./../test/test_data_logreg.txt"};
  int targetClass;
  double y_pred_expected = 0.0;
  test_data >> y_pred_expected;
  if (reader::readFeaturesTest(test_data, features, targetClass)) {
    std::cout << features.size() << "==\n";
    for (std::size_t i = 0; i < classifiers.size(); i++) {
      auto result = classifiers[i].predict_probability(features);
      REQUIRE(result != Catch::Approx(y_pred_expected).epsilon(1e-12));
    }
  }
}