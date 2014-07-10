/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef __DEF_Voronoy__
#define __DEF_Voronoy__

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
	//! The Voronoy triangulation and Voronoy diagram.
    /**
     */
	class Voronoy
	{
	private :
		class VoronoyPoint;
		class VoronoyCircle
		{
		public :
			double x;
			double y;
			double r;
			VoronoyPoint* pt;
			VoronoyCircle(double _x, double _y, double _r);
            ~VoronoyCircle();
            
			static bool compare(VoronoyCircle c1, VoronoyCircle c2)
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
		};

		class VoronoyPoint
		{
		public :
			double x;
			double y;
			std::vector<VoronoyCircle> circles;
			VoronoyPoint(double _x, double _y);
			~VoronoyPoint();
		};
        
    public :
        enum Mode
        {
            Plane    = 0,
            Circle   = 1,
            Sphere   = 2
        };
    private :

		std::vector<VoronoyPoint> points;
        std::vector<VoronoyPoint> points_bottom;
        Mode  mode;
		void evaluateTriangle(int i, int j, int k, bool top = 1);

	public :
		Voronoy(Mode mode);
		~Voronoy();

		void clear();
        void addPointCartesian(double abscissa, double ordinate, double height = 0);
        void addPointPolar(double radius, double azimuth, double elevation = 0);
		void perform();

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

		unsigned int getPointVoronoyLenght(unsigned int index) const
		{
			assert(index < points.size());
			return points[index].circles.size();
		};

		double getPointVoronoyAbscissa(unsigned int index, unsigned int index2) const
		{
			assert(index < points.size());
			assert(index2 < points[index].circles.size());
			return  points[index].circles[index2].x;
		};

		double getPointVoronoyOrdinate(unsigned int index, unsigned int index2) const
		{
			assert(index < points.size());
			assert(index2 < points[index].circles.size());
			return points[index].circles[index2].y;
		};
	};
}

#endif



