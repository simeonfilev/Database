//
// Created by Moni on 10-May-20.
//

#include "../headers/Catalog.h"

#include <iostream>


std::ostream &operator<<(std::ostream &out, Catalog &object) {
    std::map<std::string, std::string>::iterator it;
    for (auto const &x : object.catalog) {
        out << (x.first + " - " + x.second) << '\n';
    }
    return out;
}

std::istream &operator>>(std::istream &in, Catalog &object) {
    return in;
}


const std::map<std::string, std::string> &Catalog::getCatalog() const {
    return catalog;
}

void Catalog::setCatalog(const std::map<std::string, std::string> &catalog) {
    Catalog::catalog = catalog;
}

Catalog::Catalog() {

}
//! adds a table in the catalog if it doesn't exists in the catalog
void Catalog::insertTableInCatalog(const std::string& name,const std::string& path) {
    std::map<std::string, std::string>::iterator it = this->catalog.find(name);
    if (it != this->catalog.end()) {
        throw std::invalid_argument("Table already exists");
    }

    this->catalog.insert(std::make_pair(name, path));
}

//! return a table by given table name if it exists
Table Catalog::getTableByName(const std::string &name) {
    //check if exists
    if (this->catalog.find(name) == this->catalog.end()) {
        throw std::invalid_argument("received invalid name of table");
    } else {
        return this->catalog.at(name);
    }
}

//! checks if a given table name exists in the catalog
bool Catalog::tableExistsInCatalog(const std::string& tableName) {
    try {
        getTableByName(tableName);
        return true;
    } catch (const std::invalid_argument &e) {
        return false;
    }
}
//! changes a table name by given old table name and new table name
void Catalog::changeTableName(const std::string& oldName,const std::string& newName) {
    std::map<std::string, std::string>::iterator it = this->catalog.find(oldName);

    if (it == this->catalog.end()) {
        throw std::invalid_argument("Table not found!");
    }

    std::string path = this->getTableByName(oldName).getPathToFile();

    this->catalog.erase(oldName);

    this->insertTableInCatalog(newName, path);


}


