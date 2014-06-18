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
	};

	void Delaunay::evaluateTriangle(int i, int j, int k)
	{
		int size = points.size();
		double abs, ord, dist_ij, dist_ik, dist_jk, angle;
		double ix = points[i].x, iy = points[i].y, jx = points[j].x, jy = points[j].y, kx = points[k].x, ky = points[k].y;
		
		abs = (jx - ix);
		ord = (jy - iy);
		dist_ij = sqrt(abs * abs + ord * ord);
		abs = (kx - ix);
		ord = (ky - iy);
		dist_ik = sqrt(abs * abs + ord * ord);
		abs = (kx - jx);
		ord = (ky - jy);
		dist_jk = sqrt(abs * abs + ord * ord);
		angle =  acos((dist_ij * dist_ij + dist_ik * dist_ik - dist_jk * dist_jk) / (2 * dist_ij * dist_ik));

		double circle_radius = dist_ij / ( 2. * sin(angle));
		//double circle_abscissa = points[i].x + (points[j].x - points[i].x) * 0.5;
		//double circle_ordinate = circle_radius + points[i].y;
		double delta = 2. * (ix * (jy - ky) + jx * (ky - iy) + kx * (iy - jy));
		double circle_abscissa = ((ix * ix + iy * ix) * (jy - ky) + (jx * jx + jy * jy) * (ky - iy) + (kx * kx + ky * ky) * (iy - jy)) / delta;
		double circle_ordinate = ((ix * ix + iy * ix) * (ky - jy) + (jx * jx + jy * jy) * (iy - ky) + (kx * kx + ky * ky) * (jy - iy)) / delta;
		post("abs %f ord %f", circle_abscissa, circle_ordinate);
		// If one point is inside the circle, the circle is exclude.
		for(int l = 0; l < size; l++)
		{
			abs = circle_abscissa - points[l].x;
			ord = circle_ordinate - points[l].y;
			if(sqrt(abs * abs + ord * ord) < circle_radius)
			{
				return;
			}
		}

		// Else, the points save the circle and the circle save the the points.
		DelaunayCircle circle = DelaunayCircle(circle_abscissa, circle_ordinate, circle_radius);
		circle.points.push_back(&points[i]);
		circle.points.push_back(&points[j]);
		circle.points.push_back(&points[k]);
		circles.push_back(circle);
		points[i].circles.push_back(&circle);
		points[j].circles.push_back(&circle);
		points[k].circles.push_back(&circle);
	};    
}

