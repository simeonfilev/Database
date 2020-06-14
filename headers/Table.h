//
// Created by Moni on 10-May-20.
//

#ifndef DATABASE_TABLE_H
#define DATABASE_TABLE_H


#include <string>
#include <vector>


class Table {
private:
    std::string pathToFile;
    std::string tableName;
    std::vector<std::string> types;
    std::vector<std::string> rows;
    std::vector<std::string> colNames;

public:
    Table();
    Table(const std::string &pathToFile);
    friend std::ostream& operator<<(std::ostream& out, const Table & object);
    friend std::istream& operator>>(std::istream& in, Table & object);

    const std::string &getTableName() const;

    void setTableName(const std::string &tableName);

    const std::vector<std::string> &getTypes() const;

    void setTypes(const std::vector<std::string> &types);

    const std::vector<std::string> &getRows() const;

    void setRows(const std::vector<std::string> &rows);

    const std::string &getPathToFile() const;

    void setPathToFile(const std::string &pathToFile);

    std::vector<std::string> splitLineByDelimeter(char delimeter, std::string line);

    void insertRow(const std::string& row);

    std::string removeNewLines(const std::string& line);

    void printSelectColByVal(int col,const std::string& value);

    void deleteRowsByColValue(int col,const std::string& value);

    void saveTableToCurrentFile();

    void updateTableVal(int searchCol,const std::string& searchVal, int targetColconst,const std::string& value);

    void insertRowInTable(const std::string& columns);

    void addRowInRows(const std::string& row);

    void addColInCols(const std::string& colName);

    void addTypeToTypes(const std::string& type);

    void changeName(const std::string& newName);

    void insertColInTable(const std::string& colName,const std::string& colType);

    const std::vector<std::string> &getColNames() const;

    void setColNames(const std::vector<std::string> &colNames);

    void addNullToTheEndOfEveryRow();

    void addValToTheEndOfRow(int id,const std::string& value);

    void getCountOfValInCol(int index,const std::string& value);

    void printRows();

    void printFromStartIndexToEndIndex(int startIndex, int endIndex,const std::vector<std::string>& rows);

    void aggregateOperations(int searchCol,const std::string& searchVal, int targetCol,const std::string& operation);

    void printTypesByDelimeter(char del);

    void printColNamesByDelimeter(char del);

    std::string innerJoin(int col,const Table& joinTable, int targetCol);
};


#endif //DATABASE_TABLE_H
