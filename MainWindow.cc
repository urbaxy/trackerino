#include "MainWindow.h"
#include <iostream>

MainWindow::MainWindow ()
{	set_title ("trackerino");
	set_default_size (600, 600);
	set_border_width (10);

	m_VBox.set_orientation (Gtk::ORIENTATION_VERTICAL);
	m_VBox.set_margin_top (12);
	m_VBox.set_margin_right (12);
	m_VBox.set_margin_bottom (12);
	m_VBox.set_margin_left (12);
	m_VBox.set_spacing(6);

	m_button.add_label ("Click me!", true);
	m_button.signal_clicked ().connect (sigc::mem_fun (*this, &MainWindow::on_button_clicked));

	m_MainArea.show ();

	m_VBox.pack_start (m_MainArea, Gtk::PACK_SHRINK);
	m_VBox.pack_start (m_button, Gtk::PACK_SHRINK);

	add (m_VBox);
 
	show_all_children ();
}

MainWindow::~MainWindow ()
{
}

void MainWindow::on_button_clicked ()
{	std::cout << "button clicked"  << std::endl;
}
