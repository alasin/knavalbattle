/***************************************************************************
                               kbattlefield.h
                             -------------------
    Developers: (c) 2000 Nikolas Zimmermann <wildfox@kde.org>
                (c) 2000 Daniel Molkentin <molkentin@kde.org>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KBATTLEFIELD_H
#define KBATTLEFIELD_H

#include <qpainter.h>
#include <qwidget.h>
#include "kgridwidget.h"

class KBattleField : public KGridWidget
{
    Q_OBJECT
    public:
        enum{ OWNFIELD, ENEMYFIELD, FREE, WATER, HIT, DEATH, SHIP };
        KBattleField( QWidget *parent, const char *name, int type );
        ~KBattleField();
    
        void requestedShipIconDraw( int type, bool hit = false, bool death = false );
        void setDrawValues();
        void drawField();
        void clearField();
        void changeData( int &fieldx, int &fieldy, int type );
        void findOwnFieldShipType( int x, int y, bool hit = false, bool death = false );
        void findEnemyFieldShipType( int x, int y );
        int getState( int fieldx, int fieldy );

    private:
        int FieldData[ 8 ][ 8 ];
        int internalType;
        int FromLeft;
        QString nickName;

    signals:
        void doOwnFieldShipListJob( int, int, bool, bool );
        void doEnemyFieldShipListJob( int, int );

};

#endif
