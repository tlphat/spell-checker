#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>
#include <vector>

namespace output {

void Error(const std::string& err_message);
void WrongWordsDetected(const std::vector<char*>& spelt_wrong_words);

}

#endif
