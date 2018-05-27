/**
 * @author Dov Neimand
 */
#include "Edge.h"

/**
 * moves the edge one vertex its inside facet.  The direction moved is counter 
 * clockwise be default and can be toggled with change direction.
 */
void Edge::progress() {
    index = (index + indexDir) % inside->size();
    if (index < 0) index = inside->size() + index;
}

/**
 * Flips the inside and outside polygon of the edge.
 */
void Edge::flip() {
    int tempInd = outsideIndex();
    inside = outside();
    index = tempInd;
}

/**
 * Changes the direction the edge will move along its inside polygon.
 * counter clockwise to clockwise or vice versa.
 */
void Edge::changeDir() {
    indexDir = -indexDir;
}

/**
 * checks if the a() and b() values of the two edges are equal.
 * @param e the edge to be compared
 * @return Are they equal?
 */
bool Edge::operator==(const Edge& e) const {
    return (a() == e.a() && b() == e.b()) || (a() == e.b() && b() == e.a());
}

/**
 * the vert vertex of the edge counter clockwise on the inside facet.
 * @return 
 */
Point Edge::a() const {
    return (*inside)[index];
}

/**
 * the second vertex of the edge counter clockwise on the inside facet.
 * @return 
 */
Point Edge::b() const {
    return (*inside)[(index + 1) % (inside->size())];
}

/**
 * The index of b() on the outside matrix.  This is O(n) where n is the number
 * of vertices on the outside facet. Can be made to be faster by storing the
 * value, but will then require regular maintenance whenever anything is done
 * with this edge.
 * @return The index of b() on the outside matrix.
 */
 int Edge::outsideIndex() const {
    
    for (int i = 0; i < outside()->size(); i++) if ((*outside())[i] == b()) return i;
    cerr << "index not found" << endl;
    throw "index not found";
}

/**
 * the facet outside this edge.  Every edge is between two facets, an inside
 * facet and an outside facet.
 * @return the facet outside this edge
 */
SmartFacet* Edge::outside() const{
    return inside->getNeighbor(index);
}

/**
 * The two points at the ends of the edge.
 * @param os the ostream to output the edge
 * @param e the edge to be outputed
 * @return the ostream
 */
std::ostream& operator<<(std::ostream& os, const Edge& e) {
    return os << "(" << e.a() << "), (" << e.b() << ")";
}
