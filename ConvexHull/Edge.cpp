
#include "Edge.h"

void Edge::progress() {
    index = (index + indexDir) % inside->size();
    if (index < 0) index = inside->size() + index;
}

void Edge::flip() {
    int tempInd = outsideIndex();
    inside = outside();
    index = tempInd;
}

void Edge::changeDir() {
    indexDir = -indexDir;
}

bool Edge::operator==(const Edge& e) const {
    return (a() == e.a() && b() == e.b()) || (a() == e.b() && b() == e.a());
}

Point Edge::a() const {
    return (*inside)[index];
}

Point Edge::b() const {
    return (*inside)[(index + 1) % (inside->size())];
}

int Edge::outsideIndex() const {
    for (int i = 0; i < outside()->size(); i++) if ((*outside())[i] == b()) return i;
    throw "index not found";
}

SmartFacet* Edge::outside() const {
    return inside->getNeighbor(index);
}

std::ostream& operator<<(std::ostream& os, const Edge& e) {
    return os << "(" << e.a() << "), (" << e.b() << ")";
}
