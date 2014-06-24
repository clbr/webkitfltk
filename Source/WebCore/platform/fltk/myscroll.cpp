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

#include "myscroll.h"

myscroll::myscroll(int x, int y, int w, int h): Fl_Scrollbar(x, y, w, h) {

	button1 = button2 = false;
}

void myscroll::draw() {
	draw_box();

	int X = x()+Fl::box_dx(box());
	int Y = y()+Fl::box_dy(box());
	int W = w()-Fl::box_dw(box());
	int H = h()-Fl::box_dh(box());

	if (horizontal()) {
		if (W < 3*H) {Fl_Slider::draw(X,Y,W,H); return;}
		Fl_Slider::draw(X+H,Y,W-2*H,H);

		// Buttons
		draw_box(button1 ? fl_down(slider()) : slider(),
			 X, Y, H, H, selection_color());
		draw_box(button2 ? fl_down(slider()) : slider(),
			 X+W-H, Y, H, H, selection_color());
		if (active_r())
			fl_color(labelcolor());
		else
			fl_color(fl_inactive(labelcolor()));
		int w1 = (H-4)/3; if (w1 < 1) w1 = 1;
		int x1 = X+(H-w1-1)/2;
		int yy1 = Y+(H-2*w1-1)/2;
		if (Fl::scheme_ && !strcmp(Fl::scheme_, "gtk+")) {
			fl_polygon(x1, yy1+w1, x1+w1, yy1+2*w1, x1+w1-1, yy1+w1,
					x1+w1, yy1);
			x1 += (W-H);
			fl_polygon(x1, yy1, x1+1, yy1+w1, x1, yy1+2*w1, x1+w1,
					yy1+w1);
		} else {
			fl_polygon(x1, yy1+w1, x1+w1, yy1+2*w1, x1+w1, yy1);
			x1 += (W-H);
			fl_polygon(x1, yy1, x1, yy1+2*w1, x1+w1, yy1+w1);
		}
	} else { // vertical
		if (H < 3*W) {Fl_Slider::draw(X,Y,W,H); return;}
		Fl_Slider::draw(X,Y+W,W,H-2*W);

		// Buttons
		draw_box(button1 ? fl_down(slider()) : slider(),
			 X, Y, W, W, selection_color());
		draw_box(button2 ? fl_down(slider()) : slider(),
			 X, Y+H-W, W, W, selection_color());
		if (active_r())
			fl_color(labelcolor());
		else
			fl_color(fl_inactive(labelcolor()));
		int w1 = (W-4)/3; if (w1 < 1) w1 = 1;
		int x1 = X+(W-2*w1-1)/2;
		int yy1 = Y+(W-w1-1)/2;
		if (Fl::scheme_ && !strcmp(Fl::scheme_, "gtk+")) {
			fl_polygon(x1, yy1+w1, x1+w1, yy1+w1-1, x1+2*w1, yy1+w1, x1+w1,
					yy1);
			yy1 += H-W;
			fl_polygon(x1, yy1, x1+w1, yy1+1, x1+w1, yy1+w1);
			fl_polygon(x1+w1, yy1+1, x1+2*w1, yy1, x1+w1, yy1+w1);
		} else {
			fl_polygon(x1, yy1+w1, x1+2*w1, yy1+w1, x1+w1, yy1);
			yy1 += H-W;
			fl_polygon(x1, yy1, x1+w1, yy1+w1, x1+2*w1, yy1);
		}
	}
}
