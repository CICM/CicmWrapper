/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "Delaunay.h"
#define Pi  (3.141592653589793238462643383279502884)

namespace Cicm
{
	Delaunay::Delaunay(){};

	Delaunay::~Delaunay()
	{
		clear();
	};

	void Delaunay::clear()
	{
		points.clear();
		circles.clear();
	};

	void Delaunay::addPoint(double abs, double ord)
	{
		points.push_back(DelaunayPoint(abs, ord));
	}
	/*
	void Delaunay::addPoint(double _azimuth, double _elevation, bool _bottom)
	{
		double abs;
		double ord;
		if((_bottom && _elevation > 0) || (!_bottom && _elevation < 0))
		{
			abs = cos(_azimuth) * (2. - fabs(2. * _elevation / Pi));
			ord = sin(_azimuth) * (2. - fabs(2. * _elevation / Pi));
		}
		else
		{
			abs = cos(_azimuth) * fabs(2. * _elevation / Pi);
			ord = sin(_azimuth) * fabs(2. * _elevation / Pi);
		}
		points.push_back(DelaunayPoint(abs, ord));
	};*/
			
	void Delaunay::perform()
	{
		int size = points.size();
		for(int i = 0; i < size - 2; i++)
		{
			for(int j = i+1; j < size - 1; j++)
			{
				for(int k = j+1; k < size; k++)
				{
					evaluateTriangle(i, j, k);
				}
			}
		}
		for(int i = 0; i < size; i++)
		{
			for(int j = 0; j < points[i].circles.size(); j++)
				points[i].circles[j].pt = &points[i];
			std::sort(points[i].circles.begin(), points[i].circles.end(), DelaunayCircle::compare);
		}
	};

	void Delaunay::evaluateTriangle(int i, int j, int k)
	{
		int size = points.size();
		double abs, ord, circle_abscissa, circle_ordinate;
		double ix = points[i].x, iy = points[i].y, jx = points[j].x, jy = points[j].y, kx = points[k].x, ky = points[k].y;
		double D = (ix - kx) * (jy - ky) - (jx - kx) * (iy - ky);
		if(!D)
			D = 0.0000001;
		
		circle_abscissa = (((ix - kx) * (ix + kx) + (iy - ky) * (iy + ky)) / 2 * (jy - ky) - ((jx - kx) * (jx + kx) + (jy - ky) * (jy + ky)) / 2 * (iy - ky)) / D;
		circle_ordinate = (((jx - kx) * (jx + kx) + (jy - ky) * (jy + ky)) / 2 * (ix - kx) - ((ix - kx) * (ix + kx) + (iy - ky) * (iy + ky)) / 2 * (jx - kx)) / D;
		
		abs = circle_abscissa - ix;
		ord = circle_ordinate - iy;
		double circle_radius = sqrt(abs * abs + ord * ord);

		// If one point is inside the circle, the circle is exclude.
		for(int l = 0; l < size; l++)
		{
			if(l != i && l != j && l != k)
			{
				abs = circle_abscissa - points[l].x;
				ord = circle_ordinate - points[l].y;
				if(sqrt(abs * abs + ord * ord) < circle_radius)
				{
					return;
				}
			}
		}

		// Else, the points save the circle and the circle save the the points.
		DelaunayCircle circle = DelaunayCircle(circle_abscissa, circle_ordinate, circle_radius);
		circles.push_back(circle);
		circle.points.push_back(points[i]);
		circle.points.push_back(points[j]);
		circle.points.push_back(points[k]);
		points[i].circles.push_back(circle);
		points[j].circles.push_back(circle);
		points[k].circles.push_back(circle);
	};    
}

