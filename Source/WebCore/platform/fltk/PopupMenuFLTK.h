/*
 * Copyright (C) 2014 Lauri Kasanen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef PopupMenuFLTK_h
#define PopupMenuFLTK_h

#include "IntRect.h"
#include "PopupMenu.h"
#include "PopupMenuClient.h"

namespace WebCore {

class FrameView;
class Scrollbar;

class PopupMenuFLTK : public PopupMenu {
public:
    PopupMenuFLTK(PopupMenuClient*, const unsigned, const unsigned);
    ~PopupMenuFLTK();

    virtual void show(const IntRect&, FrameView*, int index) override;
    virtual void hide() override;
    virtual void updateFromElement() override;
    virtual void disconnectClient() override;

private:
    PopupMenuClient* client() const { return m_client; }
    PopupMenuClient* m_client;

    unsigned m_x, m_y;
};

}

#endif // PopupMenuGtk_h
