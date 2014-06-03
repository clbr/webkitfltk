/*
 * Copyright (C) 2007 Apple Inc.
 * Copyright (C) 2007 Alp Toker <alp@atoker.com>
 * Copyright (C) 2008 Collabora Ltd.
 * Copyright (C) 2008 INdT - Instituto Nokia de Tecnologia
 * Copyright (C) 2009-2010 ProFUSION embedded systems
 * Copyright (C) 2009-2011 Samsung Electronics
 * Copyright (c) 2012 Intel Corporation. All rights reserved.
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
 *
 */

#include "config.h"
#include "RenderThemeFLTK.h"

#include "CSSValueKeywords.h"
#include "ExceptionCodePlaceholder.h"
#include "FloatRoundedRect.h"
#include "FontDescription.h"
#include "GraphicsContext.h"
#include "HTMLInputElement.h"
#include "InputTypeNames.h"
#include "NotImplemented.h"
#include "Page.h"
#include "PaintInfo.h"
#include "PlatformContextCairo.h"
#include "RenderBox.h"
#include "RenderObject.h"
#include "RenderProgress.h"
#include "RenderSlider.h"
#include "Settings.h"
#include "UserAgentStyleSheets.h"
#include <new>
#include <wtf/text/CString.h>
#include <wtf/text/StringBuilder.h>
#include <wtf/text/WTFString.h>

namespace WebCore {

// Initialize default font size.
float RenderThemeFLTK::defaultFontSize = 16.0f;

static const float minCancelButtonSize = 5;
static const float maxCancelButtonSize = 21;

static const float minSearchDecorationButtonSize = 1;
static const float maxSearchDecorationButtonSize = 15;
static const float searchFieldDecorationButtonOffset = 3;

// Constants for progress tag animation.
// These values have been copied from RenderThemeGtk.cpp
static const int progressAnimationFrames = 10;
static const double progressAnimationInterval = 0.125;

static const int sliderThumbWidth = 29;
static const int sliderThumbHeight = 11;

void RenderThemeFLTK::adjustSizeConstraints(RenderStyle* style, FormType type) const
{
    // These are always valid, even if no theme could be loaded.
    const ThemePartDesc* desc = m_partDescs + (size_t)type;

    if (style->minWidth().isIntrinsic())
        style->setMinWidth(desc->min.width());
    if (style->minHeight().isIntrinsic())
        style->setMinHeight(desc->min.height());

    if (desc->max.width().value() > 0 && style->maxWidth().isIntrinsicOrAuto())
        style->setMaxWidth(desc->max.width());
    if (desc->max.height().value() > 0 && style->maxHeight().isIntrinsicOrAuto())
        style->setMaxHeight(desc->max.height());

    style->setPaddingTop(desc->padding.top());
    style->setPaddingBottom(desc->padding.bottom());
    style->setPaddingLeft(desc->padding.left());
    style->setPaddingRight(desc->padding.right());
}

static bool isFormElementTooLargeToDisplay(const IntSize& elementSize)
{
    // This limit of 20000 pixels is hardcoded inside edje -- anything above this size
    // will be clipped. This value seems to be reasonable enough so that hardcoding it
    // here won't be a problem.
    static const int maxEdjeDimension = 20000;

    return elementSize.width() > maxEdjeDimension || elementSize.height() > maxEdjeDimension;
}

bool RenderThemeFLTK::isControlStyled(const RenderStyle* style, const BorderData& border, const FillLayer& background, const Color& backgroundColor) const
{
    return RenderTheme::isControlStyled(style, border, background, backgroundColor) || style->appearance() == MenulistButtonPart;
}

bool RenderThemeFLTK::paintThemePart(const RenderObject& object, FormType type, const PaintInfo& info, const IntRect& rect)
{
/*
    bool haveBackgroundColor = isControlStyled(&object.style(), object.style().border(), *object.style().backgroundLayers(), Color::white);
    ControlStates states(extractControlStatesForRenderer(object));
    applyEdjeStateFromForm(entry->edje(), &states, haveBackgroundColor);

    applyEdjeRTLState(entry->edje(), object, type, rect);

    edje_object_calc_force(entry->edje());
    edje_object_message_signal_process(entry->edje());
    evas_render(ecore_evas_get(entry->canvas()));

    cairo_t* cairo = info.context->platformContext()->cr();
    ASSERT(cairo);

    cairo_save(cairo);
    cairo_set_source_surface(cairo, entry->surface(), rect.x(), rect.y());
    cairo_paint_with_alpha(cairo, 1.0);
    cairo_restore(cairo);
*/
    return false;
}

PassRefPtr<RenderTheme> RenderThemeFLTK::create(Page* page)
{
    return adoptRef(new RenderThemeFLTK(page));
}

PassRefPtr<RenderTheme> RenderTheme::themeForPage(Page* page)
{
    if (page)
        return RenderThemeFLTK::create(page);

    static RenderTheme* fallback = RenderThemeFLTK::create(0).leakRef();
    return fallback;
}

RenderThemeFLTK::RenderThemeFLTK(Page* page)
    : RenderTheme()
    , m_page(page)
    , m_activeSelectionBackgroundColor(0, 0, 255)
    , m_activeSelectionForegroundColor(Color::white)
    , m_inactiveSelectionBackgroundColor(0, 0, 128)
    , m_inactiveSelectionForegroundColor(200, 200, 200)
    , m_focusRingColor(32, 32, 224, 224)
    , m_sliderThumbColor(Color::darkGray)
    , m_supportsSelectionForegroundColor(false)
{
}

RenderThemeFLTK::~RenderThemeFLTK()
{
}

static bool supportsFocus(ControlPart appearance)
{
    switch (appearance) {
    case PushButtonPart:
    case ButtonPart:
    case TextFieldPart:
    case TextAreaPart:
    case SearchFieldPart:
    case MenulistPart:
    case RadioPart:
    case CheckboxPart:
    case SliderVerticalPart:
    case SliderHorizontalPart:
        return true;
    default:
        return false;
    }
}

bool RenderThemeFLTK::supportsFocusRing(const RenderStyle* style) const
{
    return supportsFocus(style->appearance());
}

bool RenderThemeFLTK::controlSupportsTints(const RenderObject& object) const
{
    return isEnabled(object);
}

int RenderThemeFLTK::baselinePosition(const RenderObject& object) const
{
    if (!object.isBox())
        return 0;

    if (object.style().appearance() == CheckboxPart
    ||  object.style().appearance() == RadioPart)
        return toRenderBox(&object)->marginTop() + toRenderBox(&object)->height() - 3;

    return RenderTheme::baselinePosition(object);
}

Color RenderThemeFLTK::platformActiveSelectionBackgroundColor() const
{
    return m_activeSelectionBackgroundColor;
}

Color RenderThemeFLTK::platformInactiveSelectionBackgroundColor() const
{
    return m_inactiveSelectionBackgroundColor;
}

Color RenderThemeFLTK::platformActiveSelectionForegroundColor() const
{
    return m_activeSelectionForegroundColor;
}

Color RenderThemeFLTK::platformInactiveSelectionForegroundColor() const
{
    return m_inactiveSelectionForegroundColor;
}

Color RenderThemeFLTK::platformFocusRingColor() const
{
    return m_focusRingColor;
}

bool RenderThemeFLTK::supportsSelectionForegroundColors() const
{
    return m_supportsSelectionForegroundColor;
}

bool RenderThemeFLTK::paintSliderTrack(const RenderObject& object, const PaintInfo& info, const IntRect& rect)
{
    if (object.style().appearance() == SliderHorizontalPart)
        paintThemePart(object, SliderHorizontal, info, rect);
    else
        paintThemePart(object, SliderVertical, info, rect);

#if ENABLE(DATALIST_ELEMENT)
    paintSliderTicks(object, info, rect);
#endif

    return false;
}

void RenderThemeFLTK::adjustSliderTrackStyle(StyleResolver*, RenderStyle* style, Element*) const
{
    style->setBoxShadow(nullptr);
}

void RenderThemeFLTK::adjustSliderThumbStyle(StyleResolver* styleResolver, RenderStyle* style, Element* element) const
{
    RenderTheme::adjustSliderThumbStyle(styleResolver, style, element);
    style->setBoxShadow(nullptr);
}

void RenderThemeFLTK::adjustSliderThumbSize(RenderStyle* style, Element*) const
{
    ControlPart part = style->appearance();
    if (part == SliderThumbVerticalPart) {
        style->setWidth(Length(sliderThumbHeight, Fixed));
        style->setHeight(Length(sliderThumbWidth, Fixed));
    } else if (part == SliderThumbHorizontalPart) {
        style->setWidth(Length(sliderThumbWidth, Fixed));
        style->setHeight(Length(sliderThumbHeight, Fixed));
    }
}

#if ENABLE(DATALIST_ELEMENT)
IntSize RenderThemeFLTK::sliderTickSize() const
{
    return IntSize(1, 6);
}

int RenderThemeFLTK::sliderTickOffsetFromTrackCenter() const
{
    static const int sliderTickOffset = -12;

    return sliderTickOffset;
}

LayoutUnit RenderThemeFLTK::sliderTickSnappingThreshold() const
{
    // The same threshold value as the Chromium port.
    return 5;
}
#endif

bool RenderThemeFLTK::supportsDataListUI(const AtomicString& type) const
{
#if ENABLE(DATALIST_ELEMENT)
    // FIXME: We need to support other types.
    return type == InputTypeNames::email()
        || type == InputTypeNames::range()
        || type == InputTypeNames::search()
        || type == InputTypeNames::url();
#else
    UNUSED_PARAM(type);
    return false;
#endif
}

bool RenderThemeFLTK::paintSliderThumb(const RenderObject& object, const PaintInfo& info, const IntRect& rect)
{
    if (object.style().appearance() == SliderThumbHorizontalPart)
        paintThemePart(object, SliderThumbHorizontal, info, rect);
    else
        paintThemePart(object, SliderThumbVertical, info, rect);

    return false;
}

void RenderThemeFLTK::adjustCheckboxStyle(StyleResolver* styleResolver, RenderStyle* style, Element* element) const
{
    if (!m_page && element && element->document().page()) {
        static_cast<RenderThemeFLTK&>(element->document().page()->theme()).adjustCheckboxStyle(styleResolver, style, element);
        return;
    }

    adjustSizeConstraints(style, CheckBox);

    style->resetBorder();

    const ThemePartDesc* desc = m_partDescs + (size_t)CheckBox;
    if (style->width().value() < desc->min.width().value())
        style->setWidth(desc->min.width());
    if (style->height().value() < desc->min.height().value())
        style->setHeight(desc->min.height());
}

bool RenderThemeFLTK::paintCheckbox(const RenderObject& object, const PaintInfo& info, const IntRect& rect)
{
    return paintThemePart(object, CheckBox, info, rect);
}

void RenderThemeFLTK::adjustRadioStyle(StyleResolver* styleResolver, RenderStyle* style, Element* element) const
{
    if (!m_page && element && element->document().page()) {
        static_cast<RenderThemeFLTK&>(element->document().page()->theme()).adjustRadioStyle(styleResolver, style, element);
        return;
    }

    adjustSizeConstraints(style, RadioButton);

    style->resetBorder();

    const ThemePartDesc* desc = m_partDescs + (size_t)RadioButton;
    if (style->width().value() < desc->min.width().value())
        style->setWidth(desc->min.width());
    if (style->height().value() < desc->min.height().value())
        style->setHeight(desc->min.height());
}

bool RenderThemeFLTK::paintRadio(const RenderObject& object, const PaintInfo& info, const IntRect& rect)
{
    return paintThemePart(object, RadioButton, info, rect);
}

void RenderThemeFLTK::adjustButtonStyle(StyleResolver* styleResolver, RenderStyle* style, Element* element) const
{
    if (!m_page && element && element->document().page()) {
        static_cast<RenderThemeFLTK&>(element->document().page()->theme()).adjustButtonStyle(styleResolver, style, element);
        return;
    }

    // adjustSizeConstrains can make SquareButtonPart's size wrong (by adjusting paddings), so call it only for PushButtonPart and ButtonPart
    if (style->appearance() == PushButtonPart || style->appearance() == ButtonPart)
        adjustSizeConstraints(style, Button);
}

bool RenderThemeFLTK::paintButton(const RenderObject& object, const PaintInfo& info, const IntRect& rect)
{
    return paintThemePart(object, Button, info, rect);
}

void RenderThemeFLTK::adjustMenuListStyle(StyleResolver* styleResolver, RenderStyle* style, Element* element) const
{
    if (!m_page && element && element->document().page()) {
        static_cast<RenderThemeFLTK&>(element->document().page()->theme()).adjustMenuListStyle(styleResolver, style, element);
        return;
    }
    adjustSizeConstraints(style, ComboBox);
    style->resetBorder();
    style->setWhiteSpace(PRE);

    style->setLineHeight(RenderStyle::initialLineHeight());
}

bool RenderThemeFLTK::paintMenuList(const RenderObject& object, const PaintInfo& info, const IntRect& rect)
{
    return paintThemePart(object, ComboBox, info, rect);
}

void RenderThemeFLTK::adjustMenuListButtonStyle(StyleResolver* styleResolver, RenderStyle* style, Element* element) const
{
    // Height is locked to auto if height is not specified.
    style->setHeight(Length(Auto));

    // The <select> box must be at least 12px high for the button to render the text inside the box without clipping.
    const int dropDownBoxMinHeight = 12;

    // Calculate min-height of the <select> element.
    int minHeight = style->fontMetrics().height();
    minHeight = std::max(minHeight, dropDownBoxMinHeight);
    style->setMinHeight(Length(minHeight, Fixed));

    adjustMenuListStyle(styleResolver, style, element);
}

bool RenderThemeFLTK::paintMenuListButtonDecorations(const RenderObject& object, const PaintInfo& info, const FloatRect& rect)
{
    return paintMenuList(object, info, IntRect(rect));
}

void RenderThemeFLTK::adjustTextFieldStyle(StyleResolver* styleResolver, RenderStyle* style, Element* element) const
{
    if (!m_page && element && element->document().page()) {
        static_cast<RenderThemeFLTK&>(element->document().page()->theme()).adjustTextFieldStyle(styleResolver, style, element);
        return;
    }
    adjustSizeConstraints(style, TextField);
    style->resetBorder();
}

bool RenderThemeFLTK::paintTextField(const RenderObject& object, const PaintInfo& info, const FloatRect& rect)
{
    return paintThemePart(object, TextField, info, IntRect(rect));
}

void RenderThemeFLTK::adjustTextAreaStyle(StyleResolver*, RenderStyle*, Element*) const
{
}

bool RenderThemeFLTK::paintTextArea(const RenderObject& object, const PaintInfo& info, const FloatRect& rect)
{
    return paintTextField(object, info, rect);
}

void RenderThemeFLTK::adjustSearchFieldResultsButtonStyle(StyleResolver* styleResolver, RenderStyle* style, Element* element) const
{
    if (!m_page && element && element->document().page()) {
        static_cast<RenderThemeFLTK&>(element->document().page()->theme()).adjustSearchFieldResultsButtonStyle(styleResolver, style, element);
        return;
    }
    adjustSizeConstraints(style, SearchFieldResultsButton);
    style->resetBorder();
    style->setWhiteSpace(PRE);

    float fontScale = style->fontSize() / defaultFontSize;
    int decorationSize = lroundf(std::min(std::max(minSearchDecorationButtonSize, defaultFontSize * fontScale), maxSearchDecorationButtonSize));

    style->setWidth(Length(decorationSize + searchFieldDecorationButtonOffset, Fixed));
    style->setHeight(Length(decorationSize, Fixed));
}

bool RenderThemeFLTK::paintSearchFieldResultsButton(const RenderObject& object, const PaintInfo& info, const IntRect& rect)
{
    return paintThemePart(object, SearchFieldResultsButton, info, rect);
}

void RenderThemeFLTK::adjustSearchFieldResultsDecorationPartStyle(StyleResolver* styleResolver, RenderStyle* style, Element* element) const
{
    if (!m_page && element && element->document().page()) {
        static_cast<RenderThemeFLTK&>(element->document().page()->theme()).adjustSearchFieldResultsDecorationPartStyle(styleResolver, style, element);
        return;
    }
    adjustSizeConstraints(style, SearchFieldResultsDecoration);
    style->resetBorder();
    style->setWhiteSpace(PRE);

    float fontScale = style->fontSize() / defaultFontSize;
    int decorationSize = lroundf(std::min(std::max(minSearchDecorationButtonSize, defaultFontSize * fontScale), maxSearchDecorationButtonSize));

    style->setWidth(Length(decorationSize + searchFieldDecorationButtonOffset, Fixed));
    style->setHeight(Length(decorationSize, Fixed));
}

bool RenderThemeFLTK::paintSearchFieldResultsDecorationPart(const RenderObject& object, const PaintInfo& info, const IntRect& rect)
{
    return paintThemePart(object, SearchFieldResultsDecoration, info, rect);
}

void RenderThemeFLTK::adjustSearchFieldCancelButtonStyle(StyleResolver* styleResolver, RenderStyle* style, Element* element) const
{
    if (!m_page && element && element->document().page()) {
        static_cast<RenderThemeFLTK&>(element->document().page()->theme()).adjustSearchFieldCancelButtonStyle(styleResolver, style, element);
        return;
    }
    adjustSizeConstraints(style, SearchFieldCancelButton);
    style->resetBorder();
    style->setWhiteSpace(PRE);

    // Logic taken from RenderThemeChromium.cpp.
    // Scale the button size based on the font size.
    float fontScale = style->fontSize() / defaultFontSize;
    int cancelButtonSize = lroundf(std::min(std::max(minCancelButtonSize, defaultFontSize * fontScale), maxCancelButtonSize));

    style->setWidth(Length(cancelButtonSize, Fixed));
    style->setHeight(Length(cancelButtonSize, Fixed));
}

bool RenderThemeFLTK::paintSearchFieldCancelButton(const RenderObject& object, const PaintInfo& info, const IntRect& rect)
{
    return paintThemePart(object, SearchFieldCancelButton, info, rect);
}

void RenderThemeFLTK::adjustSearchFieldStyle(StyleResolver* styleResolver, RenderStyle* style, Element* element) const
{
    if (!m_page && element && element->document().page()) {
        static_cast<RenderThemeFLTK&>(element->document().page()->theme()).adjustSearchFieldStyle(styleResolver, style, element);
        return;
    }
    adjustSizeConstraints(style, SearchField);
    style->resetBorder();
    style->setWhiteSpace(PRE);
}

bool RenderThemeFLTK::paintSearchField(const RenderObject& object, const PaintInfo& info, const IntRect& rect)
{
    return paintThemePart(object, SearchField, info, rect);
}

void RenderThemeFLTK::adjustInnerSpinButtonStyle(StyleResolver* styleResolver, RenderStyle* style, Element* element) const
{
    if (!m_page && element && element->document().page()) {
        static_cast<RenderThemeFLTK&>(element->document().page()->theme()).adjustInnerSpinButtonStyle(styleResolver, style, element);
        return;
    }
    adjustSizeConstraints(style, Spinner);
}

bool RenderThemeFLTK::paintInnerSpinButton(const RenderObject& object, const PaintInfo& info, const IntRect& rect)
{
    return paintThemePart(object, Spinner, info, rect);
}

void RenderThemeFLTK::setDefaultFontSize(int size)
{
    defaultFontSize = size;
}

void RenderThemeFLTK::systemFont(CSSValueID, FontDescription& fontDescription) const
{
    // It was called by RenderEmbeddedObject::paintReplaced to render alternative string.
    // To avoid cairo_error while rendering, fontDescription should be passed.
    fontDescription.setOneFamily("Sans");
    fontDescription.setSpecifiedSize(defaultFontSize);
    fontDescription.setIsAbsoluteSize(true);
    fontDescription.setGenericFamily(FontDescription::NoFamily);
    fontDescription.setWeight(FontWeightNormal);
    fontDescription.setItalic(false);
}

#if ENABLE(PROGRESS_ELEMENT)
void RenderThemeFLTK::adjustProgressBarStyle(StyleResolver*, RenderStyle* style, Element*) const
{
    style->setBoxShadow(nullptr);
}

double RenderThemeFLTK::animationRepeatIntervalForProgressBar(RenderProgress*) const
{
    return progressAnimationInterval;
}

double RenderThemeFLTK::animationDurationForProgressBar(RenderProgress*) const
{
    return progressAnimationInterval * progressAnimationFrames * 2; // "2" for back and forth;
}

bool RenderThemeFLTK::paintProgressBar(const RenderObject& object, const PaintInfo& info, const IntRect& rect)
{
    if (!object.isProgress())
        return true;

    return paintThemePart(object, ProgressBar, info, rect);
}
#endif

#if ENABLE(VIDEO)
String RenderThemeFLTK::mediaControlsStyleSheet()
{
    return ASCIILiteral(mediaControlsAppleUserAgentStyleSheet);
}

String RenderThemeFLTK::mediaControlsScript()
{
    StringBuilder scriptBuilder;
    scriptBuilder.append(mediaControlsLocalizedStringsJavaScript, sizeof(mediaControlsLocalizedStringsJavaScript));
    scriptBuilder.append(mediaControlsAppleJavaScript, sizeof(mediaControlsAppleJavaScript));
    return scriptBuilder.toString();
}
#endif

}
