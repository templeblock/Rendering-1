/*
	This file is part of the Rendering library.
	Copyright (C) 2019-2020 Sascha Brandt <sascha@brandt.graphics>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "PipelineState.h"

#include <Util/Macros.h>

namespace Rendering {

//-------------

ViewportState& ViewportState::setViewport(const Viewport& value, uint32_t index) { 
	WARN_AND_RETURN_IF(viewports.size() > index, "Invalid viewport index " + std::to_string(index), *this); 
	viewports[index] = value; 
	return *this; 
}

//-------------

ViewportState& ViewportState::setScissor(const Geometry::Rect_i& value, uint32_t index) { 
	WARN_AND_RETURN_IF(scissors.size() > index, "Invalid scissor index " + std::to_string(index), *this); 
	scissors[index] = value; 
	return *this; 
}

//-------------

ColorBlendState& ColorBlendState::setAttachment(const ColorBlendAttachmentState& value, uint32_t index) { 
	WARN_AND_RETURN_IF(attachments.size() > index, "Invalid attachment index " + std::to_string(index), *this); 
	attachments[index] = value; 
	return *this; 
}

//---------------

PipelineState::PipelineState() {
	setVertexInputState({})
		.setInputAssemblyState({})
		.setViewportState({})
		.setRasterizationState({})
		.setMultisampleState({})
		.setDepthStencilState({})
		.setColorBlendState({});
}

//-------------

PipelineState::PipelineState(PipelineState&& o) {
	setVertexInputState(std::move(o.vertexInput))
		.setInputAssemblyState(std::move(o.inputAssembly))
		.setViewportState(std::move(o.viewport))
		.setRasterizationState(std::move(o.rasterization))
		.setMultisampleState(std::move(o.multisample))
		.setDepthStencilState(std::move(o.depthStencil))
		.setColorBlendState(std::move(o.colorBlend));
	o.setVertexInputState({})
		.setInputAssemblyState({})
		.setViewportState({})
		.setRasterizationState({})
		.setMultisampleState({})
		.setDepthStencilState({})
		.setColorBlendState({});
}

//-------------

PipelineState::PipelineState(const PipelineState& o) {
	setVertexInputState(o.vertexInput)
		.setInputAssemblyState(o.inputAssembly)
		.setViewportState(o.viewport)
		.setRasterizationState(o.rasterization)
		.setMultisampleState(o.multisample)
		.setDepthStencilState(o.depthStencil)
		.setColorBlendState(o.colorBlend);
}

//-------------

PipelineState& PipelineState::operator=(PipelineState&& o) {
	setVertexInputState(std::move(o.vertexInput))
		.setInputAssemblyState(std::move(o.inputAssembly))
		.setViewportState(std::move(o.viewport))
		.setRasterizationState(std::move(o.rasterization))
		.setMultisampleState(std::move(o.multisample))
		.setDepthStencilState(std::move(o.depthStencil))
		.setColorBlendState(std::move(o.colorBlend));
	o.setVertexInputState({})
		.setInputAssemblyState({})
		.setViewportState({})
		.setRasterizationState({})
		.setMultisampleState({})
		.setDepthStencilState({})
		.setColorBlendState({});
	return *this;
}

//-------------

PipelineState& PipelineState::operator=(const PipelineState& o) {
	setVertexInputState(o.vertexInput)
		.setInputAssemblyState(o.inputAssembly)
		.setViewportState(o.viewport)
		.setRasterizationState(o.rasterization)
		.setMultisampleState(o.multisample)
		.setDepthStencilState(o.depthStencil)
		.setColorBlendState(o.colorBlend);
	return *this;
}

//-------------


} /* Rendering */