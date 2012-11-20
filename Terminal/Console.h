#ifndef ETSAI_ALLEGROTERMINAL_CONSOLE
#define ETSAI_ALLEGROTERMINAL_CONSOLE

#include <vector>
#include <string>

namespace etsai {
namespace allegroterminal {

/**
 * Stores the lines to be displayed to the terminal window
 * @author etsai
 */
class Console {
public:
    /**
     * Create a console given certain dimensions
     * @param   charPerLine     Number of characters per line
     * @param   maxLinesStored  Max number of lines to keep
     * @param   maxVisibleLines Max number of lines that are visible in the window
     */
    Console(int charPerLine, int maxLinesStored, int maxVisibleLines);

    /**
     * Get the linse visible in the window
     * @return Visible lines in the window
     */
    std::vector<std::string> getVisibleLines() const;
    /**
     * Get the number of characters in a line
     */
    int getCharPerLine() const;
    /**
     * Get the max number of lines visible in the window
     */
    int getMaxVisibleLines() const;
    
    /**
     * Add a line to the console.  If the line size exceeds the character limit per line, multiple 
     * lines will be used to accomodate the addition
     * @param   line    Line to add
     */
    void addLine(const std::string &line);
    /**
     * Scroll the visible window up by a number of lines
     * @param   numLines    Number of lines to scroll up
     */
    void scrollUp(int numLines);
    /**
     * Scroll the visible window down by a number of lines
     * @param   numLines    Number of lines to scroll down
     */
    void scrollDown(int numLines);

private:
    /**
     * Calculate the number of lines between two indexes.  This function handles wrap around
     * @param   left    Beginning line
     * @param   right   End line
     */
    int calcNumLines(int left, int right) const;
    /**
     * Increment the input by 1, while handling wrap arround
     * @param   i   Pass by reference parameter to increment
     */
    void increment(int &i) const;
    /**
     * Increment the input by the offset, while handling wrap arround
     * @param   i       Pass by reference parameter to increment
     * @param   offset  Amount to add to the input
     */
    void increment(int &i, int offset) const;
    /**
     * Decrement the input by the offset, while handling wrap arround
     * @param   i   Pass by reference parameter to increment
     * @param   offset  Amount to subtract from the input
     */
    void decrement(int &i, int offset) const;

    int charPerLine;                        ///< Number of characters in a line
    int maxVisibleLines;                    ///< Max number of visible line
    int begin;                              ///< Index of the first line
    int end;                                ///< Index of the last line
    int visibleBegin;                       ///< Index of where the visible window begins
    int visibleEnd;                         ///< Index of where the visible window ends
    std::vector<std::string> lines;         ///< Lines stored in the console
};  //class Console

}   //namespace allegroterminal
}   //namespace etsai

#endif  //ETSAI_ALLEGROTERMINAL_CONSOLE
