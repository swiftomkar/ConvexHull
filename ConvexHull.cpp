
# include "ConvexHull.h"
#include <iostream>
/**
 * A printable list of the facets in ch
 * @param os an ostream to output the convex hull desacription
 * @param ch the convex hull to be printed.
 * @return the ostream.
 */
ostream& operator<<(ostream& os, const ConvexHull& ch) {
    for (int i = 0; i < ch.size(); i++) {
        os << ch[i] << endl;
    }
    return os;
}