#define _USE_MATH_DEFINES

#include "observateur.h"
#include "Utils.h"
#include <math.h>
#include <stdio.h>


int g_obsMode = DEFAULT_OBS_MODE;

double g_polarDistance	= 0.0;
double g_polarAzimut	= 0.0;
double g_polarElevation = 0.0;
double g_polarTwist		= 0.0;

double g_planeX		    = 0.0;
double g_planeY		    = 0.0;
double g_planeZ		    = 0.0;
double g_planeRoulis	= 0.0;
double g_planeTangage	= 0.0;
double g_planeLacet		= 0.0;

void initView() {
	g_polarDistance	 = DEFAULT_POLAR_DISTANCE;
	g_polarAzimut	 = DEFAULT_POLAR_AZIMUT;
	g_polarElevation = DEFAULT_POLAR_ELEVATION;
	g_polarTwist	 = DEFAULT_POLAR_TWIST;
	
	g_planeX	     = DEFAULT_PLANE_X;
	g_planeY	     = DEFAULT_PLANE_Y;
	g_planeZ	     = DEFAULT_PLANE_Z;
	g_planeRoulis	 = DEFAULT_PLANE_ROULIS;
	g_planeTangage	 = DEFAULT_PLANE_TANGAGE;
	g_planeLacet	 = DEFAULT_PLANE_LACET;

	recomputeView();
}

void changeMode() {
	if(g_obsMode == OBS_MODE_PILOT_VIEW) {
		g_obsMode = OBS_MODE_POLAR_VIEW;
		glutSetWindowTitle("Polar View");
	} else {
		g_obsMode = OBS_MODE_PILOT_VIEW;
		glutSetWindowTitle("Pilot View");
	}

	recomputeView();
}

void polarView(double distance, double azimut, double elevation, double twist) {
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -distance);
	glRotated(180,				0.0, 1.0, 0.0);
	glRotated(-90 - elevation,  1.0, 0.0, 0.0);
	glRotated((90 - azimut),	0.0, 0.0, 1.0);
	glRotated(twist,			1.0, 0.0, 0.0);
}

void pilotView(double planeX, double planeY, double planeZ, double roulis, double tangage, double lacet) {
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	glRotated(roulis,	0.0, 0.0, 1.0);
	glRotated(lacet,	1.0, 0.0, 0.0);
	glRotated(tangage,	0.0, 1.0, 0.0);
	glTranslated(-planeX, -planeY, -planeZ);
}

void recomputeView() {
	if(g_obsMode == OBS_MODE_POLAR_VIEW) {
		polarView(g_polarDistance, g_polarAzimut, g_polarElevation, g_polarTwist);
	} else if(g_obsMode == OBS_MODE_PILOT_VIEW) {
		pilotView(g_planeX, g_planeY, g_planeZ, g_planeRoulis, g_planeTangage, g_planeLacet);
	}
}

void rotateZ(double _az) {
	double newX, newY;

	if(g_obsMode == OBS_MODE_POLAR_VIEW) {
		g_polarAzimut = fmod(g_polarAzimut + _az, 360);
	} else if(g_obsMode == OBS_MODE_PILOT_VIEW) {
		g_planeLacet  = fmod(g_planeLacet - _az, 360);
		
		newX = g_planeX * cos(_az * M_PI / 180.0) - g_planeY * sin(_az * M_PI / 180.0);
		newY = g_planeX * sin(_az * M_PI / 180.0) + g_planeY * cos(_az * M_PI / 180.0);

		g_planeX = newX;
		g_planeY = newY;

		//printf("az : %f, planeX : %f, planeY : %f, lacet : %f \n", _az, g_planeX, g_planeY, g_planeLacet);
	}
	recomputeView();
}

void rotateHead(double _az) {
	if(g_obsMode == OBS_MODE_POLAR_VIEW) {
		g_polarAzimut = fmod(g_polarAzimut + _az, 360);
	} else if(g_obsMode == OBS_MODE_PILOT_VIEW) {
		g_planeLacet += _az;
	}
	recomputeView();
}

void nodHead(double _az) {
	if(g_obsMode == OBS_MODE_POLAR_VIEW) {
		g_polarElevation = fmod(g_polarElevation + _az, 360);
	} else if(g_obsMode == OBS_MODE_PILOT_VIEW) {
		g_planeTangage += _az;
	}
	recomputeView();
}

void zoom(double _d) {
    if(g_obsMode == OBS_MODE_POLAR_VIEW) {
        g_polarDistance = max(g_polarDistance + _d, 0.1);
	} else if(g_obsMode == OBS_MODE_PILOT_VIEW) {
		g_planeX = max(g_planeX + g_planeX * _d / getPlaneDistance(), 0.1);
        g_planeY = max(g_planeY + g_planeY * _d / getPlaneDistance(), 0.1);
	}
	recomputeView();
}

double getPlaneDistance() {
    return sqrt(g_planeX * g_planeX + g_planeY * g_planeY + g_planeZ * g_planeZ);
}

void translateX(double _x) {
    if(g_obsMode == OBS_MODE_POLAR_VIEW) {
        double z      = g_polarDistance * sin(g_polarElevation * M_PI / 180);
        double xyDist = g_polarDistance * cos(g_polarElevation * M_PI / 180);
        double x      = xyDist * cos(g_polarAzimut * M_PI / 180);
        double y      = xyDist * sin(g_polarAzimut * M_PI / 180);
        x += _x;
        g_polarAzimut   = atan2(y, x) * 180 / M_PI;
        g_polarDistance = sqrt(x * x + y * y + z * z);
	} else if(g_obsMode == OBS_MODE_PILOT_VIEW) {
		g_planeX += _x;
	}
	recomputeView();
}

void translateY(double _y) {
    if(g_obsMode == OBS_MODE_POLAR_VIEW) {
        double z      = g_polarDistance * sin(g_polarElevation * M_PI / 180);
        double xyDist = g_polarDistance * cos(g_polarElevation * M_PI / 180);
        double x      = xyDist * cos(g_polarAzimut * M_PI / 180);
        double y      = xyDist * sin(g_polarAzimut * M_PI / 180);
        y += _y;
        g_polarAzimut   = atan2(y, x) * 180 / M_PI;
        g_polarDistance = sqrt(x * x + y * y + z * z);
	} else if(g_obsMode == OBS_MODE_PILOT_VIEW) {
        g_planeY += _y;
	}
	recomputeView();
}
