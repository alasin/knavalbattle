/***************************************************************************
                               kbattlefield.cpp
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

#include "kbattlefield.moc"

KBattleField::KBattleField(QWidget *parentw, const char *name, int type) : KGridWidget(parentw, name)
{
    m_parent_widget = static_cast<QWidget *>(parent());
    internalType = type;
    clearField();
    setDrawValues();
    drawField();
}

KBattleField::~KBattleField()
{
}

void KBattleField::clearField()
{
    int i, j;
    for(i = 0; i != 8; i++)
    {
        for(j = 0; j != 8; j++)
    	{
	    FieldData[i][j] = KBattleField::FREE;
	}
    }
}

void KBattleField::drawField()
{
    int i, j;

    for(i = 0; i != 8; i++)
    {
        for(j = 0; j != 8; j++)
        {
            setValues((((i + 1) * 30) + FromLeft), (((j + 1) * 30) + 5), 30);
            switch( FieldData[ i ][ j ] )
	    {
		case KBattleField::FREE:
		    drawSquare();	
                    break;
		
		case KBattleField::WATER:
		    drawSquare();	
		    drawWaterIcon();
		    break;
		
		case KBattleField::HIT:
		    drawSquare();	
		    if(internalType == KBattleField::OWNFIELD)
		        emit doOwnFieldShipListJob(i, j, true, false); 	
                    else if(internalType == KBattleField::ENEMYFIELD)
			drawHitIcon();
		    break;
		    
		case KBattleField::DEATH:
		    drawSquare();	
		    drawDeathIcon();
		    break;	

		case KBattleField::SHIP:
		    drawSquare();	
		    if(internalType == KBattleField::OWNFIELD)
		        emit doOwnFieldShipListJob(i, j, false, false); 	
                    else if(internalType == KBattleField::ENEMYFIELD)
                        emit doEnemyFieldShipListJob(i, j); 	
                    break;
            }
        }		
    }
}

void KBattleField::requestedShipIconDraw(int type, bool hit, bool death)
{
    drawShipIcon(type);
    if(hit)
	drawHitIcon();
    else if(death)
	drawDeathIcon();
}

void KBattleField::setDrawValues()
{
    if(internalType == KBattleField::OWNFIELD)
        FromLeft = 15;
    else if(internalType == KBattleField::ENEMYFIELD)
        FromLeft = (static_cast<QWidget*>(parent())->width() / 2);
}
