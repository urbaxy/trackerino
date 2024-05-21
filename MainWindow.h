#ifndef GTKMM_MAINWINDOW_H
#define GTKMM_MAINWINDOW_H

#include "MainArea.h"
#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/window.h>

class MainWindow : public Gtk::Window
{	public:
		MainWindow ();
		virtual ~MainWindow ();
		
	protected:
		void on_button_clicked ();
		void drag_data_get (const Glib::RefPtr<Gdk::DragContext>& context, Gtk::SelectionData& selection_data, guint info, guint time);
		void drop_drag_data_received (const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);
		Glib::RefPtr<Gtk::TreeStore> m_refTreeModel;
		Gtk::Box m_VBox_main, m_HBox, m_VBox;
		Gtk::Button m_button;
		Gtk::ScrolledWindow m_ScrolledWindow;
		Gtk::TreeView m_TreeView;
		MainArea m_MainArea;
		sql::Connection *connection;
		sql::Driver *driver;
		sql::PreparedStatement *pstmt;
		sql::ResultSet *res;
		sql::Statement *stmt;

		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{	public:
				ModelColumns ()
				{	add (m_col_id);
					add (m_col_name);
				}

				Gtk::TreeModelColumn<int> m_col_id;
				Gtk::TreeModelColumn<Glib::ustring> m_col_name;
		};

		ModelColumns m_Columns;
};

#endif
