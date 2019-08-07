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
#include "ElementImage.h"
#include "../../Include/RmlUi/Core.h"
#include "TextureDatabase.h"

namespace Rml {
namespace Core {

// Constructs a new ElementImage.
ElementImage::ElementImage(const String& tag) : Element(tag), dimensions(-1, -1), geometry(this)
{
	ResetCoords();
	geometry_dirty = false;
	texture_dirty = true;
}

ElementImage::~ElementImage()
{
}

// Sizes the box to the element's inherent size.
bool ElementImage::GetIntrinsicDimensions(Vector2f& _dimensions)
{
	// Check if we need to reload the texture.
	if (texture_dirty)
		LoadTexture();

	// Calculate the x dimension.
	if (HasAttribute("width"))
		dimensions.x = GetAttribute< float >("width", -1);
	else if (using_coords)
		dimensions.x = (float) (coords[2] - coords[0]);
	else
		dimensions.x = (float) texture.GetDimensions(GetRenderInterface()).x;

	// Calculate the y dimension.
	if (HasAttribute("height"))
		dimensions.y = GetAttribute< float >("height", -1);
	else if (using_coords)
		dimensions.y = (float) (coords[3] - coords[1]);
	else
		dimensions.y = (float) texture.GetDimensions(GetRenderInterface()).y;

	// Return the calculated dimensions. If this changes the size of the element, it will result in
	// a 'resize' event which is caught below and will regenerate the geometry.
	_dimensions = dimensions;
	return true;
}

// Renders the element.
void ElementImage::OnRender()
{
	// Regenerate the geometry if required (this will be set if 'coords' changes but does not
	// result in a resize).
	if (geometry_dirty)
		GenerateGeometry();

	// Render the geometry beginning at this element's content region.
	geometry.Render(GetAbsoluteOffset(Rml::Core::Box::CONTENT).Round());
}

// Called when attributes on the element are changed.
void ElementImage::OnAttributeChange(const Rml::Core::ElementAttributes& changed_attributes)
{
	// Call through to the base element's OnAttributeChange().
	Rml::Core::Element::OnAttributeChange(changed_attributes);

	float dirty_layout = false;

	// Check for a changed 'src' attribute. If this changes, the old texture handle is released,
	// forcing a reload when the layout is regenerated.
	if (changed_attributes.find("src") != changed_attributes.end())
	{
		texture_dirty = true;
		dirty_layout = true;
	}

	// Check for a changed 'width' attribute. If this changes, a layout is forced which will
	// recalculate the dimensions.
	if (changed_attributes.find("width") != changed_attributes.end() ||
		changed_attributes.find("height") != changed_attributes.end())
	{
		dirty_layout = true;
	}

	// Check for a change to the 'coords' attribute. If this changes, the coordinates are
	// recomputed and a layout forced.
	if (changed_attributes.find("coords") != changed_attributes.end())
	{
		if (HasAttribute("coords"))
		{
			StringList coords_list;
			StringUtilities::ExpandString(coords_list, GetAttribute< String >("coords", ""));

			if (coords_list.size() != 4)
			{
				Rml::Core::Log::Message(Log::LT_WARNING, "Element '%s' has an invalid 'coords' attribute; coords requires 4 values, found %d.", GetAddress().c_str(), coords_list.size());
				ResetCoords();
			}
			else
			{
				for (size_t i = 0; i < 4; ++i)
					coords[i] = atoi(coords_list[i].c_str());

				// Check for the validity of the coordinates.
				if (coords[0] < 0 || coords[2] < coords[0] ||
					coords[1] < 0 || coords[3] < coords[1])
				{
					Rml::Core::Log::Message(Log::LT_WARNING, "Element '%s' has an invalid 'coords' attribute; invalid coordinate values specified.", GetAddress().c_str());
					ResetCoords();
				}
				else
				{
					// We have new, valid coordinates; force the geometry to be regenerated.
					geometry_dirty = true;
					using_coords = true;
				}
			}
		}
		else
			ResetCoords();

		// Coordinates have changes; this will most likely result in a size change, so we need to force a layout.
		dirty_layout = true;
	}

	if (dirty_layout)
		DirtyLayout();
}

void ElementImage::OnPropertyChange(const PropertyIdSet& changed_properties)
{
    Element::OnPropertyChange(changed_properties);

    if (changed_properties.Contains(PropertyId::ImageColor) ||
        changed_properties.Contains(PropertyId::Opacity)) {
        GenerateGeometry();
    }
}

// Regenerates the element's geometry.
void ElementImage::OnResize()
{
	GenerateGeometry();
}

void ElementImage::GenerateGeometry()
{
	// Release the old geometry before specifying the new vertices.
	geometry.Release(true);

	std::vector< Rml::Core::Vertex >& vertices = geometry.GetVertices();
	std::vector< int >& indices = geometry.GetIndices();

	vertices.resize(4);
	indices.resize(6);

	// Generate the texture coordinates.
	Vector2f texcoords[2];
	if (using_coords)
	{
		Vector2f texture_dimensions((float) texture.GetDimensions(GetRenderInterface()).x, (float) texture.GetDimensions(GetRenderInterface()).y);
		if (texture_dimensions.x == 0)
			texture_dimensions.x = 1;
		if (texture_dimensions.y == 0)
			texture_dimensions.y = 1;

		texcoords[0].x = (float) coords[0] / texture_dimensions.x;
		texcoords[0].y = (float) coords[1] / texture_dimensions.y;

		texcoords[1].x = (float) coords[2] / texture_dimensions.x;
		texcoords[1].y = (float) coords[3] / texture_dimensions.y;
	}
	else
	{
		texcoords[0] = Vector2f(0, 0);
		texcoords[1] = Vector2f(1, 1);
	}

	const ComputedValues& computed = GetComputedValues();

	float opacity = computed.opacity;
	Colourb quad_colour = computed.image_color;
    quad_colour.alpha = (byte)(opacity * (float)quad_colour.alpha);
	
	Vector2f quad_size = GetBox().GetSize(Rml::Core::Box::CONTENT).Round();

	Rml::Core::GeometryUtilities::GenerateQuad(&vertices[0], &indices[0], Vector2f(0, 0), quad_size, quad_colour,  texcoords[0], texcoords[1]);

	geometry_dirty = false;
}

bool ElementImage::LoadTexture()
{
	texture_dirty = false;

	// Get the source URL for the image.
	String image_source = GetAttribute< String >("src", "");
	if (image_source.empty())
		return false;

	geometry_dirty = true;

	Rml::Core::ElementDocument* document = GetOwnerDocument();
	URL source_url(document == nullptr ? "" : document->GetSourceURL());

	if (!texture.Load(image_source, source_url.GetPath()))
	{
		geometry.SetTexture(nullptr);
		return false;
	}

	// Set the texture onto our geometry object.
	geometry.SetTexture(&texture);
	return true;
}

void ElementImage::ResetCoords()
{
	using_coords = false;

	for (int i = 0; i < 4; ++i)
		coords[i] = -1;
}

}
}
