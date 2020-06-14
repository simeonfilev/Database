#include <iostream>

#include "CommandParser.cpp"

int main() {

    std::string filename;// = "..//project.txt";
    Catalog *catalog = new Catalog();
    bool closed = false;
    std::string commandExpression;
    getline(std::cin, commandExpression);
    std::string currentInput = "";
    CommandParser *commandParser = new CommandParser();
    bool userWantsToQuit = false;
    while (!userWantsToQuit) {
        std::string command = commandExpression;

        command = command.substr(0, commandExpression.find(' '));

        Command cmd = CommandParser::getCommand(command);
        if (closed && (cmd != open )) {
          std::cout<<"You can only use command: open"<<std::endl;
            getline(std::cin, commandExpression);
        }else {
            switch (cmd) {
                case invalid:
                    std::cout << "Invalid command" << std::endl;
                    break;
                case import:
                case showtables:
                case describe:
                case print:
                case export_file:
                case select_col:
                case addcolumn:
                case update:
                case delete_col:
                case innerjoin:
                case rename_table:
                case count:
                case aggregate:
                case insert_row:
                    commandParser->execute(commandExpression, catalog);
                    break;
                case open:
                    //open PATH
                    filename = commandExpression.substr( commandExpression.find(' ')+1);
                    commandParser->openFile(catalog, filename, currentInput);
                    closed = false;
                    break;
                case close:
                    delete catalog;
                    catalog = new Catalog();
                    closed = true;
                    break;
                case save:
                    commandParser->saveFile(filename,currentInput);
                    break;
                case save_as:
                    commandParser->saveFile(commandExpression.substr(command.length() + 1),currentInput);
                    break;
                case help:
                    commandParser->printHelpMsg();
                    break;
                case exit_command:
                    commandParser->printExitMsg();
                    userWantsToQuit = true;
                    break;
                default:
                    std::cout << "Invalid command" << std::endl;
                    break;
            }
            if (!userWantsToQuit) {
                // check if command is valid
                if(cmd != open && cmd!=help && cmd!=save && cmd!=save_as && cmd!=exit_command && cmd!=invalid)
                    currentInput += commandExpression+"\n";
                getline(std::cin, commandExpression);
            }
        }
    }
    return 0;
}
