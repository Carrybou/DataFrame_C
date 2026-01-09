#include <iostream>
#include <optional>

#include "Column.h"

Column::Column(const std::string& colName){
    this->title = colName;
    this->data = std::vector<int>();
    this->data.reserve(REALLOC_SIZE);
    this->index = std::vector<size_t>();
    this->validIndex = false;
    this->sortAscending = true;
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

int Column::compareValues(int a, int b) const
{
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

void Column::sort(bool ascending)
{
    // Initialiser l'index si nécessaire
    if (this->index.empty()) {
        this->index.resize(this->data.size());
        std::iota(this->index.begin(), this->index.end(), 0);
    }

    // Trier l'index
    std::sort(this->index.begin(), this->index.end(),
        [this, ascending](size_t a, size_t b) {
            if (ascending) {
                return this->compareValues(this->data[a], this->data[b]) < 0;
            } else {
                return this->compareValues(this->data[a], this->data[b]) > 0;
            }
        });

    this->validIndex = true;
    this->sortAscending = ascending;
}

void Column::printSorted(bool ascending)
{
    // Si l'index n'existe pas ou n'est pas dans le bon ordre, trier
    if (!this->validIndex || this->sortAscending != ascending) {
        this->sort(ascending);
    }

    // Afficher les valeurs dans l'ordre trié
    for (size_t i = 0; i < this->index.size(); i++) {
        size_t idx = this->index[i];
        std::cout << "[" << idx << "] " << this->data[idx] << std::endl;
    }
}

