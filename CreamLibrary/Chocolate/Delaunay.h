/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef __DEF_DELAUNAY__
#define __DEF_DELAUNAY__

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string>
#include <assert.h>
#include "../c.library.h"

namespace Cicm
{
	//! The Delaunay triangulation and Voronoy diagram.
    /**
     */
	class Delaunay
	{
	private :
		class DelaunayPoint;
		class DelaunayCircle
		{
		public :
			double x;
			double y;
			double r;
			std::vector<DelaunayPoint *> points;

			DelaunayCircle(double _x, double _y, double _r){
				x = _x;
				y = _y;
				r = _r;
			};
			~DelaunayCircle()
			{
				points.clear();
			};
		};

		class DelaunayPoint
		{
		public :
			double x;
			double y;
			std::vector<DelaunayCircle *> circles;

			DelaunayPoint(double _x, double _y){
				x = _x;
				y = _y;
			};

			~DelaunayPoint()
			{
				circles.clear();
			};
		};

		std::vector<DelaunayPoint> points;
		std::vector<DelaunayCircle> circles;

		void evaluateTriangle(int i, int j, int k);

	public :
		Delaunay();
		~Delaunay();

		void clear();
		//void addPoint(double azimuth, double elevation, bool bottom = 0);
		void addPoint(double abscissa, double ordinate);
		void perform();

		unsigned int getNumberOfCircles() const
		{
			return circles.size();
		};

		double getCircleAbscissa(unsigned int index) const
		{
			assert(index < circles.size());
			return circles[index].x;
		};

		double getCircleOrdinate(unsigned int index) const
		{
			assert(index < circles.size());
			return circles[index].y;
		};

		double getCircleRadius(unsigned int index) const
		{
			assert(index < circles.size());
			return circles[index].r;
		};

		unsigned int getNumberOfPoints() const
		{
			return points.size();
		};

		double getPointAbscissa(unsigned int index) const
		{
			assert(index < points.size());
			return points[index].x;
		};

		double getPointOrdinate(unsigned int index) const
		{
			assert(index < points.size());
			return points[index].y;
		};

		unsigned int getPointNumberOfCircles(unsigned int _index_point) const
		{
			assert(_index_point < points.size());
			return points[_index_point].circles.size();
		};
		/*
		double getPointCircleAzimuth(unsigned int _index_point, unsigned int _index_circle) const
		{
			assert(_index_point < points.size());
			assert(_index_circle < points[_index_point].circles.size());
			if (points[_index_point].circles[_index_circle].x == 0 && points[_index_point].circles[_index_circle].y == 0);
				return 0;
			else
				return atan2(points[_index_point].circles[_index_circle].y, points[_index_point].circles[_index_circle].x);
		};

		double getPointCircleElevation(unsigned int _index_point, unsigned int _index_circle) const
		{
			assert(_index_point < points.size());
			assert(_index_circle < points[_index_point].circles.size());
			double radius = sqrt(points[_index_point].circles[_index_circle].x * points[_index_point].circles[_index_circle].x + points[_index_point].circles[_index_circle].x * points[_index_point].circles[_index_circle].x);
			if (radius <= 0)
				return radius * HOA_PI2;
			else
				return (1. - radius) * HOA_PI2;
		};*/
	};
}

#endif



