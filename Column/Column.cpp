#include <iostream>
#include <optional>

#include "Column.h"

Column::Column(const std::string& colName){
    this->title = colName;
    this->data = std::vector<std::optional<int>>();
    this->data.reserve(REALLOC_SIZE);
    this->index = std::vector<size_t>();
    this->validIndex = false;
    this->sortAscending = true;
}

bool Column::insertValue(std::optional<int> value)
{

    if (this->data.size() < REALLOC_SIZE) 
    {
        this->data.push_back(value);
        // Invalider l'index après insertion
        if (this->validIndex) {
            this->validIndex = false;
        }
        return true;
    }

    std::cout << "VECTOR IS FULL" << std::endl;
    return false;
}

bool Column::removeValue(const int index)
{
    if (index < 0 || static_cast<size_t>(index) >= data.size())
        return false;

    data.erase(data.begin() + index);
    return true;
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

bool Column::setName(const std::string newValue)
{
    if (newValue.empty())
        return false;

    this->title = newValue;
    return true;
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
    // Initialiser ou redimensionner l'index si nécessaire
    if (this->index.empty() || this->index.size() != this->data.size()) {
        this->index.resize(this->data.size());
        std::iota(this->index.begin(), this->index.end(), 0);
    }

    // Trier l'index
    std::sort(this->index.begin(), this->index.end(),
        [this, ascending](size_t a, size_t b) {
            if (ascending) {
                return this->compareValues(this->data[a].value(), this->data[b].value()) < 0;
            } else {
                return this->compareValues(this->data[a].value(), this->data[b].value()) > 0;
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
        std::cout << "[" << idx << "] " << this->data[idx].value() << std::endl;
    }
}

void Column::eraseIndex()
{
    // Ne pas vider le vecteur, juste le marquer comme invalide
    // Cela permet de distinguer "jamais créé" (-1) de "créé mais invalide" (0)
    this->validIndex = false;
}

int Column::checkIndex() const
{
    // Si l'index est vide, il n'existe pas
    if (this->index.empty()) {
        return -1;
    }

    // Si l'index existe mais n'est pas valide
    if (!this->validIndex) {
        return 0;
    }

    // L'index est valide et correct
    return 1;
}

void Column::updateIndex()
{
    // Si l'index n'existe pas, utiliser l'ordre croissant par défaut
    if (this->index.empty()) {
        this->sort(true);
    } else {
        // Reconstruire l'index avec l'ordre de tri actuel
        this->sort(this->sortAscending);
    }
}

int Column::searchValue(int val) const
{
    // Vérifier si la colonne est triée
    if (!this->validIndex) {
        return -1;
    }

    // Recherche dichotomique manuelle
    size_t left = 0;
    size_t right = this->index.size();

    while (left < right) {
        size_t mid = left + (right - left) / 2;
        size_t idx = this->index[mid];
        int comparison = this->compareValues(this->data[idx].value(), val);

        if (comparison == 0) {
            return 1; // Trouvé
        } else if (comparison < 0) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    return 0; // Non trouvé
}

bool Column::exist(const int value)
{
    for (int i = 0; i < this->getSize(); i++)
    {
        if(this->data[i] == value)
            return true;
    }

    return false;
}

