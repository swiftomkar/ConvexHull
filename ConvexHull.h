#pragma once
#include <vector>
#include "SmartFacet.h"

using namespace std;

/**
 * This class is the parent class for convex hulls.
 * @author Dov Neimand
 */
class ConvexHull : public vector<SmartFacet> {

    friend ostream& operator << (ostream& os, const ConvexHull& ch);
};
