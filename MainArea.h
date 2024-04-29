#include <gtkmm/drawingarea.h>

class MainArea : public Gtk::DrawingArea
{	public:
		MainArea ();
		virtual ~MainArea ();
		
	protected:
		bool on_draw (const Cairo::RefPtr<Cairo::Context>& cr) override;
};
