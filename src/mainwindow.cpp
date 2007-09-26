/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
  Copyright (c) 2007 Riccardo Iaconelli <ruphy@fsfe.org>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "mainwindow.h"

#include <QResizeEvent>
#include <QHBoxLayout>

#include <KAction>
#include <KActionCollection>
#include <KConfigDialog>
#include <KLocalizedString>
#include <KStandardAction>
#include <KStandardGameAction>
#include <KStatusBar>

#include <kggzmod/module.h>
#include <kggzmod/player.h>
#include <kggznet/kggzraw.h>

#include "playfield.h"
#include "settings.h"

MainWindow::MainWindow()
: m_mod(0), m_raw(0), m_fd(-1), m_ggzsetup(false)
{
    m_main = new PlayField(this, statusBar());
    
    setCentralWidget(m_main);

    setupActions();

    if(KGGZMod::Module::isGGZ())
    {
        kDebug() << "GGZDEBUG: running in GGZ mode...";
        m_mod = new KGGZMod::Module("KBattleship");
        connect(m_mod, SIGNAL(signalError()), SLOT(networkErrorHandler()));
        connect(m_mod, SIGNAL(signalNetwork(int)), SLOT(networkData(int)));
        connect(m_mod, SIGNAL(signalEvent(const KGGZMod::Event&)),
            SLOT(networkEvent(const KGGZMod::Event&)));
        kDebug() << "GGZDEBUG: kggzmod activated";
    }
    
    connect(m_main, SIGNAL(welcomeScreen()), this, SLOT(welcomeScreen()));
    connect(m_main, SIGNAL(startingGame()), this, SLOT(startingGame()));
    
    m_main->newGame();
}

void MainWindow::setupActions()
{
    KStandardGameAction::gameNew(m_main, SLOT(newGame()), actionCollection());
    KStandardGameAction::restart(m_main, SLOT(restart()), actionCollection());     
    KStandardGameAction::highscores(m_main, SLOT(highscores()), actionCollection());
    KStandardGameAction::quit(this, SLOT(close()), actionCollection());
    KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
    
    setupGUI();
}

void MainWindow::optionsPreferences()
{
    if (KConfigDialog::showDialog("preferences")) {
        return;
    }
    
    KConfigDialog* dialog = new KConfigDialog(this, "preferences", Settings::self());
    QWidget* mainPage = new QWidget(dialog);
    m_pref_ui.setupUi(mainPage);
    dialog->addPage(mainPage, i18n("Main page"), "main_page");
    connect(dialog, SIGNAL(settingsChanged(const QString&)),
            centralWidget(), SLOT(updatePreferences()));

    dialog->show();
}

void MainWindow::networkErrorHandler()
{
    kDebug() << "GGZDEBUG: error";
}

void MainWindow::networkData(int fd)
{
    kDebug() << "GGZDEBUG: data on fd" << fd;
}

void MainWindow::networkEvent(const KGGZMod::Event& event)
{
    kDebug() << "GGZDEBUG: event" << event.type();

    if(event.type() == KGGZMod::Event::server) {
        KGGZMod::ServerEvent se = (KGGZMod::ServerEvent)event;
        m_fd = se.fd();
    }

    if(event.type() == KGGZMod::Event::self) {
        if(!m_ggzsetup) {
            KGGZMod::Player *player = KGGZMod::Module::instance()->self();
            if(player) {
                m_ggzsetup = true;
                m_main->runGGZ(m_fd);
                m_main->setupController();
            }
        }
    }
}

void MainWindow::startingGame() {
    stateChanged("playing");
}

void MainWindow::welcomeScreen() {
    stateChanged("playing", KXMLGUIClient::StateReverse);
}

#include "mainwindow.moc"

