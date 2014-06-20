/*
WebkitFLTK
Copyright (C) 2014 Lauri Kasanen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef webview_h
#define webview_h

#include <FL/Fl_Widget.H>

class privatewebview;

class webview: public Fl_Widget {
public:
	webview(int x, int y, int w, int h);
	~webview();

	void draw() override;
	void drawWeb();

	int handle(int) override;

	void resize();

	void load(const char *);
	void loadString(const char * const str, const char * const mime = NULL,
			const char * const enc = NULL, const char * const baseurl = NULL);

	privatewebview *priv;
};

#endif
