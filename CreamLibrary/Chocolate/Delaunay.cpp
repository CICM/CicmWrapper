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
		double abs, ord;
		double ix = points[i].x, iy = points[i].y, jx = points[j].x, jy = points[j].y, kx = points[k].x, ky = points[k].y;
		double D = (ix - kx) * (jy - ky) - (jx - kx) * (iy - ky);
		double circle_abscissa = (((ix - kx) * (ix + kx) + (iy - ky) * (iy + ky)) / 2 * (jy - ky) 
    -  ((jx - kx) * (jx + kx) + (jy - ky) * (jy + ky)) / 2 * (iy - ky))  / D;
		double circle_ordinate = (((jx - kx) * (jx + kx) + (jy - ky) * (jy + ky)) / 2 * (ix - kx)
    -  ((ix - kx) * (ix + kx) + (iy - ky) * (iy + ky)) / 2 * (jx - kx))
    / D;


		
		/*
		double ix = points[i].x, iy = points[i].y, jx = points[j].x, jy = points[j].y, kx = points[k].x, ky = points[k].y;
		
		double tempi = (ix * ix + iy * ix), tempj = (jx * jx + jy * jy), tempk = (kx * kx + ky * ky);
		double tempjk = (jy - ky), tempki = (ky - iy), tempij = (iy - jy);

		double delta = 2. * (ix * tempjk + jx * tempki + kx * tempij);
		double circle_abscissa = (tempi * tempjk + tempj * tempki + tempk * tempij) / delta;
		double circle_ordinate = (tempi * (ky - jy) + tempj * (iy - ky) + tempk * (jy - iy)) / delta;

		abs = circle_abscissa - ix;
		ord = circle_ordinate - iy;
		*/

		/*
		double xba, yba, xca, yca;
		double balength, calength;
		double denominator;
		double xcirca, ycirca;

		xba = points[j].x - points[i].x;
		yba = points[j].y - points[i].y;
		xca = points[k].x - points[i].x;
		yca = points[k].y - points[i].y;

		balength = xba * xba + yba * yba;
		calength = xca * xca + yca * yca;

		#ifdef EXACT
		denominator = 0.5 / orient2d(b, c, a);
		#else
		denominator = 0.5 / (xba * yca - yba * xca);
		#endif

		xcirca = (yca * balength - yba * calength) * denominator;
		ycirca = (xba * calength - xca * balength) * denominator;
		double circle_abscissa = xcirca;
		double circle_ordinate = ycirca;
		*/
		abs = circle_abscissa - points[i].x;
		ord = circle_ordinate - points[i].y;
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
		circle.points.push_back(&points[i]);
		circle.points.push_back(&points[j]);
		circle.points.push_back(&points[k]);
		circles.push_back(circle);
		points[i].circles.push_back(&circle);
		points[j].circles.push_back(&circle);
		points[k].circles.push_back(&circle);
	};    
}

