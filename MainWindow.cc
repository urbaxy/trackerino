#include "MainWindow.h"
#include <fstream>
#include <iostream>

#include "fit_decode.hpp"

using namespace sql;
using namespace std;

MainWindow::MainWindow ()
{	set_title ("trackerino");
	set_default_size (1200, 800);
	set_border_width (10);

	m_HBox_main.set_orientation (Gtk::ORIENTATION_HORIZONTAL);
	m_HBox_main.set_margin_top (12);
	m_HBox_main.set_margin_right (12);
	m_HBox_main.set_margin_bottom (12);
	m_HBox_main.set_margin_left (12);
	m_HBox_main.set_spacing(6);

	m_HBox.set_orientation (Gtk::ORIENTATION_HORIZONTAL);
	m_HBox.set_margin_top (12);
	m_HBox.set_margin_right (12);
	m_HBox.set_margin_bottom (12);
	m_HBox.set_margin_left (12);
	m_HBox.set_spacing(6);

	m_VBox.set_orientation (Gtk::ORIENTATION_VERTICAL);
	m_VBox.set_margin_top (12);
	m_VBox.set_margin_right (12);
	m_VBox.set_margin_bottom (12);
	m_VBox.set_margin_left (12);
	m_VBox.set_spacing(6);

	m_ScrolledWindow.add (m_TreeView);
	m_ScrolledWindow.set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_refTreeModel = Gtk::TreeStore::create (m_Columns);
	m_TreeView.set_model (m_refTreeModel);

	m_button.add_label ("Click me!", true);
	m_button.signal_clicked ().connect (sigc::mem_fun (*this, &MainWindow::on_button_clicked));

	Gtk::TreeModel::Row row = *(m_refTreeModel->append ());
	row[m_Columns.m_col_id] = 1;
	row[m_Columns.m_col_name] = "Billy Bob";

	Gtk::TreeModel::Row childrow = *(m_refTreeModel->append (row.children ()));
	childrow = *(m_refTreeModel->append (row.children ()));
	childrow[m_Columns.m_col_id] = 12;
	childrow[m_Columns.m_col_name] = "Sue Bob";

	row = *(m_refTreeModel->append ());
	row[m_Columns.m_col_id] = 2;
	row[m_Columns.m_col_name] = "Joey Jojo";

	row = *(m_refTreeModel->append ());
	row[m_Columns.m_col_id] = 3;
	row[m_Columns.m_col_name] = "Rob McRoberts";

	childrow = *(m_refTreeModel->append (row.children ()));
	childrow[m_Columns.m_col_id] = 31;
	childrow[m_Columns.m_col_name] = "Xavier McRoberts";

	m_TreeView.append_column ("ID", m_Columns.m_col_id);
	m_TreeView.append_column ("Name", m_Columns.m_col_name);

	m_MainArea.show ();

	m_HBox_main.pack_start (m_VBox, Gtk::PACK_SHRINK);
	m_HBox_main.pack_start (m_HBox, Gtk::PACK_SHRINK);
	m_VBox.pack_start (m_ScrolledWindow);
	m_VBox.pack_start (m_button, Gtk::PACK_SHRINK);
	m_HBox.pack_start (m_MainArea, Gtk::PACK_SHRINK);

	add (m_HBox_main);

	show_all_children ();

	std::vector<Gtk::TargetEntry> listTargets;
	listTargets.push_back (Gtk::TargetEntry ("STRING"));
	listTargets.push_back (Gtk::TargetEntry ("text/plain"));

	m_button.drag_source_set (listTargets);
	m_button.signal_drag_data_get ().connect (sigc::mem_fun(*this, &MainWindow::drag_data_get));
	m_MainArea.drag_dest_set (listTargets);
	m_MainArea.signal_drag_data_received ().connect(sigc::mem_fun (*this, &MainWindow::drop_drag_data_received) );

	driver = get_driver_instance ();
	connection = driver->connect ("tcp://192.168.0.2:3306", "trackerino", "");
	connection->setSchema ("trackerino");
	stmt = connection->createStatement ();
	stmt->execute ("CREATE TABLE IF NOT EXISTS activities (id INT auto_increment NOT NULL, name VARCHAR(30), CONSTRAINT id_PK PRIMARY KEY (id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci");
	stmt->execute ("CREATE TABLE IF NOT EXISTS points (id INT auto_increment NOT NULL, CONSTRAINT id_PK PRIMARY KEY (id)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci");

	//stmt->execute ("INSERT INTO activities (name) VALUES ('Laufen')");

	pstmt = connection->prepareStatement ("SELECT id, name FROM activities ORDER BY name");
	res = pstmt->executeQuery ();
	res->beforeFirst ();

	while (res->next ())
	{	cout << res->getInt ("id") << " - " << res->getString ("name") <<endl;

		childrow = *(m_refTreeModel->append (row.children ()));
		childrow[m_Columns.m_col_id] = res->getInt ("id");
		childrow[m_Columns.m_col_name] = (Glib::ustring)res->getString ("name");
	}

	delete res;
	delete pstmt;
	delete stmt;
	delete connection;

	MainWindow::importFit ();
}

MainWindow::~MainWindow ()
{
}

void MainWindow::on_button_clicked ()
{	std::cout << "button clicked"  << std::endl;
}

void MainWindow::drag_data_get (const Glib::RefPtr<Gdk::DragContext>&, Gtk::SelectionData& selection_data, guint, guint)
{	selection_data.set (selection_data.get_target (), 8 /* 8 bits format */, (const guchar*)"I'm Data!", 9 /* the length of I'm Data! in bytes */);
}

void MainWindow::drop_drag_data_received (const Glib::RefPtr<Gdk::DragContext>& context, int, int, const Gtk::SelectionData& selection_data, guint, guint time)
{	const int length = selection_data.get_length ();

	if ((length >= 0) && (selection_data.get_format () == 8))
	{	std::cout << "Received \"" << selection_data.get_data_as_string () << "\" in button " << std::endl;
	}

	context->drag_finish (false, false, time);
}

void MainWindow::importFit()
{	fit::Decode decode;
	fit::MesgBroadcaster mesgBroadcaster;

	ifstream fitfile ("D9GB4449.FIT", ios::binary);

	if (fitfile.is_open ())
	{	decode.CheckIntegrity (fitfile);

		mesgBroadcaster.AddListener ((fit::FileIdMesgListener &)fitListener);
		mesgBroadcaster.AddListener ((fit::UserProfileMesgListener &)fitListener);
		mesgBroadcaster.AddListener ((fit::MonitoringMesgListener &)fitListener);
		mesgBroadcaster.AddListener ((fit::DeviceInfoMesgListener &)fitListener);
		mesgBroadcaster.AddListener ((fit::RecordMesgListener&)fitListener);
		mesgBroadcaster.AddListener ((fit::MesgListener &)fitListener);

		try
		{	decode.Read (&fitfile, &mesgBroadcaster, &mesgBroadcaster, &fitListener);
		}

	 	catch (const fit::RuntimeException& e)
		{	printf ("Exception decoding file: %s\n", e.what ());
		}

		catch (...)
		{	printf ("Exception decoding file");
		}

		fitfile.close ();
	}

	else cout << "Unable to open file"; 

	exit (0);
}
