#ifndef ETSAI_ALLEGROTERMINAL_WINDOW
#define ETSAI_ALLEGROTERMINAL_WINDOW

#include "AllegroTerminal/Terminal/Console.h"
#include <string>

namespace etsai {
namespace allegroterminal {

/**
 * Defines functionality available in the terminal window
 * @author etsai
 */
class Window {
public:
    /**
     * Creates a Window that controls a console
     * @param   console     Console to control
     */
    Window(Console* console);

    /**
     * Moves the cursor a specified number of spaces to the left.  The cursor will only move 
     * as many spaces as there are available and will return the actual number of spaces moved.
     * @param   offset  Number of spaces to move
     * @return  Actual number of spaces moved
     */
    int moveCursorLeft(int offset);
    /**
     * Moves the cursor a specified number of spaces to the right.  The cursor will only move 
     * as many spaces as there are available and will return the actual number of spaces moved.
     * @param   offset  Number of spaces to move
     * @return  Actual number of spaces moved
     */
    int moveCursorRight(int offset);
    /**
     * Moves the cursor to the beginning of the line
     */
    void moveToStart();
    /**
     * Moves the cursor to the end of the line
     */
    void moveToEnd();

    /**
     * Scroll the console output up by a given number of lines
     * @param   offset  Number of lines to scroll up
     */
    void scrollUp(int offset);
    /**
     * Scroll the console output down by a given number of lines
     * @param   offset  Number of lines to scroll down
     */
    void scrollDown(int offset);

    /**
     * Add a character to the spot immediately preceding the cursor.
     * @param   ch  Character to add
     */
    void addChar(char ch);
    /**
     * Remove the character immediately preceding the cursor
     */
    void removeChar();
    /**
     * Delete all of the characters past the cursor
     */
    void deletePastCursor();
    /**
     * Delete all of the characters before the cursor
     */
    void deleteBeforeCursor();

    /**
     * Execute the current input
     */
    void execute();

    /**
     * Retrieve the command entered prior to the current one
     */
    void prevCommand();
    /**
     * Retrieve the command entered after the current one.  If the command is the last one entered, then the 
     * input the user was entering before cycling will be restored.
     */
    void nextCommand();
    
    /**
     * Draw the cursor onto the window
     */
    virtual void drawCursor()= 0;
    /**
     * Draw the console output
     */
    virtual void drawConsole()= 0;
    /**
     * Draw the current user input
     */
    virtual void drawInput()= 0;

protected:
    Console* console;                   ///< Console object tied to the current window
    unsigned int cursorPos;             ///< Current cursor position on the screen
    unsigned int charPos;               ///< Position to insert or remove characters to the input
    unsigned int inputOffset;           ///< Index to start displaying the user input
    int historyOffset;                  ///< Which previously executed command to retrieve
    std::string input;                  ///< Current user input
    std::string savedInput;             ///< Used to save the current input when cycling through the input history
};  //class Window

}   //namespace allegroterminal
}   //namespace etsai

#endif