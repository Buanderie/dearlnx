#ifndef __QUADTREE_HPP__
#define __QUADTREE_HPP__

#include <opencv/cv.h>

#include <iostream>

using namespace std;

class QuadTree
{
  public:
    QuadTree( const CvRect& boundingBox, QuadTree* parent=0 )
    {
      _northWest = 0;
      _northEast = 0;
      _southWest = 0;
      _southEast = 0; 
	  _parent = parent;
	  _bbox = boundingBox;
	  _retrieved = false;
    }

    virtual ~QuadTree()
    {
		if( _northWest != 0 ){ delete _northWest; _northWest = 0; }
		if( _northEast != 0 ){ delete _northEast; _northEast = 0; }
		if( _southWest != 0 ){ delete _southWest; _southWest = 0; }
		if( _southEast != 0 ){ delete _southEast; _southEast = 0; }
    }

    double computeOwnCoverage( IplImage* img )
    {
		cvSetImageROI( img, _bbox );
		//CvScalar sc = cvSum( img );
		double popo = cvCountNonZero( img );
		double np = (double)_bbox.width * (double)_bbox.height;
		cvResetImageROI( img );
		//cout << "pol:" << popo << " bak:" << np << endl;
		//cout << "kab:" << _bbox.width << " lop:" << _bbox.height << endl;
		return popo / np;
    }

    double coverage( IplImage* img )
    {
      double res = 0.0;
	  
	  // leaf-case
      if( _northWest == 0 &&
          _northEast == 0 &&
          _southWest == 0 &&
          _southEast == 0 )
      {
        res = computeOwnCoverage( img );
		return res;
      }
      else
      {
		int activeNodes = 0;
		if( _northWest != 0 ){ res += _northWest->coverage( img ); activeNodes++; }
        if( _northEast != 0 ){ res += _northEast->coverage( img ); activeNodes++; }
        if( _southWest != 0 ){ res += _southWest->coverage( img ); activeNodes++; }
        if( _southEast != 0 ){ res += _southEast->coverage( img ); activeNodes++; }
		return res / ((double)(activeNodes));
	  }
    }

	void subdivide( unsigned int depth )
	{
		if( depth == 0 )
		{
			return;
		}
		else
		{
			_northWest = new QuadTree( cvRect( _bbox.x, _bbox.y, _bbox.width / 2.0, _bbox.height / 2.0 ), this );
			_northEast = new QuadTree( cvRect( _bbox.x + _bbox.width / 2.0, _bbox.y, _bbox.width / 2.0, _bbox.height / 2.0 ), this );
			_southWest = new QuadTree( cvRect( _bbox.x, _bbox.y + _bbox.height / 2.0, _bbox.width / 2.0, _bbox.height / 2.0 ), this );
			_southEast = new QuadTree( cvRect( _bbox.x + _bbox.width / 2.0, _bbox.y + _bbox.height / 2.0, _bbox.width / 2.0, _bbox.height / 2.0 ), this );
			_northWest->subdivide( depth - 1 );
			_northEast->subdivide( depth - 1 );
			_southWest->subdivide( depth - 1 );
			_southEast->subdivide( depth - 1 );
		}
	}

	void printShit( IplImage* img )
	{
	  // leaf-case
      if( _northWest == 0 &&
          _northEast == 0 &&
          _southWest == 0 &&
          _southEast == 0 )
      {
		  cvRectangle( img, cvPoint( _bbox.x, _bbox.y ), cvPoint( _bbox.x + _bbox.width, _bbox.y + _bbox.height ), CV_RGB(255,0,0), 1 );
      }
      else
      {
		cvRectangle( img, cvPoint( _bbox.x, _bbox.y ), cvPoint( _bbox.x + _bbox.width, _bbox.y + _bbox.height ), CV_RGB(255,255,0), 1 );
		if( _northWest != 0 ){ _northWest->printShit(img); }
        if( _northEast != 0 ){ _northEast->printShit(img); }
        if( _southWest != 0 ){ _southWest->printShit(img); }
        if( _southEast != 0 ){ _southEast->printShit(img); }
	  }
	}

	void pruneTree( IplImage* img, double coverThreshold )
	{
		if( _northWest == 0 &&
			_northEast == 0 &&
			_southWest == 0 &&
			_southEast == 0 )
		{
			return;
		}
		else
		{
			if( _northWest !=0 && _northWest->coverage( img ) < coverThreshold )
			{
				delete _northWest;
				_northWest = 0;
			}
			else
			{
				_northWest->pruneTree( img, coverThreshold );
			}

			if( _northEast !=0 && _northEast->coverage( img ) < coverThreshold )
			{
				delete _northEast;
				_northEast = 0;
			}
			else
			{
				_northEast->pruneTree( img, coverThreshold );
			}

			if( _southWest !=0 && _southWest->coverage( img ) < coverThreshold )
			{
				delete _southWest;
				_southWest = 0;
			}
			else
			{
				_southWest->pruneTree( img, coverThreshold );
			}

			if( _southEast !=0 && _southEast->coverage( img ) < coverThreshold )
			{
				delete _southEast;
				_southEast = 0;
			}
			else
			{
				_southEast->pruneTree( img, coverThreshold );
			}
			
		}
	}

	void mergeTree( IplImage* img, double coverThreshold )
	{
		// Create leaves list

	}

	std::vector< CvRect > retrieveRegions( IplImage* img )
	{
		std::vector< CvRect > res;

		// leaf case
		if( _northWest == 0 &&
			_northEast == 0 &&
			_southWest == 0 &&
			_southEast == 0 )
		{
			res.push_back( _bbox );
		}
		else 
		{
			if( _northWest != 0 ){ 
				vector< CvRect > sres = _northWest->retrieveRegions( img );
				res.insert( res.end(), sres.begin(), sres.end() );
			}

			if( _northEast != 0 ){ 
				vector< CvRect > sres = _northEast->retrieveRegions( img );
				res.insert( res.end(), sres.begin(), sres.end() );
			}

			if( _southWest != 0 ){ 
				vector< CvRect > sres = _southWest->retrieveRegions( img );
				res.insert( res.end(), sres.begin(), sres.end() );
			}

			if( _southEast != 0 ){ 
				vector< CvRect > sres = _southEast->retrieveRegions( img );
				res.insert( res.end(), sres.begin(), sres.end() );
			}
		}
		
		return res;
	}

	std::vector< CvRect > retrieveRegions2()
	{
		std::vector< CvRect > res;

		return res;
	}

	bool isLeaf()
	{
		return (	_northWest == 0 &&
					_northEast == 0 &&
					_southWest == 0 &&
					_southEast == 0 );
	}

	int refit()
	{
		unsigned count = 0;

		// if complete quad
		if( _northWest != 0 && _northWest->isLeaf() &&
			_northEast != 0 && _northEast->isLeaf() &&
			_southWest != 0 && _southWest->isLeaf() &&
			_southEast != 0 && _southEast->isLeaf() )
		{
			delete _northWest; _northWest = 0;
			delete _southWest; _southWest = 0;
			delete _northEast; _northEast = 0;
			delete _southEast; _southEast = 0;
			count++;
		}
		else
		{
			if( _northWest != 0 )
			{
				count += _northWest->refit();
			}

			if( _northEast != 0 )
			{
				count += _northEast->refit();
			}

			if( _southWest != 0 )
			{
				count += _southWest->refit();
			}

			if( _southEast != 0 )
			{
				count += _southEast->refit();
			}
		}
		return count;
	}

    QuadTree*   _northWest;
    QuadTree*   _northEast;
    QuadTree*   _southWest;
    QuadTree*   _southEast;

	QuadTree*	_parent;
	bool		_retrieved;
	bool		_active;

    CvRect      _bbox;

};

#endif
