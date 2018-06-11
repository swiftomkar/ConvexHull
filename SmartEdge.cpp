/**
 * @author Dov Neimand
 */
#include "SmartEdge.h"

/**
 * moves the edge one vertex its inside facet.  The direction moved is counter 
 * clockwise be default and can be toggled with change direction.
 */
void SmartEdge::progress() {
    index = (index + indexDir) % inside->size();
    if (index < 0) index = inside->size() + index;
}

/**
 * Flips the inside and outside polygon of the edge.
 */
void SmartEdge::flip() {
    int tempInd = outsideIndex();
    inside = outside();
    index = tempInd;
}

/**
 * Changes the direction the edge will move along its inside polygon.
 * This should be thought of as going backwards, not turning around.  b() will
 * now be in the back and a() in the front.
 * counter clockwise to clockwise or vice versa.
 */
void SmartEdge::changeDir() {
    indexDir = -indexDir;
}

/**
 * backs the edge up.
 */
void SmartEdge::backUp() {
    index--;
    if(index < 0) index = inside->size() - 1;
}

/**
 * equals
 * @param e
 * @return e 
 */
SmartEdge& SmartEdge::operator=(const SmartEdge& e) {
    inside = e.inside;
    index = e.index;
    indexDir = e.indexDir;
    return *this;
}

/**
 * checks if the a() and b() values of the two edges are equal.
 * @param e the edge to be compared
 * @return Are they equal?
 */
bool SmartEdge::operator==(const SmartEdge& e) const {
    return (a() == e.a() && b() == e.b()) || (a() == e.b() && b() == e.a());
}

/**
 * the vert vertex of the edge counter clockwise on the inside facet.
 * @return 
 */
Point SmartEdge::a() const {
    return (*inside)[index];
}

/**
 * the second vertex of the edge counter clockwise on the inside facet.
 * @return 
 */
Point SmartEdge::b() const {
    return (*inside)[(index + 1) % (inside->size())];
}

/**
 * The index of b() on the outside matrix.  This is O(n) where n is the number
 * of vertices on the outside facet. Can be made to be faster by storing the
 * value, but will then require regular maintenance whenever anything is done
 * with this edge.
 * @return The index of b() on the outside matrix.
 */
int SmartEdge::outsideIndex(){
//    return outside()->neighborIndex(inside);
    
    for(int i = 0; i < outside()->size(); i++)
        if((*outside())[i] == b()) return i;
    cerr << "SmartEdge::outsideIndex() - index not found" << endl;
    return -1;
}

/**
 * the facet outside this edge.  Every edge is between two facets, an inside
 * facet and an outside facet.
 * @return the facet outside this edge
 */
SmartFacet* SmartEdge::outside() const {
    return inside->getNeighbor(index);
}

/**
 * The two points at the ends of the edge.
 * @param os the ostream to output the edge
 * @param e the edge to be outputed
 * @return the ostream
 */
std::ostream& operator<<(std::ostream& os, const SmartEdge& e) {
    return os << "(" << e.a() << "), (" << e.b() << ")";
}

/**
 * gets the index
 * @return the index of the inside facet that is this edge
 */
int SmartEdge::getIndex() const {
    return index;
}

/**
 * sets the index for this facet on the inside edge
 * @param i the index
 */
void SmartEdge::setIndex(int i) {
    index = i;
}
