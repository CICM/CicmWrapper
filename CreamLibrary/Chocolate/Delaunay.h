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
#include <algorithm>
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
			std::vector<DelaunayPoint> points;
			DelaunayPoint* pt;

			DelaunayCircle(double _x, double _y, double _r)
			{
				x = _x;
				y = _y;
				r = _r;
			};

			static bool compare(DelaunayCircle c1, DelaunayCircle c2)
			{
				double azi1, azi2, x = 0, y = 0;
				if(c1.pt)
				{
					x = c1.pt->x;
					y = c1.pt->y;
				}
				if(c1.x - x == 0 && c1.y - y == 0)
					azi1 = 0;
				else
					azi1 = atan2(c1.y - y, c1.x -x);
				if(c2.x - x == 0 && c2.y - y == 0)
					azi2 = 0;
				else
					azi2 = atan2(c2.y - y, c2.x - x);
				return azi1 < azi2;
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
			std::vector<DelaunayCircle> circles;

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

		unsigned int getPointNumberOfCircles(unsigned int index) const
		{
			assert(index < points.size());
			return points[index].circles.size();
		};

		double getPointCircleAbscissa(unsigned int index, unsigned int index2) const
		{
			assert(index < points.size());
			assert(index2 < points[index].circles.size());
			return  points[index].circles[index2].x;
		};

		double getPointCircleOrdinate(unsigned int index, unsigned int index2) const
		{
			assert(index < points.size());
			assert(index2 < points[index].circles.size());
			return points[index].circles[index2].y;
		};
	};
}

#endif



