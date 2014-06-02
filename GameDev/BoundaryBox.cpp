#include "BoundaryBox.h"

BoundaryBox::BoundaryBox(const int& t)
   : _type(t)
{
}
BoundaryBox::~BoundaryBox()
{
}

int BoundaryBox::type() const
{
   return _type;
}

CircleBox::CircleBox()
   : BoundaryBox(BBOX_CIRCLE)
{
}
CircleBox::~CircleBox()
{
}

void CircleBox::Draw(HDC hdc)
{
   ::Ellipse(hdc, center.x-radius, center.y-radius,
      center.x+radius, center.y+radius);
}
bool CircleBox::IsCollide(BoundaryBox* other)
{
   if (other == NULL) return false;

   if (other->type() == BBOX_CIRCLE)
   {
      Point diff = center - dynamic_cast<CircleBox*>(other)->center;

      float dist = sqrt(float(diff.x*diff.x + diff.y*diff.y));

      std::wostringstream oss;
      oss << _T("c : ") << center.x << _T(", ") << center.y << _T("\n");
      oss << _T("d : ") << dynamic_cast<CircleBox*>(other)->center.x;
      oss << _T(", ") << dynamic_cast<CircleBox*>(other)->center.y << _T("\n");
      oss << _T("dist : ") << dist << _T("\n");
      ::OutputDebugString(oss.str().c_str());
      return (dist <= radius + dynamic_cast<CircleBox*>(other)->radius);
   }
   else if (other->type() == BBOX_SQUARE)
   {
      Rect rcBox = dynamic_cast<SquareBox*>(other)->getBox();
      Point diff = center - rcBox.center();

      if (abs(diff.x) > radius + rcBox.width()/2 || 
          abs(diff.y) > radius + rcBox.height()/2)
         return false;

      if (abs(diff.x) <= rcBox.width()/2 ||
         abs(diff.y) <= rcBox.height()/2)
         return true;

      float dist = sqrt(float(diff.x*diff.x + diff.y*diff.y));

      return (dist <= radius + rcBox.radius());
   }

   return false;
}
void CircleBox::SetPosition(const Point& pt)
{
   center = pt;
}
void CircleBox::SetRadius(const LONG& r)
{
   radius = r;
}

SquareBox::SquareBox()
   : BoundaryBox(BBOX_SQUARE)
{
}
SquareBox::~SquareBox()
{
}

void SquareBox::Draw(HDC hdc)
{
	::MoveToEx(hdc, rcBox.left, rcBox.top, NULL);
	::LineTo(hdc, rcBox.left, rcBox.bottom);
	::LineTo(hdc, rcBox.right, rcBox.bottom);
	::LineTo(hdc, rcBox.right, rcBox.top);
	::LineTo(hdc, rcBox.left, rcBox.top);
}
BYTE SquareBox::BoxIsCollide(BoundaryBox* other, const bool& back)
{
   if (other == NULL) return 10;

   if (other->type() == BBOX_SQUARE)
   {
      Rect oBox = dynamic_cast<SquareBox*>(other)->getBox();
      /*return !(rcBox.left > oBox.right ||
               rcBox.right < oBox.left ||
               rcBox.top > oBox.bottom ||
               rcBox.bottom < oBox.top);*/
	  Rect tmp;
	  if(IntersectRect(&tmp, &rcBox, &oBox))
	  {
		  if(rcBox.left < oBox.right && rcBox.right > oBox.right && back)
		  {
			  return 1;
		  }
		  else if(rcBox.right > oBox.left && rcBox.left < oBox.left && !back)
		  {
			  return 2;
		  } 
		  else if(rcBox.bottom > oBox.top && rcBox.top < oBox.top)
		  {
			  return 0;
		  }
		  else if(rcBox.top < oBox.bottom)
		  {
			  return 3;
		  }
	  }
	  else
		  return 4;
   }
   else if (other->type() == BBOX_CIRCLE)
   {
      /*return other->IsCollide(this);*/
	   return 0;
   }

   return 4;
}

Rect SquareBox::getBox() const
{
   return rcBox;
}
void SquareBox::SetPosition(const Point& pt)
{
   rcBox = rcBox >> Size(pt.x, pt.y) << Size(rcBox.width()/2, rcBox.height()/2);
}
void SquareBox::SetRect(const Rect& rc)
{
   rcBox = rc;
}
bool SquareBox::IsCollide(BoundaryBox* other)
{
	return true;
}