////////////////////////////////////////
//  File      : launcher.cpp          //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "launcher.h"

////////////////////////////////////////

Launcher::Launcher(Antico *a, QWidget *parent) : QLabel(parent)
{
    app = a;
    read_settings();
    init();
    setPixmap(QPixmap(launcher_pix).scaled(dock_height-5, dock_height-5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    show();
}

Launcher::~Launcher()
{
    delete app;
    delete &launcher_pix;
    delete &quit_pix;
    delete &shutdown_pix;
    delete &restart_pix;
    delete &refresh_pix;
    delete &show_pix;
    delete &run_pix;
    delete &manager_pix;
    delete &dock_position;
    delete &dock_height;
}

void Launcher::init()
{
    main_menu = new QMenu(this);
    connect(main_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_command(QAction *))); // Quit, Run, Refresh, Manager

    quit = new QAction(tr("Quit WM"), this);
    shutdown = new QAction(tr("Shutdown PC"), this);
    restart = new QAction(tr("Restart PC"), this);
    refresh = new QAction(tr("Refresh WM"), this);
    run = new QAction(tr("Run..."), this);
    manag = new QAction(tr("Manager"), this);
    show_desk = new QAction(tr("Show Desktop"), this);
    quit->setIcon(QIcon(quit_pix));
    quit->setData("quit");
    shutdown->setIcon(QIcon(shutdown_pix));
    shutdown->setData("shutdown");
    restart->setIcon(QIcon(restart_pix));
    restart->setData("restart");
    refresh->setIcon(QIcon(refresh_pix));
    refresh->setData("refresh");
    show_desk->setIcon(QIcon(show_pix));
    show_desk->setData("show");
    run->setIcon(QIcon(run_pix));
    run->setData("run");
    manag->setIcon(QIcon(manager_pix));
    manag->setData("manager");
    ////////////
    QList <QMenu *> menu_list = app->get_category_menu()->get_menus();
    for (int i = 0; i <  menu_list.size(); ++i)
    {
        // main_menu->addMenu(menu_list.at(i)); // add Category menu on Launcher
    }
    main_menu->addSeparator();
    main_menu->addAction(manag);
    main_menu->addAction(run);
    main_menu->addAction(show_desk);
    main_menu->addAction(quit);
    main_menu->addAction(shutdown);
    main_menu->addAction(restart);
    main_menu->addAction(refresh);
}

void Launcher::read_settings()
{
    // get style path
    antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get action icons
    style = new QSettings(stl_path + stl_name, QSettings::IniFormat, this);
    style->beginGroup("Launcher");
    launcher_pix = stl_path + style->value("launcher_pix").toString();
    quit_pix = stl_path + style->value("quit_pix").toString();
    shutdown_pix = stl_path + style->value("shutdown_pix").toString();
    restart_pix = stl_path + style->value("restart_pix").toString();
    refresh_pix = stl_path + style->value("refresh_pix").toString();
    show_pix = stl_path + style->value("show_pix").toString();
    run_pix = stl_path + style->value("run_pix").toString();
    manager_pix = stl_path + style->value("manager_pix").toString();
    style->endGroup(); // Launcher
    style->beginGroup("Dockbar");
    dock_height = style->value("dock_height").toInt();
    dock_position = style->value("dock_position").toInt();
    style->endGroup(); //Dockbar
}

void Launcher::run_command(QAction *act)
{
    QString cmd = act->data().toString();

    if (cmd == "quit")
        app->wm_quit();
    if (cmd == "shutdown")
        app->wm_shutdown();
    if (cmd == "restart")
        app->wm_restart();
    if (cmd == "refresh")
        app->wm_refresh();
    if (cmd == "show")
        app->show_desktop();
    if (cmd == "manager")
        new Manager();
    if (cmd == "run")
        new Runner();
}

void Launcher::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPoint p = mapToGlobal(QPoint(0, 0));
        
        if (dock_position == 0) // 0 = bottom / 1 = top
            p.setY(p.y()-main_menu->sizeHint().height());
        else
            p.setY(p.y()+height());
            
        main_menu->popup(p);
    }
}

void Launcher::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    setPixmap(QPixmap(launcher_pix).scaled(dock_height, dock_height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void Launcher::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    setPixmap(QPixmap(launcher_pix).scaled(dock_height-5, dock_height-5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void Launcher::update_style()
{
    read_settings();
    setPixmap(QPixmap(launcher_pix).scaled(dock_height-5, dock_height-5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    quit->setIcon(QIcon(quit_pix));
    shutdown->setIcon(QIcon(shutdown_pix));
    restart->setIcon(QIcon(restart_pix));
    refresh->setIcon(QIcon(refresh_pix));
    show_desk->setIcon(QIcon(show_pix));
    run->setIcon(QIcon(run_pix));
    manag->setIcon(QIcon(manager_pix));
    app->get_category_menu()->update_menu(); // update .desktop/user menu entry
}




