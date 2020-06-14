//
// Created by Moni on 10-May-20.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "../headers/Table.h"
#include <algorithm>

Table::Table() {

}

Table::Table(const std::string &pathToFile) {
    setPathToFile(pathToFile);
    std::string line;
    std::ifstream myfile (pathToFile);
    if (myfile.is_open())
    {
        getline (myfile,line);
        std::vector<std::string> tokens = splitLineByDelimeter(',',line);
        setTableName(tokens[0]);
        tokens.erase(tokens.begin(),tokens.begin()+1);
        setTypes(tokens);

        getline (myfile,line);
        std::vector<std::string> colNames = splitLineByDelimeter(',',line);
        this->setColNames(colNames);

        while ( getline (myfile,line) )
        {
            std::string formattedLine = removeNewLines(line);
            this->insertRow(formattedLine);
        }
        myfile.close();
    }
    else std::cout << "Unable to open file";
}
//! splits a string into tokens by given delimeter
std::vector<std::string> Table::splitLineByDelimeter(char delimeter, std::string line) {
    line = removeNewLines(line);

    std::stringstream steam(line);
    std::string splited;
    std::vector<std::string> tokens;
    while(getline(steam, splited, delimeter))
    {
        tokens.push_back(splited);
    }
    return tokens;
}

const std::string &Table::getTableName() const {
    return tableName;
}

void Table::setTableName(const std::string &tableName) {
    Table::tableName = tableName;
}

const std::vector<std::string> &Table::getTypes() const {
    return types;
}

void Table::setTypes(const std::vector<std::string> &types) {
    Table::types = types;
}

const std::vector<std::string> &Table::getRows() const {
    return rows;
}

void Table::setRows(const std::vector<std::string> &rows) {
    Table::rows = rows;
}

const std::string &Table::getPathToFile() const {
    return pathToFile;
}

void Table::setPathToFile(const std::string &pathToFile) {
    Table::pathToFile = pathToFile;
}

void Table::insertRow(const std::string& row) {
    this->rows.push_back(row);
}
//! removes the new lines symbols by given line
std::string Table::removeNewLines(const std::string& line) {
    std::string lineToChange = line;
    while ( line.find ("\r") != std::string::npos )
    {
        lineToChange.erase ( line.find ("\r"), 2 );
    }
    while ( line.find ("\n") != std::string::npos )
    {
        lineToChange.erase ( line.find ("\n"), 2 );
    }
    return lineToChange;
}

std::ostream &operator<<(std::ostream &out, const Table &table) {
    out<<table.getTableName()+',';

    for(int i=0;i<table.getTypes().size()-1;i++){
        out<<table.getTypes()[i]+',';
    }
    out<<table.getTypes()[table.getTypes().size()-1]<<std::endl;

    for(int i=0;i<table.getColNames().size()-1;i++){
        out<<table.getColNames()[i]+',';
    }
    out<<table.getColNames()[table.getColNames().size()-1]<<std::endl;

    for(int i=0;i<table.getRows().size();i++){
        out<<table.getRows()[i]<<std::endl;
    }
    return out;
}

//! prints col by given col and value
void Table::printSelectColByVal(int col,const std::string& value) {
    if(col> this->getTypes().size() || col<0){
        std::cout<<"Col out of bound"<<std::endl;
        return;
    }
    std::string type = this->getTypes().at(col-1);
    std::vector<std::string> rowsContainingValue;
    for(int i=0;i<this->getRows().size();i++){
        std::string row = this->getRows()[i];
        std::vector<std::string> tokens = this->splitLineByDelimeter(',',row);

        std::string valToCheck = tokens.at(col-1);

        if((valToCheck == "NULL" && value != "NULL")
            ||  (valToCheck != "NULL" && value == "NULL")){
            continue;
        }
        if(valToCheck == "NULL" && value == "NULL"){
            rowsContainingValue.push_back(row);
            continue;
        }
        if(type == "int"){
            int toCheck = std::stoi(valToCheck);
            int valueChecked = std::stoi(value);
            if(toCheck == valueChecked){
                rowsContainingValue.push_back(row);
            }
        }
        else if(type=="string"){
            if(value == valToCheck){
                rowsContainingValue.push_back(row);
            }
        }
        else if(type=="double"){
            double toCheck = std::stod(valToCheck);
            double valueChecked = std::stod(value);
            if(toCheck == valueChecked){
                rowsContainingValue.push_back(row);
            }
        }
    }

    if(rowsContainingValue.empty()){
        std::cout<<"0 matching values found"<<std::endl;
    }
    for(int i=0;i<rowsContainingValue.size();i++){
        std::cout<<rowsContainingValue[i]<<std::endl;
    }

}
//! delete rows by given col and val
void Table::deleteRowsByColValue(int col,const std::string& value) {
    std::vector<std::string> newTableRows;

    if(col> this->getTypes().size() || col<0){
        std::cout<<"Col out of bound"<<std::endl;
        return;
    }
    std::string type = this->getTypes().at(col-1);
    for(int i=0;i<this->getRows().size();i++){
        std::string row = this->getRows()[i];
        std::vector<std::string> tokens = this->splitLineByDelimeter(',',row);

        std::string valToCheck = tokens.at(col-1);

        if((valToCheck == "NULL" && value != "NULL")
           ||  (valToCheck != "NULL" && value == "NULL")){
            newTableRows.push_back(row);
            continue;
        }
        if(valToCheck == "NULL" && value == "NULL"){
            continue;
        }
        if(type == "int"){
            int toCheck = std::stoi(valToCheck);
            int valueChecked = std::stoi(value);
            if(toCheck != valueChecked){
                newTableRows.push_back(row);
            }
        }
        else if(type=="string"){
            if(value != valToCheck){
                newTableRows.push_back(row);
            }
        }
        else if(type=="double"){
            double toCheck = std::stod(valToCheck);
            double valueChecked = std::stod(value);
            if(toCheck != valueChecked){
                newTableRows.push_back(row);
            }
        }

    }
    this->setRows(newTableRows);
}

//! saves the table to the current file
void Table::saveTableToCurrentFile() {
    std::ofstream myfile (this->getPathToFile());
    if (myfile.is_open())
    {
        myfile << *this;
        myfile.close();
    }
}
//! checks if a given string is an integer
bool isInteger(const std::string& input){
   if(input.empty()){
       return false;
   }
   for(int i=0;i<input.size();i++){
       char c = input.at(i);
       if(i==0 && (c == '+' || c == '-') && input.size()==1){
           return false;
       }
       else if(i==0 && (c == '+' || c == '-')){
           continue;
       }
       if(!isdigit(c)){
           return false;
       }
   }
   return true;
}
//! checks if a given string is a double
bool isDouble(const std::string& input){
    int dotsCount =0;
    if(input.empty()){
        return false;
    }
    for(int i=0;i<input.size();i++){
        char c = input.at(i);
        if(i==0 && (c == '+' || c == '-') && input.size()==1){
            return false;
        }
        else if(i==0 && (c == '+' || c == '-')){
            continue;
        }
        else if(!isdigit(c) && c=='.'){
            dotsCount++;
            continue;
        }
        else if(!isdigit(c)){
            return false;
        }
    }
    return  dotsCount == 1;
}

//! updates table values by given search col, search value, target col and the value we want
void Table::updateTableVal(int searchCol,const std::string& searchVal, int targetCol,const std::string& valueToChange) {
    //check if index out of bound
    if(searchCol> this->getTypes().size() || searchCol<0
        ||targetCol> this->getTypes().size() || targetCol<0){
        std::cout<<"Col out of bound"<<std::endl;
        return;
    }
    std::string type = this->getTypes().at(searchCol-1);
    std::vector<std::string> newTableRows;

    std::string targetColType = this->getTypes().at(targetCol-1);
    if(valueToChange == "NULL"){
        //do nothing,because null is valid new value
    }else if(targetColType == "int"){
        if(!isInteger(valueToChange)){
            std::cout<<"Invalid new update value. It should be:"<<targetColType<<std::endl;
            return;
        }
    }else if(targetColType == "double"){
        if(!isDouble(valueToChange)){
            std::cout<<"Invalid new update value. It should be:"<<targetColType<<std::endl;
            return;
        }
    }

    for(int i=0;i<this->getRows().size();i++){
        std::string row = this->getRows()[i];
        std::vector<std::string> tokens = this->splitLineByDelimeter(',',row);

        std::string valToCheck = tokens.at(searchCol-1);


        if((valToCheck == "NULL" && searchVal != "NULL")
           ||  (valToCheck != "NULL" && searchVal == "NULL")){
            newTableRows.push_back(row);
            continue;
        }
        if(valToCheck == "NULL" && searchVal == "NULL"){

            std::string newRow = "";
            for(int i=0;i<tokens.size()-1;i++){
                newRow += tokens.at(i)+",";
            }
            newRow += tokens.at(tokens.size()-1);
            newTableRows.push_back(newRow);
            continue;
        }
        if(type == "int"){
            int toCheck = std::stoi(valToCheck);
            int valueChecked = std::stoi(searchVal);
            if(toCheck==valueChecked){
                tokens.at(targetCol-1) = valueToChange;
            }
            std::string newRow = "";
            for(int i=0;i<tokens.size()-1;i++){
                newRow += tokens.at(i)+",";
            }
            newRow += tokens.at(tokens.size()-1);
            newTableRows.push_back(newRow);

        }
        else if(type=="string"){
            if(searchVal == valToCheck){
                tokens.at(targetCol-1) = valueToChange;
            }
            std::string newRow = "";
            for(int i=0;i<tokens.size()-1;i++){
                newRow += tokens.at(i)+",";
            }
            newRow += tokens.at(tokens.size()-1);
            newTableRows.push_back(newRow);

        }
        else if(type=="double"){
            double toCheck = std::stod(valToCheck);
            double valueChecked = std::stod(searchVal);
            if(toCheck==valueChecked){
                tokens.at(targetCol-1) = valueToChange;
            }
            std::string newRow = "";
            for(int i=0;i<tokens.size()-1;i++){
                newRow += tokens.at(i)+",";
            }
            newRow += tokens.at(tokens.size()-1);
            newTableRows.push_back(newRow);
        }

    }
    this->setRows(newTableRows);
    std::cout<<"Successfully updated value!"<<std::endl;
}

//! inswers a row into the table
void Table::insertRowInTable(const std::string& columns) {
    std::vector<std::string> tokens = this->splitLineByDelimeter(' ',columns);

    std::string row = "";
    bool invalid = false;
    for(int i=0;i<tokens.size();i++){
        if(invalid){
            break;
        }
        std::string type = this->getTypes()[i];
        std::string currentValFromCol = tokens[i];
        if(type=="double"){
            if(!isDouble(currentValFromCol)){
                std::cout<<"Invalid format for col"<<std::endl;
                invalid = true;
            }
        }else if(type == "int"){
            if(!isInteger(currentValFromCol)){
                std::cout<<"Invalid format for col"<<std::endl;
                invalid = true;
            }
        }
    }
    if(invalid){
        return;
    }

    for(int i=0;i<tokens.size()-1;i++){
        row += tokens[i]+",";
    }
    row+=tokens[tokens.size()-1];
    this->addRowInRows(row);
    std::cout<<"Successfully added row:"<<row<<"!"<<std::endl;
}

//! adds a row into the table by given string
void Table::addRowInRows(const std::string& row) {
    this->rows.push_back(row);
}

//! changes the name of the table by given new name string
void Table::changeName(const std::string& newName) {
    this->setTableName(newName);
}
//! inserts a col in the table by given name and type
void Table::insertColInTable(const std::string& colName,const std::string& colType) {
    addColInCols(colName);
    addTypeToTypes(colType);
    addNullToTheEndOfEveryRow();
}

//! adds a NULL value to the end of all rows
void Table:: addNullToTheEndOfEveryRow(){
    for(int i=0;i<this->getRows().size();i++){
        addValToTheEndOfRow(i,"NULL");
    }
}
//! adds a col in the table by given name
void Table::addColInCols(const std::string& colName){
    std::vector<std::string> colNames = this->getColNames();
    for(int i=0;i<colNames.size();i++){
        std::string colNameTemp = colNames[i];
        if(colNameTemp == colName){
            throw std::invalid_argument( "col with this name already exists" );
        }
    }

    this->colNames.push_back(colName);
}

const std::vector<std::string> &Table::getColNames() const {
    return colNames;
}

void Table::setColNames(const std::vector<std::string> &colNames) {
    Table::colNames = colNames;
}
//! add the type to the end of types
void Table::addTypeToTypes(const std::string& type) {
    if(type != "string"
       && type != "int"
       && type != "double" ){
        throw std::invalid_argument( "received invalid col type" );
    }
    this->types.push_back(type);
}
//! adds a value to the end of row
void Table::addValToTheEndOfRow(int id,const std::string& value) {
    if(id<0 || id>this->getRows().size()){
        throw std::invalid_argument( "received invalid row" );
    }

    this->rows[id] += ","+value;
}


//! Prints the number of occurences on given col index and value
void Table::getCountOfValInCol(int index,const std::string& value) {
    if(index<0 || index>this->getTypes().size()){
        throw std::invalid_argument( "received invalid index" );
    }

    int counter = 0;
    for(int i=0;i< this->getRows().size();i++){
        std::string row = this->getRows()[i];
        std::vector<std::string> tokens = splitLineByDelimeter(',',row);
        std::string valueToBeChecked = tokens[index-1];
        if(valueToBeChecked == value)
          counter++;
    }
    std::cout<<"Number of occurrences: "<<counter<<std::endl;
}

//! Print rows on the table
void Table::printRows() {
    int numberOfRowsOnOnePage = 10;

    int startIndex =0;
    int endIndex = startIndex+numberOfRowsOnOnePage;

    std::vector<std::string> rows = getRows();

    std::string command;
    printFromStartIndexToEndIndex(0,endIndex,rows);
    std::cout<<"Available commands: next, previous and exit!"<<std::endl;
    getline(std::cin, command);

    while (true){
        if(command == "exit"){
            std::cout<<"Successfully exited print command"<<std::endl;
            break;
        }
        else if(command=="next"){
            if(startIndex+numberOfRowsOnOnePage < rows.size()){
                endIndex += numberOfRowsOnOnePage;
                startIndex += numberOfRowsOnOnePage;
            }else{
                std::cout<<"out of bound"<<std::endl;
                getline(std::cin, command);
                continue;
            }
        }else if(command=="previous"){
            if(startIndex - numberOfRowsOnOnePage>=0){
                startIndex -= numberOfRowsOnOnePage;
                endIndex -= numberOfRowsOnOnePage;
            } else{
                std::cout<<"out of bound"<<std::endl;
                getline(std::cin, command);
                continue;
            }
        }
        printFromStartIndexToEndIndex(startIndex,endIndex,rows);
        std::cout<<"Available commands: next, previous and exit!"<<std::endl;
        getline(std::cin, command);
    }

}

//! prints rows by given start index, end index and rows
void Table::printFromStartIndexToEndIndex(int startIndex,int endIndex,const std::vector<std::string>& rows){
    if(startIndex<0){
        return;
    }
    std::cout<<"Types: ";
    printTypesByDelimeter(',');
    std::cout<<"Col names: ";
    printColNamesByDelimeter(',');

    int counter =0;
    for(int index = startIndex,i=1; index<endIndex && index<rows.size();index++,i++){
        std::cout<<i<<": "<<rows.at(index)<<std::endl;
        counter++;
    }
    std::cout<<"Showing from: "<<startIndex+1<<" to "<<startIndex+counter<<"! All: "<<rows.size()<<std::endl;
}

//! aggregate function by given search col, search val,
void Table::aggregateOperations(int searchCol,const std::string& searchVal, int targetCol,const std::string& operation) {
    if(searchCol< 0
            || searchCol>this->getTypes().size()
            || targetCol< 0
            || targetCol>this->getTypes().size()){
        throw std::invalid_argument( "received invalid index" );
    }

    searchCol--;
    targetCol--;

    if((this->getTypes()[searchCol] != "double" && this->getTypes()[searchCol] != "int")
    ||(this->getTypes()[targetCol] != "double" && this->getTypes()[targetCol] != "int") ){
        throw std::invalid_argument( "received invalid index of col" );
    }

    std::vector<std::string> targetRowsValues;
    for(int i=0;i<this->getRows().size();i++){
        std::string row = this->getRows()[i];
        std::vector<std::string> tokens = splitLineByDelimeter(',',row);
        std::string valueToCheck = tokens.at(searchCol);
        if(searchVal == valueToCheck ){
            if(tokens[targetCol] != "NULL")
                targetRowsValues.push_back(tokens[targetCol]);
        }
    }

    if(operation=="sum"){
        double sum = 0;
        for(int i=0;i<targetRowsValues.size();i++){
            if(targetRowsValues[i] !="NULL"){
            double currentVal = std::stod(targetRowsValues[i]);
            sum += currentVal;
            }
        }
        std::cout<<"Sum: "<<sum<<std::endl;
    }
    else if(operation=="maximum"){
        double max = std::numeric_limits<double>::lowest();
        for(int i=0;i<targetRowsValues.size();i++){
            if(targetRowsValues[i] !="NULL") {
                double currentVal = std::stod(targetRowsValues[i]);
                if (currentVal > max) {
                    max = currentVal;
                }
            }
        }
        std::cout<<"Max: "<<max<<std::endl;
    }
    else if(operation=="minimum"){
        double min = std::numeric_limits<double>::max();
        for(int i=0;i<targetRowsValues.size();i++){
            if(targetRowsValues[i] !="NULL") {
                double currentVal = std::stod(targetRowsValues[i]);
                if (currentVal < min) {
                    min = currentVal;
                }
            }
        }
        std::cout<<"Min: "<<min<<std::endl;
    }
    else if(operation=="product"){
        double product = 1;
        for(int i=0;i<targetRowsValues.size();i++){
            if(targetRowsValues[i] !="NULL") {
                double currentVal = std::stod(targetRowsValues[i]);
                product *= currentVal;
            }
        }
        std::cout<<"Product: "<<product<<std::endl;
    }

}
//! prints types by given delimeter char
void Table::printTypesByDelimeter(char del) {
    for(int i=0;i<this->getTypes().size()-1;i++){
        std::cout<<this->getTypes()[i]<<del;
    }
    std::cout<<this->getTypes()[this->getTypes().size()-1]<<std::endl;
}

//! prints col names by given delimeter char
void Table::printColNamesByDelimeter(char del) {
    for(int i=0;i<this->getColNames().size()-1;i++){
        std::cout<<this->getColNames()[i]<<del;
    }
    std::cout<<this->getColNames()[this->getColNames().size()-1]<<std::endl;
}

//! inner joins two tables based on given col index, table name and target col
std::string Table::innerJoin(int col,const Table& joinTable, int targetCol) {
    if(col<0 || col>this->getTypes().size() ||
            targetCol<0 || targetCol>joinTable.getTypes().size() ){
        throw std::invalid_argument( "received invalid index of col" );
    }
    std::string firstTableColType = this->getTypes().at(col-1);
    std::string secondTableColType = joinTable.getTypes().at(targetCol-1);
    if(firstTableColType!=secondTableColType){
        throw std::invalid_argument( "Can't join tables with two different field types!" );
    }
    std::string newJoinedTableName = this->getTableName()
            + "_JOIN_"+joinTable.getTableName()+"_COL_"
            +std::to_string(col)+"_AND_"+std::to_string(targetCol);

    Table newTable = Table();
    newTable.setTableName(newJoinedTableName);

    std::vector<std::string> types;
    for(int i=0;i<this->getTypes().size();i++){
        types.push_back(this->getTypes()[i]);
    }
    for(int i=0;i<joinTable.getTypes().size();i++){
        types.push_back(joinTable.getTypes()[i]);
    }

    newTable.setTypes(types);

    std::vector<std::string> colNames;
    for(int i=0;i<this->getColNames().size();i++){
        colNames.push_back(this->getColNames()[i]);
    }
    for(int i=0;i<joinTable.getColNames().size();i++){
        colNames.push_back(joinTable.getColNames()[i]);
    }

    newTable.setColNames(colNames);

    newTable.setPathToFile("..//tables//"+newJoinedTableName+".txt");

    std::vector<std::string> newTableRows;
    for(int i=0;i<this->getRows().size();i++){
        std::string currentRow = this->getRows()[i];
        std::vector<std::string> tokens = splitLineByDelimeter(',',currentRow);
        std::string currentValOnRow = tokens.at(col-1);
        for(int j =0;j<joinTable.getRows().size();j++){
            std::string joinedCurrentRow = joinTable.getRows()[j];
            std::vector<std::string> joinedTokens = splitLineByDelimeter(',',joinedCurrentRow);
            std::string joinedCurrentValOnRow = joinedTokens.at(targetCol-1);
            if(joinedCurrentValOnRow == currentValOnRow ){
                //same target col
                std::string rowToAdd = currentRow+","+joinedCurrentRow;
                newTableRows.push_back(rowToAdd);
            }
        }
    }
    newTable.setRows(newTableRows);
    newTable.saveTableToCurrentFile();
    std::cout<<newTable.getTableName()<<std::endl;
    return newTable.getPathToFile();
}
