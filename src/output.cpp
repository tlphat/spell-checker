#include "output.h"

#include <iostream>

namespace output {

void Error(const std::string& err_message) {
    std::cerr << err_message << std::endl;
}

void WrongWordsDetected(const std::vector<char*>& spelt_wrong_words) {
    if (spelt_wrong_words.empty()) {
        std::cout << "All words are spelt correctly." << std::endl;
        return;
    }

    std::cout << "These words are spelt wrong:" << std::endl;
    for (char* word : spelt_wrong_words) {
        std::cout << " " << word << std::endl;
    }
}

}
