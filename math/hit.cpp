#include "bbox.h"
#include "seg.h"

namespace math {

//-BBOX-VS-SEG---------------------------------------------------------------//

bool hit(const bbox3f &bb, const seg3f &s) {return cohen_sutherland(bb, s);}
bool hit(const bbox3d &bb, const seg3d &s) {return cohen_sutherland(bb, s);}
bool hit(const bbox2f &bb, const seg2f &s) {return cohen_sutherland(bb, s);}
bool hit(const bbox2d &bb, const seg2d &s) {return cohen_sutherland(bb, s);}

// cohen-sutherland clipping outcodes
#define PLA_DRET	(1<<0)
#define PLA_ESQUERRA	(1<<1)
#define PLA_SUPERIOR	(1<<2)
#define PLA_INFERIOR	(1<<3)
#define PLA_FRONTAL	(1<<4)
#define PLA_DARRERE	(1<<5)

template <typename Real>
unsigned long outcode(const bbox<vec3<Real> > &b, const vec3<Real> &pnt)
{
	unsigned long outcode = 0;

	     if( pnt.x > b.max.x ) outcode |= PLA_DRET;
	else if( pnt.x < b.min.x ) outcode |= PLA_ESQUERRA;
	     if( pnt.y > b.max.y ) outcode |= PLA_SUPERIOR;
	else if( pnt.y < b.min.y ) outcode |= PLA_INFERIOR;
	     if( pnt.z > b.max.z ) outcode |= PLA_DARRERE;
	else if( pnt.z < b.min.z ) outcode |= PLA_FRONTAL;

	return outcode;
}

template <typename Real>
unsigned long outcode(const bbox<vec2<Real> > &b, const vec2<Real> &pnt)
{
	unsigned long outcode = 0;

	     if( pnt.x > b.max.x ) outcode |= PLA_DRET;
	else if( pnt.x < b.min.x ) outcode |= PLA_ESQUERRA;
	     if( pnt.y > b.max.y ) outcode |= PLA_SUPERIOR;
	else if( pnt.y < b.min.y ) outcode |= PLA_INFERIOR;

	return outcode;
}

template <typename Real>
bool cohen_sutherland(const bbox<vec3<Real> > &bb, const seg<vec3<Real> > &s)
{
	//Segment: p1 <--> p2, rec: Punt interseccio. 
	vec3<Real> p1 = s.b, p2 = s.e, rec;
	unsigned long outcode1, outcode2;

	// Punt p1 interior a la bounding box
	outcode1 = outcode( bb, p1 );
	if( outcode1 == 0 ) return true;

	// Punt p2 interior a la bounding box
	outcode2 = outcode( bb, p2 );
	if( outcode2 == 0 ) return true;

	// Tots dos punts estan al mateix costat de la bounding box
	if( (outcode1 & outcode2) > 0 ) return false;

	// mirem interseccions
	if( outcode1 & (PLA_DRET | PLA_ESQUERRA) )
	{
		if( outcode1 & PLA_DRET ) rec.x = bb.max.x;
		else                      rec.x = bb.min.x;

		float x1 = p2.x  - p1.x;
		float x2 = rec.x - p1.x;
		rec.y = p1.y + x2 * (p2.y - p1.y) / x1;
		rec.z = p1.z + x2 * (p2.z - p1.z) / x1;

		if( rec.y <= bb.max.y &&
		    rec.y >= bb.min.y &&
		    rec.z <= bb.max.z &&
		    rec.z >= bb.min.z ) {
			return true;
		}
	}

	if( outcode1 & (PLA_SUPERIOR | PLA_INFERIOR) )
	{
		if( outcode1 & PLA_SUPERIOR ) rec.y = bb.max.y;
		else                          rec.y = bb.min.y;

		float y1 = p2.y  - p1.y;
		float y2 = rec.y - p1.y;
		rec.x = p1.x + y2 * (p2.x - p1.x) / y1;
		rec.z = p1.z + y2 * (p2.z - p1.z) / y1;

		if( rec.x <= bb.max.x &&
		    rec.x >= bb.min.x &&
		    rec.z <= bb.max.z &&
		    rec.z >= bb.min.z ) {
			return true;
		}
	}

	if( outcode1 & (PLA_FRONTAL | PLA_DARRERE) )
	{
		if( outcode1 & PLA_DARRERE ) rec.z = bb.max.z;
		else                         rec.z = bb.min.z;

		float z1 = p2.z  - p1.z;
		float z2 = rec.z - p1.z;
		rec.x = p1.x + z2 * (p2.x - p1.x) / z1;
		rec.y = p1.y + z2 * (p2.y - p1.y) / z1;

		if( rec.x <= bb.max.x &&
		    rec.x >= bb.min.x &&
		    rec.y <= bb.max.y &&
		    rec.y >= bb.min.y ) {
			return true;
		}
	}

	return false;
}

template <typename Real>
bool cohen_sutherland(const bbox<vec2<Real> > &bb, const seg<vec2<Real> > &s)
{
	//Segment: p1 <--> p2, rec: Punt interseccio. 
	vec2<Real> p1 = s.b, p2 = s.e, rec;
	unsigned long outcode1, outcode2;

	// Punt p1 interior a la bounding box
	outcode1 = outcode( bb, p1 );
	if( outcode1 == 0 ) return true;

	// Punt p2 interior a la bounding box
	outcode2 = outcode( bb, p2 );
	if( outcode2 == 0 ) return true;

	// Tots dos punts estan al mateix costat de la bounding box
	if( (outcode1 & outcode2) > 0 ) return false;

	// mirem interseccions
	if( outcode1 & (PLA_DRET | PLA_ESQUERRA) )
	{
		if( outcode1 & PLA_DRET ) rec.x = bb.max.x;
		else                      rec.x = bb.min.x;

		float x1 = p2.x  - p1.x;
		float x2 = rec.x - p1.x;
		rec.y = p1.y + x2 * (p2.y - p1.y) / x1;

		if( rec.y <= bb.max.y && rec.y >= bb.min.y ) return true;
	}

	if( outcode1 & (PLA_SUPERIOR | PLA_INFERIOR) )
	{
		if( outcode1 & PLA_SUPERIOR ) rec.y = bb.max.y;
		else                          rec.y = bb.min.y;

		float y1 = p2.y - p1.y;
		float y2 = rec.y - p1.y;
		rec.x = p1.x + y2 * (p2.x - p1.x) / y1;

		if( rec.x <= bb.max.x && rec.x >= bb.min.x ) return true;
	}

	return false;
}

//Function Template Instantiation
template bool cohen_sutherland<float>(const bbox3f &bb, const seg3f &r);
template bool cohen_sutherland<float>(const bbox2f &bb, const seg2f &r);
template bool cohen_sutherland<double>(const bbox3d &bb, const seg3d &r);
template bool cohen_sutherland<double>(const bbox2d &bb, const seg2d &r);

} //namespace math
