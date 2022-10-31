/*
 * gGUIDialogue.cpp
 *
 *  Created on: 27 Jul 2022
 *      Author: Umut Can
 */

#include "gGUIDialogue.h"
#include "gGUISizer.h"


gGUIDialogue::gGUIDialogue() {
	isdialogueactive = false;

	guisizer = nullptr;

	titlebar = nullptr;
	buttonsbar = nullptr;

	minimizebutton = nullptr;
	maximizebutton = nullptr;
	exitbutton = nullptr;

	buttontrigger = EVENT_NONE; buttonevent = EVENT_NONE;

	isdragenabled = true; isresizeenabled = true;
	ismaximized = false; isdragged = false;

	dragposx = 0; dragposy = 0; sizeposx = 0; sizeposy = 0;

	resizeposition = RESIZE_NONE;

	initDefTitleBar();
	initDefButtonsBar();
}

gGUIDialogue::~gGUIDialogue() {
}

void gGUIDialogue::update() {
	if (guisizer) guisizer->update();
}

void gGUIDialogue::draw() {
	gColor oldcolor = *renderer->getColor();

	if (resizeposition == RESIZE_NONE) {
		// TITLE BAR BACKGROUND
		renderer->setColor(textbackgroundcolor);
		gDrawRectangle(left, top - titlebar->height, width, titlebar->height, true);
		// BUTTONS BAR BACKGROUND
		renderer->setColor(foregroundcolor);
		gDrawRectangle(left, top, width, height + buttonsbar->height, true);

		if (guisizer) guisizer->draw();
		if (titlebar) titlebar->draw();
		if (buttonsbar) buttonsbar->draw();
	}

	// DIALOGUE BORDERS
	renderer->setColor(fontcolor);
	gDrawLine(left, top - titlebar->height, right, top - titlebar->height);
	gDrawLine(left, bottom + buttonsbar->height, right, bottom + buttonsbar->height);
	gDrawLine(left, top - titlebar->height, left, bottom + buttonsbar->height);
	gDrawLine(right, top - titlebar->height, right, bottom + buttonsbar->height);

	renderer->setColor(&oldcolor);
}

void gGUIDialogue::setIsDialogueActive(bool isDialogueActive) {
	this->isdialogueactive = isDialogueActive;
}

bool gGUIDialogue::getIsDialogueActive() {
	return isdialogueactive;
}

void gGUIDialogue::initDefTitleBar() {
	deftitlebar.setSizer(&deftitlebarsizer);
	deftitlebarsizer.setSize(1, 5);

	deftitlebarbitmap.loadImage("gameicon/icon.png", false);
	deftitlebarbitmap.width = deftitlebarbitmapw;
	deftitlebarbitmap.height = deftitlebarbitmapw;

	deftitlebartext.setText("Properties for GlistEngine");

	deftitlebarminimizebutton.setButtonImageFromIcon(gGUIResources::ICON_MINIMIZEBLACK);
	deftitlebarminimizebutton.setPressedButtonImageFromIcon(gGUIResources::ICON_MINIMIZEBLACK);

	deftitlebarmaximizebutton.setButtonImageFromIcon(gGUIResources::ICON_MAXIMIZEBLACK);
	deftitlebarmaximizebutton.setPressedButtonImageFromIcon(gGUIResources::ICON_MAXIMIZEBLACK);

	deftitlebarexitbutton.setButtonImageFromIcon(gGUIResources::ICON_EXITBLACK);
	deftitlebarexitbutton.setPressedButtonImageFromIcon(gGUIResources::ICON_EXITBLACK);

	setMinimizeButton(&deftitlebarminimizebutton);
	setMaximizeButton(&deftitlebarmaximizebutton);
	setExitButton(&deftitlebarexitbutton);
}

void gGUIDialogue::initDefButtonsBar() {
	defbuttonsbar.setSizer(&defbuttonsbarsizer);
	defbuttonsbarsizer.setSize(1, 2);

	defbuttonsbarokbutton.setTitle("OK");
}

void gGUIDialogue::setTitleBar(gGUIContainer* titleBar) {
	this->titlebar = titleBar;
	titlebar->width = width;
	if (titlebar->height == 0) titlebar->height = deftitlebarh;
	titlebar->set(root, this, this, 0, 0, left, top - titlebar->height, titlebar->width, titlebar->height);
}

gGUIContainer* gGUIDialogue::getTitleBar() {
	return titlebar;
}

void gGUIDialogue::setButtonsBar(gGUIContainer* buttonsBar) {
	this->buttonsbar = buttonsBar;
	buttonsbar->width = width;
	if (buttonsbar->height == 0) buttonsbar->height = defbuttonsbarh;
	buttonsbar->set(root, this, this, 0, 0, left, top + height, buttonsbar->width, buttonsbar->height);
}

gGUIContainer* gGUIDialogue::getButtonsBar() {
	return buttonsbar;
}

void gGUIDialogue::resetTitleBar() {
	setTitleBar(&deftitlebar);

	float tbbitp = ((float)deftitlebarbitmapw + 10) / (float)deftitlebar.width;
	float tbbutp = (float)deftitlebarbuttonw / (float)deftitlebar.width;
	float tbtxtp = 1 - (tbbitp + 3 * tbbutp);
	float tbcolproportions[5] = {tbbitp, tbtxtp, tbbutp, tbbutp, tbbutp};
	deftitlebarsizer.setColumnProportions(tbcolproportions);

	deftitlebarsizer.setControl(0, 0, &deftitlebarbitmap);
	deftitlebarbitmap.top += (deftitlebar.height - deftitlebarbitmap.height) / 2;
	deftitlebarbitmap.left += (deftitlebar.width * tbbitp - deftitlebarbitmap.width) / 2;

	deftitlebarsizer.setControl(0, 1, &deftitlebartext);
	deftitlebartext.height = deftitlebarbitmapw / 1.5f;
	deftitlebartext.top += (deftitlebar.height - deftitlebartext.height) / 2;

	deftitlebarsizer.setControl(0, 2, &deftitlebarminimizebutton);
	deftitlebarminimizebutton.setSize(deftitlebar.height, deftitlebar.height);

	if (ismaximized) {
		deftitlebarmaximizebutton.setButtonImageFromIcon(gGUIResources::ICON_RESTOREBLACK);
		deftitlebarmaximizebutton.setPressedButtonImageFromIcon(gGUIResources::ICON_RESTOREBLACK);
	}
	else {
		deftitlebarmaximizebutton.setButtonImageFromIcon(gGUIResources::ICON_MAXIMIZEBLACK);
		deftitlebarmaximizebutton.setPressedButtonImageFromIcon(gGUIResources::ICON_MAXIMIZEBLACK);
	}

	deftitlebarsizer.setControl(0, 3, &deftitlebarmaximizebutton);
	deftitlebarmaximizebutton.setSize(deftitlebar.height, deftitlebar.height);

	deftitlebarsizer.setControl(0, 4, &deftitlebarexitbutton);
	deftitlebarexitbutton.setSize(deftitlebar.height, deftitlebar.height);
}

void gGUIDialogue::resetButtonsBar() {
	setButtonsBar(&defbuttonsbar);

	float bbbutp = ((float)defbuttonsbarbuttonw + 30) / (float)defbuttonsbar.width;
	float bbempp = 1 - bbbutp;
	float bbcolproportions[2] = {bbempp, bbbutp};
	defbuttonsbarsizer.setColumnProportions(bbcolproportions);

	defbuttonsbarsizer.setControl(0, 1, &defbuttonsbarokbutton);
	defbuttonsbarokbutton.setSize(defbuttonsbarbuttonw, defbuttonsbarbuttonh);
	defbuttonsbarokbutton.left += (defbuttonsbar.width * bbbutp - defbuttonsbarbuttonw) / 2;
	defbuttonsbarokbutton.top += (defbuttonsbar.height - defbuttonsbarbuttonh) / 2;
}

void gGUIDialogue::setMinimizeButton(gGUIImageButton* minimizeButton) {
	this->minimizebutton = minimizeButton;
}

void gGUIDialogue::setMaximizeButton(gGUIImageButton* maximizeButton) {
	this->maximizebutton = maximizeButton;
}

void gGUIDialogue::setExitButton(gGUIImageButton* exitButton) {
	this->exitbutton = exitButton;
}

void gGUIDialogue::setButtonEvent(int buttonEvent) {
	this->buttonevent = buttonEvent;
}

int gGUIDialogue::getButtonEvent() {
	return buttonevent;
}

void gGUIDialogue::enableDrag(bool isDragEnabled) {
	this->isdragenabled = isDragEnabled;
}

void gGUIDialogue::enableResize(bool isResizeEnabled) {
	this->isresizeenabled = isResizeEnabled;
}

void gGUIDialogue::setIsMaximized(bool isMaximized) {
	this->ismaximized = isMaximized;
}

void gGUIDialogue::transformDialogue(int left, int top, int width, int height) {
	this->left = left; this->top = top; this->width = width; this->height = height;
	this->right = this->left + this->width; this->bottom = this->top + this->height;

	guisizer->left = this->left; guisizer->top = this->top; guisizer->width = this->width; guisizer->height = this->height;
	guisizer->right = guisizer->left + guisizer->width; guisizer->bottom = guisizer->top + guisizer->height;

	titlebar->left = left; titlebar->top = top - deftitlebarh; titlebar->width = width; titlebar->height = deftitlebarh;
	titlebar->right = titlebar->left + titlebar->width; titlebar->bottom = titlebar->top + titlebar->bottom;

	buttonsbar->left = left; buttonsbar->top = top + height; buttonsbar->width = width; buttonsbar->height = defbuttonsbarh;
	buttonsbar->right = buttonsbar->left + buttonsbar->width; buttonsbar->bottom = buttonsbar->top + buttonsbar->height;
}

int gGUIDialogue::getCursor(int x, int y) {
	if (!ismaximized && isresizeenabled) {
		if ((x > left - 5 && x < left + 5) && (y > titlebar->top && y < buttonsbar->bottom)) return CURSOR_HRESIZE;
		if ((x > right - 5 && x < right + 5) && (y > titlebar->top && y < buttonsbar->bottom)) return CURSOR_HRESIZE;
		if ((y > titlebar->top - 5 && y < titlebar->top + 5) && (x > left && x < right)) return CURSOR_VRESIZE;
		if ((y > buttonsbar->bottom - 5 && y < buttonsbar->bottom + 5) && (x > left && x < right)) return CURSOR_VRESIZE;
	}
	return CURSOR_ARROW;
}

void gGUIDialogue::mouseMoved(int x, int y) {
	if(titlebar) titlebar->mouseMoved(x, y);
	if(guisizer) {
		if(x >= guisizer->left && x < guisizer->right && y >= guisizer->top && y < guisizer->bottom) {
			guisizer->iscursoron = true;
			guisizer->mouseMoved(x, y);
		}
	}
	if(buttonsbar) buttonsbar->mouseMoved(x, y);
}

void gGUIDialogue::mousePressed(int x, int y, int button) {
	if (titlebar) titlebar->mousePressed(x, y, button);
	if (guisizer) guisizer->mousePressed(x, y, button);
	if (buttonsbar) buttonsbar->mousePressed(x, y, button);

	if (minimizebutton->isPressed()) buttontrigger = EVENT_MINIMIZE;
	if (!ismaximized && maximizebutton->isPressed()) buttontrigger = EVENT_MAXIMIZE;
	if (ismaximized && maximizebutton->isPressed()) buttontrigger = EVENT_RESTORE;
	if (exitbutton->isPressed()) buttontrigger = EVENT_EXIT;

	if (!ismaximized && isdragenabled && x > titlebar->left + 5 && x < titlebar->left + titlebar->width - 5 && y > titlebar->top + 5 && y < titlebar->top + titlebar->height) {
		if ((minimizebutton || maximizebutton || exitbutton) && (minimizebutton->isPressed() || maximizebutton->isPressed() || exitbutton->isPressed())) {
			isdragged = false;
		}
		else {
			isdragged = true;
			dragposx = x; dragposy = y;
		}
	}

	if (!ismaximized && isresizeenabled) {
		if ((x > left - 5 && x < left + 5) && (y > titlebar->top && y < buttonsbar->bottom)) {resizeposition = RESIZE_LEFT; sizeposx = x;}
		if ((x > right - 5 && x < right + 5) && (y > titlebar->top && y < buttonsbar->bottom)) {resizeposition = RESIZE_RIGHT; sizeposx = x;}
		if ((y > titlebar->top - 5 && y < titlebar->top + 5) && (x > left && x < right)) {resizeposition = RESIZE_TOP; sizeposy = y;}
		if ((y > buttonsbar->bottom - 5 && y < buttonsbar->bottom + 5) && (x > left && x < right)) {resizeposition = RESIZE_BOTTOM; sizeposy = y;}
	}
}

void gGUIDialogue::mouseDragged(int x, int y, int button) {
	int dx = x - dragposx; int dy = y - dragposy; int sx = x - sizeposx; int sy = y - sizeposy;
	int tleft = left; int twidth = width; int theight = height; int ttop = top;

	if ((resizeposition == RESIZE_RIGHT && sx < 0 && width < 400) || (resizeposition == RESIZE_LEFT && sx > 0 && width < 400)) sx = 0;
	if ((resizeposition == RESIZE_BOTTOM && sy < 0 && height < 100) || (resizeposition == RESIZE_TOP && sy > 0 && height < 100)) sy = 0;

	if (isdragged && x >= titlebar->left - titlebar->width && x < titlebar->left + titlebar->width && y >= titlebar->top - titlebar->height - guisizer->height && y < titlebar->top + titlebar->height + guisizer->height) {
		tleft += dx; ttop += dy;

		for (int i = 0; i < titlebar->getSizer()->getLineNum(); i++) {
			for (int j = 0; j < titlebar->getSizer()->getColumnNum(); j++) {
				gGUIControl* guicontrol = titlebar->getSizer()->getControl(i, j);
				if (guicontrol != nullptr) {
					guicontrol->left += dx;
					guicontrol->top += dy;
					guicontrol->right = guicontrol->left + guicontrol->width;
					guicontrol->bottom = guicontrol->top + guicontrol->height;
				}
			}
		}

		for (int i = 0; i < guisizer->getLineNum(); i++) {
			for (int j = 0; j < guisizer->getColumnNum(); j++) {
				gGUIControl* guicontrol = guisizer->getControl(i, j);
				if (guicontrol != nullptr) {
					guicontrol->left += dx;
					guicontrol->top += dy;
					guicontrol->right = guicontrol->left + guicontrol->width;
					guicontrol->bottom = guicontrol->top + guicontrol->height;
				}
			}
		}

		for (int i = 0; i < buttonsbar->getSizer()->getLineNum(); i++) {
			for (int j = 0; j < buttonsbar->getSizer()->getColumnNum(); j++) {
				gGUIControl* guicontrol = buttonsbar->getSizer()->getControl(i, j);
				if (guicontrol != nullptr) {
					guicontrol->left += dx;
					guicontrol->top += dy;
					guicontrol->right = guicontrol->left + guicontrol->width;
					guicontrol->bottom = guicontrol->top + guicontrol->height;
				}
			}
		}
	}

	if (resizeposition == RESIZE_LEFT) {twidth -= sx; tleft += sx;}
	if (resizeposition == RESIZE_RIGHT) {twidth += sx;}
	if (resizeposition == RESIZE_TOP) {theight -= sy; ttop += sy;}
	if (resizeposition == RESIZE_BOTTOM) {theight += sy;}

	transformDialogue(tleft, ttop, twidth, theight);

	dragposx += dx; dragposy += dy; sizeposx += sx; sizeposy += sy;
}

void gGUIDialogue::mouseReleased(int x, int y, int button) {
	if (titlebar) titlebar->mouseReleased(x, y, button);
	if (guisizer) guisizer->mouseReleased(x, y, button);
	if (buttonsbar) buttonsbar->mouseReleased(x, y, button);
	if (isdragged) isdragged = false;

	if (resizeposition != RESIZE_NONE) {resizeposition = RESIZE_NONE; resetTitleBar(); resetButtonsBar();}

	if (buttontrigger == EVENT_MINIMIZE) {buttonevent = EVENT_MINIMIZE; buttontrigger = EVENT_NONE;}
	if (buttontrigger == EVENT_MAXIMIZE) {buttonevent = EVENT_MAXIMIZE; buttontrigger = EVENT_NONE;}
	if (buttontrigger == EVENT_RESTORE) {buttonevent = EVENT_RESTORE; buttontrigger = EVENT_NONE;}
	if (buttontrigger == EVENT_EXIT) {buttonevent = EVENT_EXIT; buttontrigger = EVENT_NONE;}
}
