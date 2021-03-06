/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef AIENTITY_H
#define AIENTITY_H

#include "entity.h"

class AI;

class AIEntity : public Entity
{
Q_OBJECT
    AI* m_ai;
    Sea* m_sea;

    void getShoot();

    friend class DelayedShot;
public:
    AIEntity(Sea::Player player, Sea* sea, SeaView *seaview);
    virtual ~AIEntity();

    virtual void notify(Sea::Player player, const Coord& c, const HitInfo& info);
    virtual void notifyChat(const Entity*, const QString&) { }
    virtual void notifyNick(Sea::Player, const QString&) { }
    virtual void start(bool);
    virtual void startPlacing(bool ask);
    virtual void startPlaying();
    virtual void hit(Shot* shot);
    virtual void notifyGameOptions(bool ask);

    virtual QIcon icon() const;
public slots:
    virtual void notifyAbort() { }
    virtual void notifyRestartPlacing(Sea::Player player) { };
};


#endif // AIENTITY_H

