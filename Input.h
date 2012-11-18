#ifndef ETSAI_ALLEGROTERMINAL_INPUT
#define ETSAI_ALLEGROTERMINAL_INPUT

#include <string>

namespace etsai {
namespace allegroterminal {

class Input {
public:
    Input();

    void clear();

    bool empty() const;
    int size() const;

    bool moveLeft(int offset);
    bool moveRight(int offset);
    void insertChar(char ch);
    void removeChar();

    bool prevCommand();
    bool nextCommand();

    const std::string& getValue() const;

private:
    int historyOffset;
    int inputPos;
    std::string current;
    std::string oldInput;
};

}
}
#endif  //ETSAI_ALLEGROTERMINAL_INPUT