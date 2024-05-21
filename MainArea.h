#ifndef GTKMM_MAINAREA_H
#define GTKMM_MAINAREA_H

#include <gtkmm/drawingarea.h>

class MainArea : public Gtk::DrawingArea
{	public:
		MainArea ();
		virtual ~MainArea ();
		
	protected:
		bool on_draw (const Cairo::RefPtr<Cairo::Context>& cr) override;
};


#endif