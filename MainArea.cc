#include "MainArea.h"
#include <iostream>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>

MainArea::MainArea ()
{	set_size_request (500, 500);
}

MainArea::~MainArea ()
{
}

bool MainArea::on_draw (const Cairo::RefPtr<Cairo::Context>& cr)
{	Glib::RefPtr<Gdk::Pixbuf> image = Gdk::Pixbuf::create_from_file ("map.png");
	Gdk::Cairo::set_source_pixbuf (cr, image, 100, 80);
	cr->rectangle (110, 90, image->get_width ()-20, image->get_height ()-20);
	cr->fill ();

	Gtk::Allocation allocation = get_allocation ();
	const int width = allocation.get_width ();
	const int height = allocation.get_height ();

	// coordinates for the center of the window
	int xc, yc;
	xc = width / 2;
	yc = height / 2;

	cr->set_line_width (10.0);

	// draw red lines out from the center of the window
	cr->set_source_rgb (0.8, 0.0, 0.0);
	cr->move_to (0, 0);
	cr->line_to (xc, yc);
	cr->line_to (0, height);
	cr->move_to (xc, yc);
	cr->line_to (width, yc);
	//cr->stroke ();

	return true;
}
