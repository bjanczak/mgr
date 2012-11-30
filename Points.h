/*
 ============================================================================
 Name        : Point.h
 Author      : Bart³omiej Jañczak
 Date        : 2012-09-04
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Declaration of Point classes used by specific algorithms.
 ============================================================================
 */


#ifndef _POINT_H_
#define _POINT_H_

#include <vector>
#include <list>
#include <map>

using namespace std;

/**
 * Structure comparing two double values.
 */
struct DistanceComparator {
  bool operator() (const double& a, const double& b) const
  {return a<b;}
};

/**
 * Class representing sparse point.
 */
class SparsePoint{

public:
	
	double value;
	unsigned long dimension;

	SparsePoint();

	SparsePoint(const SparsePoint& object);

	SparsePoint(unsigned long dimension, double value);

	/**
	 * Compares sparse points a and b by means of dimension.
	 *
	 * @a                 Sparse point a.
	 * @b                 Sparse point b.
	 *
	 * @return            a.dimension < b.dimension
	 */
	static bool comparator(SparsePoint& a, SparsePoint& b);
};

/**
 * Basic point class.
 */
class Point{

public:

	unsigned long id;

	vector<double> denseFormatPoint;
	vector<SparsePoint> sparseFormatPoint;
	vector<double> distance;
	bool isDense;

	virtual ~Point();
	
	Point();

	Point(const Point& object);

	/**
	 * Dense point constructor.
	 *
	 * @id                Point identifier.
	 * @denseFormatPoint  Point definition in dense format.
	 */
	Point(const unsigned long id, const vector<double>& denseFormatPoint);

	/**
	 * Sparse point constructor.
	 *
	 * @id                Point identifier.
	 * @sparseFormatPoint Point definition in sparse format.
	 */
	Point(const unsigned long id, const vector<SparsePoint>& sparseFormatPoint);

	/**
	 * Gets point value in given dimension.
	 *
	 * @dimension          Dimension which point value is returned.
	 *                     Dimension counting starts from 1.
	 *
	 * @return             Got dimension value.
	 */
	double getDimensionValue(const unsigned long dimension);

	/**
	 * Calculates inverted square root.
	 *
	 * @x               
	 *
	 * @return            Calculated value.
	 */
	static float invSqrt2(float x);

	/**
	 * Calculates Minkowski distance of order orderP.
	 *
	 * @a                 First point.
	 * @b                 Second point.
	 * @orderP            Minkowski distance order.
	 *
	 * @return            Calculated value.
	 */
	static double minkowskiDistance(
		const Point& a
		, const Point& b
		, unsigned int orderP);

	/**
	 * Compares points a and b by means of distance values.
	 *
	 * @a                 Point a.
	 * @b                 Point b.
	 *
	 * @return            a.distance < b.distance
	 */
	static bool distanceComparator(const Point& a,const Point& b);
	static bool distanceComparatorPointer(const Point* a, const Point* b);
};

/**
 * DBSCAN variants algorithms point class.
 */
class DbscanPoint: public Point{

public:

	static const unsigned long NOISE;
	static const unsigned long UNCLASSIFIED;

	/**
	 * clusterId = 0, represents NOISE.
	 * clusterId = ULONG_MAX, represents UNCLASSIFIED.
	 */
	unsigned long clusterId;
	bool isAnalyzed;
	unsigned long neighborsNr;
	list<DbscanPoint*> borderPoints;

	~DbscanPoint();
	
	DbscanPoint();

	DbscanPoint(const DbscanPoint& object);

	DbscanPoint(const Point& object);

	/**
	 * Compares points a and b by means of clusterId.
	 *
	 * @a                 Point a.
	 * @b                 Point b.
	 *
	 * @return            a.dimension < b.dimension
	 */
	static bool clusterIdComparator(DbscanPoint& a, DbscanPoint& b);

	/**
	 * Compares points a and b by means of distance values.
	 *
	 * @a                 Point a.
	 * @b                 Point b.
	 *
	 * @return            a.distance < b.distance
	 */
	static bool distanceComparatorIterator(const vector<DbscanPoint>::iterator a, const vector<DbscanPoint>::iterator b);
};

/**
 * K_NEIGHBORHOOD variants algorithms point class.
 */
class KNeighborhoodPoint: public Point{

public:

	double eps;

	multimap<double, vector<KNeighborhoodPoint>::iterator, DistanceComparator> neighbors;

	~KNeighborhoodPoint();

	KNeighborhoodPoint();

	KNeighborhoodPoint(const KNeighborhoodPoint& object);

	KNeighborhoodPoint(const Point& object);

	/**
	 * Compares points a and b by means of distance values.
	 *
	 * @a                 Point a.
	 * @b                 Point b.
	 *
	 * @return            a.distance < b.distance
	 */
	static bool distanceComparatorIterator(const vector<KNeighborhoodPoint>::iterator a, const vector<KNeighborhoodPoint>::iterator b);
};

/**
 * Forward declaration of class VpsPoint.
 */
class VpsPoint;

/**
 * Index class used by  VpsPoint class to trace distance to ancestor.
 */ 
class History{

public:

	/**
	 * Pointer to ancestor.
	 */
	VpsPoint* ancestor;

	/**
	 * Distance to ancestor.
	 */
	double distance;

	~History();

	History();

	History(const History& object);
};

/**
 * Index class used by  VpsPoint class to keep subspace boundaries for each ancestor.
 */ 
class Boundaries{

public:

	/**
	 * Pointer to ancestor.
	 */
	VpsPoint* ancestor;

	/**
	 * Lower boundary.
	 */
	double lower;

	/**
	 * Higher boundary.
	 */
	double higher;

	~Boundaries();

	Boundaries();

	Boundaries(const Boundaries& object);
};

/**
 * VPS_TREE index point/node class.
 */
class VpsPoint: public Point{
	
public:

	/**
	 * VpsPoint median.
	 */
	double median;

	/**
	 * Distance between current vantage point and a point that belongs to
	 * set defined by current vantage point and is farthest from the
	 * current vantage point.
	 */
	double leftBoundHigh;

	/**
	 * Distance between current vantage point and a point that does not 
	 * belong to set defined by current vantage point and is nearest 
	 * to the current vantage point.
	 */
	double rightBoundLow;

	/**
	 * Distances to ancestors.
	 */
	vector<History> history;

	/**
	 * Subspace boundaries for each ancestor.
	 */
	vector<Boundaries> boundaries;

	/**
	 * Left subtree.
	 */
	VpsPoint* left;

	/**
	 * Right subtree.
	 */
	VpsPoint* right;

	~VpsPoint();

	VpsPoint();

	VpsPoint(const VpsPoint& object);

	/**
	 * Dense Vps point constructor.
	 *
	 * @id                Point identifier.
	 * @denseFormatPoint  Point definition in dense format.
	 */
	VpsPoint(const unsigned long id, const vector<double>& denseFormatPoint);

	/**
	 * Sparse Vps point constructor.
	 *
	 * @id                Point identifier.
	 * @sparseFormatPoint Point definition in sparse format.
	 */
	VpsPoint(
		const unsigned long id
		, const vector<SparsePoint>& sparseFormatPoint);

	/**
	 * Destructs Vps tree.
	 */
	void destroy();

	/**
	 * @return          Iterator pointing to last element of history.
	 */
	vector<History>::iterator getHistoryTail();
};

#endif /*_POINT_H_*/