#include <iostream>
#include <fstream>
#include <cmath>

#include <plotter.h>
#include "gnuplot-iostream.h"

// Adapter examples

// 1) you decide to replace your old, time-worn (or maybe even not working) library
// with something more up-to-date you face the problem of replacing all library calls throughout the code.
// The solution to this is to use Adapter Design Pattern, and wrap new library into old interface.

// Legacy plotter
class BuggyPlotter //use Gnuplot
{
public:
	void drawCircle(int x, int y, int radius)
	{
	    Gnuplot gp;

		std::vector<std::tuple<double, double, double> > circle;

		circle.push_back(std::tuple<double, double, double>{x, y, radius});

		// Don't forget to put "\n" at the end of each line!
		gp << "set xrange [0:10]\nset yrange [0:10]\n";
		// '-' means read from stdin.  The send1d() function sends data to
		// gnuplot's stdin.
		gp << "plot '-' with circles\n";
		gp.send1d(circle);
	}
};

class RockingCirclePlotter //use XPlotter, no GnuPlot construction each time, we are able to prepare plot differently
{
public:
	RockingCirclePlotter() : plotter(params)
	{}
		
	void prepare()
	{
		if (plotter.openpl () < 0)                  // open Plotter
		{
			cerr << "Couldn't open Plotter\n";
			exit(1);
		}

		plotter.fspace (0.0, 0.0, 10.0, 10.0); // specify user coor system
		plotter.bgcolorname("black");
		plotter.flinewidth (0.25);       // line thickness in user coordinates
		plotter.pencolorname ("red");    // path will be drawn in red
		plotter.erase();                // erase Plotter's graphics display
	}
	void draw(int x, int y, int radius)
	{
		plotter.circle(x, y, radius);
	}
private:
	PlotterParams params;
	XPlotter plotter;
};


class BuggyPlotterAdapter : public BuggyPlotter, private RockingCirclePlotter
{
public:
	void drawCircle(int x, int y, int radius)
	{
		RockingCirclePlotter::prepare();
		RockingCirclePlotter::draw(x, y, radius);
	}
};


void replaceOldLibrary()
{
	//old library
	//~ BuggyPlotter plotter;
	//~ plotter.drawCircle(5, 5, 3);
	
	//new library
	BuggyPlotterAdapter plotter;
	plotter.drawCircle(5, 5, 3);
	
}





// 2) we want to use some class but it has a incompatible (or inconvienient) interface.
// In example we want to call:
//      prepare()
//      line(Point start, Point end)
//      circle(Point center, int radius)
// but the library needs:
//      ... a lot of preparing calls...
//      line(int, int, int, int)
//      circle(int, int, int)

struct Point {
    int x;
    int y;
};

struct Line {
    Point start;
    Point end;
};

class TargetInterface
{
public:
    virtual ~TargetInterface() = default;
    virtual void prepare() = 0;
    virtual void circle(const Point& p, const int radius) = 0;
    virtual void line(const Line& l) = 0;
};

class PlotterAdapter : public TargetInterface, private XPlotter
{
public:
    PlotterAdapter(PlotterParams& params) : XPlotter(params)
    {
    }
    virtual void prepare() override
    {
        if (XPlotter::openpl () < 0)                  // open Plotter
        {
            cerr << "Couldn't open Plotter\n";
            exit(1);
        }

        XPlotter::fspace (0.0, 0.0, 300.0, 300.0); // specify user coor system
        XPlotter::bgcolorname("black");
        XPlotter::flinewidth (0.25);       // line thickness in user coordinates
        XPlotter::pencolorname ("red");    // path will be drawn in red
        XPlotter::erase();                // erase Plotter's graphics display
        XPlotter::fmove (600.0, 300.0);    // position the graphics cursor
    }

    virtual void circle(const Point& p, const int radius) override
    {
        XPlotter::circle(p.x, p.y, radius);
    }

    virtual void line(const Line& l) override
    {
        XPlotter::line(l.start.x, l.start.y, l.end.x, l.end.y);
    }
};

void adjustedInterface()
{
	// set a Plotter parameter
    PlotterParams params;
	
	Point src {100, 100};
    Point dst {150, 150};
    Line line {src, dst};
	
	//old plotting interface
    //~ XPlotter plotter(params);

	//~ if (plotter.openpl () < 0)                  // open Plotter
	//~ {
		//~ cerr << "Couldn't open Plotter\n";
		//~ exit(1);
	//~ }

	//~ plotter.fspace (0.0, 0.0, 300.0, 300.0); // specify user coor system
	//~ plotter.bgcolorname("black");
	//~ plotter.flinewidth (0.25);       // line thickness in user coordinates
	//~ plotter.pencolorname ("red");    // path will be drawn in red
	//~ plotter.erase();                // erase Plotter's graphics display
	//~ plotter.fmove (600.0, 300.0);    // position the graphics cursor

    //~ plotter.circle(src.x, src.y, 50);
    //~ plotter.circle(dst.x, dst.y, 50);
    //~ plotter.line(line.start.x, line.start.y, line.end.x, line.end.y);


	//new, adapted plotter (better interface)
    PlotterAdapter plotter(params);

    plotter.prepare();

    //~ //new interface
    plotter.circle(src, 50);
    plotter.circle(dst, 50);
    plotter.line(line);	
}

int main(int argc, char* argv[])
{
	adjustedInterface();
	replaceOldLibrary();
}
