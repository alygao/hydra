#include <algorithm>
#include <random>
#include <ctime>
#include <string>
#include <string_view>
#include "util.h"

static const std::string SPACE_AND_TAB = " \t";

static const char UPPERCASE_A = 'A';
static const char LOWERCASE_A = 'a';
static const char UPPERCASE_Z = 'Z';
static const char LOWERCASE_Z = 'z';
static const char NUMBER_0 = '0';
static const char NUMBER_9 = '9';

static const int VOWELS_SIZE = 5;
static const std::string VOWELS = "aeiou";

static const int VOWELS_IN_ABBR_PREFIXES_SIZE = 6;
static const std::string VOWELS_IN_ABBR_PREFIXES = "AEIOUM";

static const int VOWEL_SOUND_WORDS_EXCEPTIONS_SIZE = 1;
static const std::string VOWEL_SOUND_WORDS_EXCEPTIONS[] = { "uni" };

static const int CONSONANT_SOUND_WORDS_EXCEPTIONS_SIZE = 2;
static const std::string CONSONANT_SOUND_WORDS_EXCEPTIONS[] = { "hon", "hour" };


void shuffle ( std::vector<std::shared_ptr<Card>> & cards ) {
    std::shuffle (cards.begin(), cards.end(), std::default_random_engine(std::time(0)));
}

// Trim all spaces and TABs of a string
std::string trim ( const std::string & str ) {
    const auto pos1 = str.find_first_not_of ( SPACE_AND_TAB );
    if ( pos1 == std::string::npos ) {
        return "";
    }
    const auto pos2 = str.find_last_not_of ( SPACE_AND_TAB );
    return str.substr ( pos1, pos2 - pos1 + 1);
}

static const std::string WORD_ENDING_CHARS = " .,\"':()!";

static bool isWordEndChar ( const char ch ) {
    for ( unsigned int i = 0; i < WORD_ENDING_CHARS.size(); i ++ ) {
        if ( ch == WORD_ENDING_CHARS[i] ) {
            return true;
        }
    }
    return false;
}

static std::string trimPureWord ( const std::string & str ) {
    const auto pos1 = str.find_first_not_of ( WORD_ENDING_CHARS );
    if ( pos1 == std::string::npos ) {
        return "";
    }
    const auto pos2 = str.find_last_not_of ( WORD_ENDING_CHARS );
    return str.substr ( pos1, pos2 - pos1 + 1);
}


static size_t findNextWord ( const std::string & sentence, const size_t startPosition ) {

    for ( unsigned int pos = startPosition; pos < sentence.size(); pos ++ ) {
        if ( isWordEndChar ( sentence[pos] ) ) {
            return pos + 1;
        }
    }
    return std::string::npos;
}

bool are_all_uppercase ( const std::string & str ) {
    for ( auto  c : str ) {
        if ( c < UPPERCASE_A || c > UPPERCASE_Z ) {
            return false;
        }
    }
    return true;
}

bool are_all_lowercase ( const std::string & str ) {
    for ( auto  c : str ) {
        if ( c < LOWERCASE_A || c > LOWERCASE_Z ) {
            return false;
        }
    }
    return true;
}

std::string to_uppercase ( const std::string & str ) {
    std::string result = "";
    for ( auto  c : str ) {
        if ( c >= LOWERCASE_A && c <= LOWERCASE_Z ) {
            c += (UPPERCASE_A - LOWERCASE_A);
        }
        result += c;
    }
    return result;
}

std::string to_lowercase ( const std::string & str ) {
    std::string result = "";
    for ( auto  c : str ) { 
        if ( c >= UPPERCASE_A && c <= UPPERCASE_Z ) {
            c += (LOWERCASE_A - UPPERCASE_A);
        }
        result += c;
    }
    return result;
}

static std::string translateNumberToWords ( std::string digits ) {
    try {
        int number = std::stoi ( digits );
        if ( number == 1 ) {
            return "one";
        } else if ( number == 8 ) {
            return "eight";
        } else if ( number == 18 ) {
            return "eighteen";
        } else if ( number >= 80 && number <= 89 ) {
            return "eighty-xxx";
        } else if ( number >= 100 && number <= 199 ) {
            return "one hundreds-xxx";
        } else if ( number >= 800 && number <= 899 ) {
            return "eight hundreds-xxx";
        } else {
            return "xxx";
        }
    } catch (std::invalid_argument &) {
        return "";
    }
}

// helper method simply convert the number prefix, only for checking vowel sounds purpose.
// It only words for less than 3-digit. It only converts:
//   1 - one
//   8 - eight
//   18 - eighteen
//   80-89 - eighty-xxx
//   100-199 - one hundreds-xxx
//   800-899 - eight hundreds-xxx
//   rest - xxx
static std::string convertNumbersPrefixToWords ( std::string word ) {
    bool toScan = true;
    std::string result = "";
    std::string digits = "";
    for ( auto c : word ) {
        if ( toScan ) {
            if ( c >= NUMBER_0 && c <= NUMBER_9 ) {
                digits += c;
            } else { // the number terminates
                if ( digits.size() > 0 ) {
                    result += translateNumberToWords ( digits );
                    digits = "";
                }
                result += c;
                toScan = false;
            }
        } else {
            result += c;
        }
    }

    return result + translateNumberToWords ( digits );
}

// Invariant: word must purely trimmed.
static bool isVowelSoundingWord ( std::string word ) {

    if ( word.size() == 0 ) return false;   // return false for blank word
    
    if ( are_all_uppercase ( word ) ) { // Abbreviation
        for ( int i = 0; i < VOWELS_IN_ABBR_PREFIXES_SIZE; i ++ ) {
            if ( word[0] == VOWELS_IN_ABBR_PREFIXES[i] ) {
                return true;
            }
        }
        return false;
    } else {    // not an abbreviation
        word = convertNumbersPrefixToWords ( word );
        // (1) check for vowel exceptions
        for ( int i = 0; i < VOWEL_SOUND_WORDS_EXCEPTIONS_SIZE; i ++ ) {
            if ( word.find ( VOWEL_SOUND_WORDS_EXCEPTIONS[i]) == 0 ) {
                return false;
            }
        }
        // (2) check for vowel
        for ( int i = 0; i < VOWELS_SIZE; i ++ ) {
            if ( word[0] == VOWELS[i] ) {
                return true;
            }
        }
        // (3) check for consonant exceptions
        for ( int i = 0; i < CONSONANT_SOUND_WORDS_EXCEPTIONS_SIZE; i ++ ) {
            if ( word.find ( CONSONANT_SOUND_WORDS_EXCEPTIONS[i]) == 0 ) {
                return true;
            }
        }
        return false;
    }
}

std::string grammarCorrection ( const std::string & sentence ) {
    std::string result = "";
    size_t pos = 0;
    size_t previousPos = pos;
    int indefiniteArticleFound = 0; // 0 - not found; 1 - 'a' found; 2 - 'A' found

    while ( pos != std::string::npos ) {
        pos = findNextWord ( sentence, pos );
        std::string word;
        if ( pos != std::string::npos ) {
            word = sentence.substr ( previousPos, pos - previousPos );
        } else {
            word = sentence.substr ( previousPos );
        }
        std::string pureWord = trimPureWord ( word );
        if ( indefiniteArticleFound > 0 ) {
            if ( isVowelSoundingWord ( pureWord ) ) {
                size_t pos2 = result.find_last_of ( indefiniteArticleFound == 1 ? "a" : "A" );
                result = result.substr ( 0, pos2 ) + ( indefiniteArticleFound == 1 ? "an" : "An" ) + result.substr ( pos2 + 1 );
            }
            // indefiniteArticleFound = 0;
        }
        if ( pureWord == "a" ) {
            indefiniteArticleFound = 1;
        } else if ( pureWord == "A" ) {
            indefiniteArticleFound = 2;
        } else if ( pureWord != "" ) {
            indefiniteArticleFound = 0;
        }
        result += word;
        previousPos = pos;
    }
    return result;
}
