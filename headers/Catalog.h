//
// Created by Moni on 10-May-20.
//

#ifndef DATABASE_CATALOG_H
#define DATABASE_CATALOG_H


#include <string>
#include <map>
#include "Table.h"

/*! \class Catalog
    \brief Клас който дефинира каталог (списък от таблици и имена на таблици)
*/
class Catalog {
private:
    std::map<std::string, std::string> catalog;

public:
    Catalog();

    const std::map<std::string, std::string> &getCatalog() const;

    void setCatalog(const std::map<std::string, std::string> &catalog);

    friend std::ostream &operator<<( std::ostream &out, Catalog &object);

    friend std::istream &operator>>( std::istream &in, Catalog &object);

    void insertTableInCatalog(const std::string& name,const std::string& path);

    Table getTableByName(const std::string &name);

    bool tableExistsInCatalog(const std::string& tableName);

    void changeTableName(const std::string& oldName,const std::string& newName);

};


#endif //DATABASE_CATALOG_H
