#include <iostream>
#include <optional>

#include "Column.h"

Column::Column(const std::string& colName){
    this->title = colName;
    this->data = std::vector<int>();
    this->data.reserve(REALLOC_SIZE);
}

bool Column::insertValue(int value)
{
    if (this->data.size() < REALLOC_SIZE) 
    {
        this->data.push_back(value);
        return true;
    }

    std::cout << "VECTOR IS FULL" << std::endl;
    return false;
}

std::optional<int> Column::getValueAt(int index) const
{
    if (index < 0 || static_cast<size_t>(index) >= data.size())
        return std::nullopt;

    return data[index];
}

int Column::getSize() const
{
    return this->data.size();
}

std::string Column::getName() const
{
    return this->title;
}

void Column::display() const
{
    for (int i = 0; i < this->getSize(); i++)
    {
        auto v = this->getValueAt(i);
        std::cout << "[" << i << "] ";
        if (v)
            std::cout << *v;
        else
            std::cout << "NULL";
    }
}

int Column::occurence(int value) const
{
    if(this->data.empty())
        return 0;

    int cnt = 0;
    for (int i = 0; i < this->getSize(); i++)
    {
        if(this->data[i] == value)
            cnt++;
    }

    return cnt;
}

int Column::numberGreaterThan(int value) const
{
    if(this->data.empty())
        return 0;

    int cnt = 0;
    for (int i = 0; i < this->getSize(); i++)
    {
        if(this->data[i] > value)
            cnt++;
    }

    return cnt;
}

int Column::numberLowerThan(int value) const
{
    if(this->data.empty())
        return 0;

    int cnt = 0;
    for (int i = 0; i < this->getSize(); i++)
    {
        if(this->data[i] < value)
            cnt++;
    }

    return cnt;
}

