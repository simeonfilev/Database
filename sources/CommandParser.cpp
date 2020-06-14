//
// Created by Moni on 04-May-20.
//

#include <string>
#include <iostream>
#include <fstream>
#include "../headers/Catalog.h"
#include "algorithm"

enum Command {
    import,
    showtables,
    describe,
    print,
    export_file,
    select_col,
    addcolumn,
    update,
    delete_col,
    innerjoin,
    rename_table,
    count,
    aggregate,
    insert_row,
    open,
    close,
    save,
    save_as,
    help,
    exit_command,
    invalid
};


/*! \class CommandParser
    \brief Клас който се грижи за командите вместо main
*/
class CommandParser {

public:
    static Command getCommand(std::string const &str) {
        if (str == "import")
            return import;
        else if (str == "showtables")
            return showtables;
        else if (str == "describe")
            return describe;
        else if (str == "print")
            return print;
        else if (str == "export")
            return export_file;
        else if (str == "select")
            return select_col;
        else if (str == "addcolumn")
            return addcolumn;
        else if (str == "update")
            return update;
        else if (str == "delete")
            return delete_col;
        else if (str == "insert")
            return insert_row;
        else if (str == "innerjoin")
            return innerjoin;
        else if (str == "rename")
            return rename_table;
        else if (str == "count")
            return count;
        else if (str == "aggregate")
            return aggregate;
        else if (str == "open")
            return open;
        else if (str == "close")
            return close;
        else if (str == "save")
            return save;
        else if (str == "saveas")
            return save_as;
        else if (str == "help")
            return help;
        else if (str == "exit")
            return exit_command;
        else
            return invalid;


    };

    //! prints a help message
    void printHelpMsg() {
        std::cout << "The following commands are supported:\n"
                     "open <file> opens <file>\n"
                     "close closes currently opened file\n"
                     "save saves the currently open file\n"
                     "saveas <file> saves the currently open file in <file>\n"
                     "help prints this information\n"
                     "exit exists the program" << std::endl;
    }

    //! prints a exit message
    void printExitMsg() {
        std::cout << "Exiting the program..." << std::endl;
    }

    //! returns the number of spaces
    int getNumberOfSpaces(const std::string& expression) {
        int counter = 0;
        for (int i = 0; i < expression.length(); i++) {
            if (expression[i] == ' ')
                counter++;
        }
        return counter;
    }

    //! prints all tables in given catalog
    void printAllTables(Catalog catalog) {
        std::map<std::string, std::string> tables = catalog.getCatalog();
        int counter =0;
        for (auto const& x : tables) {
            std::cout << (x.first +" - "+decodePath( x.second)) << std::endl;
            counter++;
        }
         std::cout<<"Total: "<<counter<<std::endl;

    }

    //! imports a table into the catalog
    void importTable(const std::string& expression, Catalog *Catalog) {
        //import {path} : EXAMPLE : import ./tables/users.txt
       try{
           Table t = getTableWithPath(encodePath(expression));
           Catalog->insertTableInCatalog(t.getTableName(), t.getPathToFile());
           std::cout<<"Successfully imported!"<<std::endl;
       }  catch (const std::invalid_argument &e) {
           std::cout << e.what() << std::endl;
       }


    }

    //! prints all fields in the given table
    void descibeFieldsFromTable( const std::string &tableName, Catalog *catalog) {
        try{
            Table t = catalog->getTableByName(tableName);
            std::cout<<"Types: ";
            t.printTypesByDelimeter(',');
            std::cout<<"Names: ";
            t.printColNamesByDelimeter(',');
        } catch (const std::invalid_argument &e) {
            std::cout<<e.what()<<std::endl;
        }

    }

    //! returns a table by given path
    Table getTableWithPath(std::string path) {
        Table *t = new Table(path);
        return *t;
    }

    //! encodes a path
    std::string encodePath(std::string path) {
        std::string ans = "";
        int indexOfExtension = path.rfind('.');
        for (int i = 0; i < path.size(); i++) {
            if (indexOfExtension != i && (path.at(i) == '.' || path.at(i) == '/')) {
                ans += path.at(i);
            }
            ans += path.at(i);
        }
        return ans;
    }

    //! decodes a path
    std::string decodePath(std::string path) {
        std::string ans = "";
        for (int i = 1; i < path.size(); i++) {
            if (path.at(i - 1) == path.at(i) && (path.at(i) == '.' || path.at(i) == '/')) {
                continue;
            }
            ans += path.at(i - 1);
        }
        ans += path.at(path.size() - 1);
        return ans;
    }
    //! exports a table to new file
    void exportTable(const std::string& text, Catalog *catalog) {
        std::string expression = text;
        std::string name = expression.substr(0, expression.find(' '));
        try{
            Table t = catalog->getTableByName(name);
            expression = expression.substr(expression.find(' ') + 1);
            std::string fileName = expression;
            t.setPathToFile("..//tables//"+fileName);
            std::ofstream outfile(fileName);
            outfile << t << std::endl;
            outfile.close();
            std::cout<<"Successfully exported!"<<std::endl;
        } catch (const std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }

    }

    //! prints a value from col
    void selectValue(const std::string& text, Catalog *catalog) {
        std::string expression = text;
        std::string colName = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string value = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string tableName = expression.substr(0);

        try {
            Table t = catalog->getTableByName(tableName);
            t.printSelectColByVal(std::stoi(colName), value);
            std::cout<<"Successfully selected value!"<<std::endl;
        } catch (const std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }

    }

    //! removes a value
    void deleteValue(const std::string& text, Catalog *catalog) {
        std::string expression = text;
        std::string tableName = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string col = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string value = expression.substr(0);

        try {
            Table t = catalog->getTableByName(tableName);
            t.deleteRowsByColValue(std::stoi(col), value);
            t.saveTableToCurrentFile();
        } catch (const std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }
    }

    //! updates a value in the table
    void updateValue(const std::string& text, Catalog *catalog) {
        std::string expression = text;
        //update <table name> <search column n> <search value> <target column n> <target value>
        std::string tableName = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string searchCol = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string searchVal = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string targetCol = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string value = expression.substr(0);

        try {
            Table t = catalog->getTableByName(tableName);
            t.updateTableVal(std::stoi(searchCol), searchVal, std::stoi(targetCol), value);
            t.saveTableToCurrentFile();
        } catch (const std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }

    }

    //! insert new row
    void insertRow(const std::string& text, Catalog *catalog) {
        std::string expression = text;
        // insert <table name> <column 1> … <column n>
        std::string tableName = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        try {
            Table t = catalog->getTableByName(tableName);
            t.insertRowInTable(expression);
            t.saveTableToCurrentFile();
        } catch (const std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }

    }
    //! renames a table
    void renameTable(const std::string& text, Catalog *catalog) {
        std::string expression = text;
        std::string oldTableName = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string newTableName = expression.substr(0);

        try {
            if (catalog->tableExistsInCatalog(newTableName)) {
                std::cout << "Table with this name already exists!" << std::endl;
                return;
            }
            Table t = catalog->getTableByName(oldTableName);
            t.changeName(newTableName);
            t.saveTableToCurrentFile();

            catalog->changeTableName(oldTableName, newTableName);
            std::cout<<"Succesfully renamed!"<<std::endl;

        } catch (const std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }
    }

    //! add new col into the table
    void addColumn(const std::string& text, Catalog *catalog) {
        std::string expression = text;
        //addcolumn <table name> <column name> <column type>
        std::string tableName = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string colName = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string colType = expression.substr(0);

        try {
            Table t = catalog->getTableByName(tableName);
            t.insertColInTable(colName, colType);
            t.saveTableToCurrentFile();
        } catch (const std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }

    }
    //! prints the number of occurences
    void getCountOfValInCol(const std::string& text, Catalog *catalog) {
        std::string expression = text;
        //count <table name> <search column n> <search value>

        std::string tableName = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string col = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);
        int colIndex = std::stoi(col);

        std::string value = expression.substr(0);

        try {
            Table t = catalog->getTableByName(tableName);
            t.getCountOfValInCol(colIndex, value);
        } catch (const std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }

    }

    //! prints rows
    void printRows(const std::string& expression, Catalog *catalog) {
        std::string tableName = expression.substr(0);
        try {
            Table t = catalog->getTableByName(tableName);
            t.printRows();
        } catch (const std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }
    }
    //! aggregate function
    void aggregateTable(const std::string& text, Catalog *catalog) {
        std::string expression = text;
        // aggregate <table name> <search column n> <search value> <target column n> <operation>
        std::vector<std::string> availableCommands;
        availableCommands.push_back("sum");
        availableCommands.push_back("product");
        availableCommands.push_back("maximum");
        availableCommands.push_back("minimum");

        std::string tableName = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string searchCol = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string searchVal = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string targetCol = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string operation = expression.substr(0, expression.find(' '));

        try {
            Table t = catalog->getTableByName(tableName);
            if(std::find(availableCommands.begin(),availableCommands.end(),operation) == availableCommands.end()){
                throw std::invalid_argument( "received invalid operation!" );
            }

            t.aggregateOperations(std::stoi(searchCol),searchVal,std::stoi(targetCol),operation);
        } catch (const std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }

    }
    //! inner join
    void innerJoin(const std::string& text, Catalog *catalog) {
        std::string expression = text;
        // innerjoin <table 1> <column n1> <table 2> <column n2>
        std::string firstTableName = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string colFirstTable = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string secondTableName = expression.substr(0, expression.find(' '));
        expression = expression.substr(expression.find(' ') + 1);

        std::string colSecTable = expression.substr(0);

        try {
            Table firstTable = catalog->getTableByName(firstTableName);
            Table secondTable = catalog->getTableByName(secondTableName);

            std::string tablePath = firstTable.innerJoin(std::stoi(colFirstTable), secondTable, std::stoi(colSecTable));

            Table t = getTableWithPath(tablePath);
            t.saveTableToCurrentFile();
            if(!catalog->tableExistsInCatalog(t.getTableName())){
                catalog->insertTableInCatalog(t.getTableName(), t.getPathToFile());
            }else{
                throw std::invalid_argument("Table with this name already exists");
            }

        } catch (const std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }
    }

    void openFile(Catalog *catalog,std::string fileName, std::string currentInput) {
        std::ifstream myfile (encodePath(fileName));
        std::string line;
        if (myfile.is_open())
        {
           while( getline (myfile,line)){
               line = removeNewLines(line);
               execute(line,catalog);
               currentInput += line+"/n";
           }
            myfile.close();
        }
        else std::cout << "Unable to open file";
    }
    std::string removeNewLines(std::string line) {
        while ( line.find ("\r") != std::string::npos )
        {
            line.erase ( line.find ("\r"), 2 );
        }
        while ( line.find ("\n") != std::string::npos )
        {
            line.erase ( line.find ("\n"), 2 );
        }
        return line;
    }

    //! executes given command
    void execute(std::string commandExpression,Catalog *catalog){
        std::string command = commandExpression;
        command = command.substr(0, commandExpression.find(' '));
        Command cmd = CommandParser::getCommand(command);
        switch (cmd) {
            case showtables:
                this->printAllTables(*catalog);
                break;
            case import:
                this->importTable(commandExpression.substr(command.length() + 1),catalog);
                break;
            case describe:
                this->descibeFieldsFromTable(commandExpression.substr(command.length() + 1),catalog);
                break;
            case export_file:
                this->exportTable(commandExpression.substr(command.length() + 1),catalog);
                break;
            case print:
                this->printRows(commandExpression.substr(command.length() + 1),catalog);
                break;
            case select_col:
                this->selectValue(commandExpression.substr(command.length() + 1),catalog);
                break;
            case addcolumn:
                this->addColumn(commandExpression.substr(command.length() + 1),catalog);
                break;
            case update:
                this->updateValue(commandExpression.substr(command.length() + 1),catalog);
                break;
            case delete_col:
                this->deleteValue(commandExpression.substr(command.length() + 1),catalog);
                break;
            case insert_row:
                this->insertRow(commandExpression.substr(command.length() + 1),catalog);
                break;
            case rename_table:
                this->renameTable(commandExpression.substr(command.length() + 1),catalog);
                break;
            case count:
                this->getCountOfValInCol(commandExpression.substr(command.length() + 1),catalog);
                break;
            case aggregate:
                this->aggregateTable(commandExpression.substr(command.length() + 1),catalog);
                break;
            case innerjoin:
                this->innerJoin(commandExpression.substr(command.length() + 1),catalog);
                break;
        }
    }

    void saveFile(std::string fileName, std::string currentInput) {
        std::ofstream outfile;
        outfile.open(encodePath( fileName), std::ios::app);
        outfile<<std::endl;
        outfile<<currentInput<<std::endl;
        outfile.close();
    }
};



