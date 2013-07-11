// Boost.Geometry Index
//
// Spatial index distance predicates, calculators and checkers used in nearest neighbor query
//
// Copyright (c) 2011-2013 Adam Wulkiewicz, Lodz, Poland.
//
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_INDEX_DISTANCE_PREDICATES_HPP
#define BOOST_GEOMETRY_INDEX_DISTANCE_PREDICATES_HPP

#include <boost/geometry/index/detail/distance_predicates.hpp>

/*!
\defgroup nearest_relations Nearest relations (pdalboost::geometry::index::)
*/

namespace pdalboost {} namespace boost = pdalboost; namespace pdalboost { namespace geometry { namespace index {

// relations generators

#ifdef BOOST_GEOMETRY_INDEX_DETAIL_EXPERIMENTAL

/*!
\brief Generate to_nearest() relationship.

Generate a nearest query Point and Value's Indexable relationship while calculating
distances. This function may be used to define that knn query should calculate distances
as smallest as possible between query Point and Indexable's points. In other words it
should be the distance to the nearest Indexable's point. This function may be also used
to define distances bounds which indicates that Indexable's nearest point should be
closer or further than value v. This is default relation.

\ingroup nearest_relations

\tparam T   Type of wrapped object. This may be a Point for PointRelation or CoordinateType for
            MinRelation or MaxRelation

\param v    Point or distance value.
*/
template <typename T>
detail::to_nearest<T> to_nearest(T const& v)
{
    return detail::to_nearest<T>(v);
}

/*!
\brief Generate to_centroid() relationship.

Generate a nearest query Point and Value's Indexable relationship while calculating
distances. This function may be used to define that knn query should calculate distances
between query Point and Indexable's centroid. This function may be also used
to define distances bounds which indicates that Indexable's centroid should be
closer or further than value v.

\ingroup nearest_relations

\tparam T   Type of wrapped object. This may be a Point for PointRelation or some CoordinateType for
            MinRelation or MaxRelation

\param v    Point or distance value.
*/
template <typename T>
detail::to_centroid<T> to_centroid(T const& v)
{
    return detail::to_centroid<T>(v);
}

/*!
\brief Generate to_furthest() relationship.

Generate a nearest query Point and Value's Indexable relationship while calculating
distances. This function may be used to define that knn query should calculate distances
as biggest as possible between query Point and Indexable's points. In other words it
should be the distance to the furthest Indexable's point. This function may be also used
to define distances bounds which indicates that Indexable's furthest point should be
closer or further than value v.

\ingroup nearest_relations

\tparam T   Type of wrapped object. This may be a Point for PointRelation or some CoordinateType for
            MinRelation or MaxRelation

\param v    Point or distance value.
*/
template <typename T>
detail::to_furthest<T> to_furthest(T const& v)
{
    return detail::to_furthest<T>(v);
}

#endif // BOOST_GEOMETRY_INDEX_DETAIL_EXPERIMENTAL

// distance predicates generators

/*!
\brief Generate unbounded() distance predicate.

Generate a distance predicate. This defines distances bounds which are used by knn query.
This function indicates that there is no distance bounds and Values should be returned
if distances between Point and Indexable are the smallest. Distance calculation is defined
by PointRelation. This is default nearest predicate.

\ingroup distance_predicates

\tparam PointRelation   PointRelation type.

\param pr               The point relation. This may be generated by \c index::to_nearest(),
                        \c index::to_centroid() or \c index::to_furthest() with \c Point passed as a parameter.
*/
//template <typename PointRelation>
//inline detail::unbounded<PointRelation>
//unbounded(PointRelation const& pr)
//{
//    return detail::unbounded<PointRelation>(pr);
//}

/*!
\brief Generate min_bounded() distance predicate.

Generate a distance predicate. This defines distances bounds which are used by knn query.
This function indicates that Values should be returned only if distances between Point and
Indexable are greater or equal to some min_distance passed in MinRelation. Check for closest Value is
defined by PointRelation. So it is possible e.g. to return Values with centroids closest to some
Point but only if nearest points are further than some distance.

\ingroup distance_predicates

\tparam PointRelation   PointRelation type.
\tparam MinRelation     MinRelation type.

\param pr               The point relation. This may be generated by \c to_nearest(),
                        \c to_centroid() or \c to_furthest() with \c Point passed as a parameter.
\param minr             The minimum bound relation. This may be generated by \c to_nearest(),
                        \c to_centroid() or \c to_furthest() with distance value passed as a parameter.
*/
//template <typename PointRelation, typename MinRelation>
//inline detail::min_bounded<PointRelation, MinRelation>
//min_bounded(PointRelation const& pr, MinRelation const& minr)
//{
//    return detail::min_bounded<PointRelation, MinRelation>(pr, minr);
//}

/*!
\brief Generate max_bounded() distance predicate.

Generate a distance predicate. This defines distances bounds which are used by knn query.
This function indicates that Values should be returned only if distances between Point and
Indexable are lesser or equal to some max_distance passed in MaxRelation. Check for closest Value is
defined by PointRelation. So it is possible e.g. to return Values with centroids closest to some
Point but only if nearest points are closer than some distance.

\ingroup distance_predicates

\tparam PointRelation   PointRelation type.
\tparam MaxRelation     MaxRelation type.

\param pr               The point relation. This may be generated by \c to_nearest(),
                        \c to_centroid() or \c to_furthest() with \c Point passed as a parameter.
\param maxr             The maximum bound relation. This may be generated by \c to_nearest(),
                        \c to_centroid() or \c to_furthest() with distance value passed as a parameter.
*/
//template <typename PointRelation, typename MaxRelation>
//inline detail::max_bounded<PointRelation, MaxRelation>
//max_bounded(PointRelation const& pr, MaxRelation const& maxr)
//{
//    return detail::max_bounded<PointRelation, MaxRelation>(pr, maxr);
//}

/*!
\brief Generate bounded() distance predicate.

Generate a distance predicate. This defines distances bounds which are used by knn query.
This function indicates that Values should be returned only if distances between Point and
Indexable are greater or equal to some min_distance passed in MinRelation and lesser or equal to
some max_distance passed in MaxRelation. Check for closest Value is defined by PointRelation.
So it is possible e.g. to return Values with centroids closest to some Point but only if nearest
points are further than some distance and closer than some other distance.

\ingroup distance_predicates

\tparam PointRelation   PointRelation type.
\tparam MinRelation     MinRelation type.
\tparam MaxRelation     MaxRelation type.

\param pr               The point relation. This may be generated by \c to_nearest(),
                        \c to_centroid() or \c to_furthest() with \c Point passed as a parameter.
\param minr             The minimum bound relation. This may be generated by \c to_nearest(),
                        \c to_centroid() or \c to_furthest() with distance value passed as a parameter.
\param maxr             The maximum bound relation. This may be generated by \c to_nearest(),
                        \c to_centroid() or \c to_furthest() with distance value passed as a parameter.
*/
//template <typename PointRelation, typename MinRelation, typename MaxRelation>
//inline detail::bounded<PointRelation, MinRelation, MaxRelation>
//bounded(PointRelation const& pr, MinRelation const& minr, MaxRelation const& maxr)
//{
//    return detail::bounded<PointRelation, MinRelation, MaxRelation>(pr, minr, maxr);
//}

}}} // namespace pdalboost::geometry::index

#endif // BOOST_GEOMETRY_INDEX_DISTANCE_PREDICATES_HPP
