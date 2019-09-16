/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 * Copyright (c) 2019 The RmlUi Team, and contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "precompiled.h"

namespace Rml {
namespace Core {

FontEngineInterface::FontEngineInterface()
{
}

FontEngineInterface::~FontEngineInterface()
{
}

bool FontEngineInterface::LoadFontFace(const String& file_name)
{
	return false;
}

FontFaceHandle FontEngineInterface::GetFontFaceHandle(const String& RMLUI_UNUSED_PARAMETER(family),
	const String& RMLUI_UNUSED_PARAMETER(charset), Style::FontStyle RMLUI_UNUSED_PARAMETER(style),
	Style::FontWeight RMLUI_UNUSED_PARAMETER(weight), int RMLUI_UNUSED_PARAMETER(size))
{
	RMLUI_UNUSED(family);
	RMLUI_UNUSED(charset);
	RMLUI_UNUSED(style);
	RMLUI_UNUSED(weight);
	RMLUI_UNUSED(size);
	return 0;
}
	
int FontEngineInterface::GenerateLayerConfiguration(FontFaceHandle, const FontEffectList& font_effects) const
{
	return 0;
}

int FontEngineInterface::GetCharacterWidth(FontFaceHandle) const
{
	return 0;
}

int FontEngineInterface::GetSize(FontFaceHandle) const
{
	return 0;
}

int FontEngineInterface::GetXHeight(FontFaceHandle) const
{
	return 0;
}

int FontEngineInterface::GetLineHeight(FontFaceHandle) const
{
	return 0;
}

int FontEngineInterface::GetBaseline(FontFaceHandle) const
{
	return 0;
}

float FontEngineInterface::GetUnderline(FontFaceHandle, float *) const
{
	return 0;
}

int FontEngineInterface::GetStringWidth(FontFaceHandle, const WString& RMLUI_UNUSED_PARAMETER(string), word RMLUI_UNUSED_PARAMETER(prior_character))
{
	RMLUI_UNUSED(string);
	RMLUI_UNUSED(prior_character);
	return 0;
}

int FontEngineInterface::GenerateString(FontFaceHandle, GeometryList& RMLUI_UNUSED_PARAMETER(geometry), const WString& RMLUI_UNUSED_PARAMETER(string), 
	const Vector2f& RMLUI_UNUSED_PARAMETER(position), const Colourb& RMLUI_UNUSED_PARAMETER(colour), int RMLUI_UNUSED_PARAMETER(layer_configuration)) const
{
	RMLUI_UNUSED(geometry);
	RMLUI_UNUSED(string);
	RMLUI_UNUSED(position);
	RMLUI_UNUSED(colour);
	RMLUI_UNUSED(layer_configuration);
	return 0;
}

}
}
