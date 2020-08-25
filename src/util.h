#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include <vector>
#include <memory>

class Card;

void shuffle ( std::vector<std::shared_ptr<Card>> & cards );

// Trim all spaces and TABs of a string
std::string trim ( const std::string & str );
std::string to_uppercase ( const std::string & str );
std::string to_lowercase ( const std::string & str );
bool are_all_uppercase ( const std::string & str );
bool are_all_lowercase ( const std::string & str );

std::string grammarCorrection ( const std::string & sentence );

#endif
