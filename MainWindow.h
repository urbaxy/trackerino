#include "MainArea.h"
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

class MainWindow : public Gtk::Window
{	public:
		MainWindow ();
		virtual ~MainWindow ();
		
	protected:
		void on_button_clicked ();
		Gtk::Box m_VBox;
		Gtk::Button m_button;
		MainArea m_MainArea;
};
