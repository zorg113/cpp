#include <fstream>
#include <iostream>


#include "logreg_classifier.hpp"
#include "reader.hpp"


int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: fashion_mnist <input_csv> <logreg_coef>" << std::endl;
    return 1;
  }

  std::vector<logreg_classifier> classifiers;
  logreg_classifier::coefs_type coefs;

  std::ifstream coefInput(argv[2]);
  while (true) {
    if (!reader::readCoefs(coefInput, coefs))
      break;

    classifiers.emplace_back(logreg_classifier(coefs));
  }
  coefInput.close();

  std::ifstream dataInput(argv[1]);

  logreg_classifier::features_type features;
  int targetClass;
  int totalCount = 0;
  int rightAnswersCount = 0;

  while (true) {
    if (!reader::readFeatures(dataInput, features, targetClass))
      break;

    totalCount++;

    float maxResult = -1;
    int maxResultClass = 0;

    for (std::size_t i = 0; i < classifiers.size(); i++) {
      auto result = classifiers[i].predict_probability(features);
      if (result > maxResult) {
        maxResult = result;
        maxResultClass = i;
      }
    }

    if (maxResultClass == targetClass)
      rightAnswersCount++;
  }

  float accuracy = 0;
  if (totalCount > 0) {
    accuracy = float(rightAnswersCount) / totalCount;
  }

  std::cout << accuracy << std::endl;

  return 0;
}
