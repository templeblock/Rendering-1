/*
	This file is part of the Rendering library.
	Copyright (C) 2013 Benjamin Eikel <benjamin@eikel.org>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "TestUtils.h"
#include <catch2/catch.hpp>

#include <Geometry/Box.h>
#include <Geometry/Vec3.h>
#include <Geometry/Angle.h>
#include <Geometry/Matrix4x4.h>

#include "../RenderingContext.h"
#include "../Core/Device.h"
#include "../Shader/Shader.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/VertexDescription.h"
#include "../MeshUtils/PrimitiveShapes.h"
#include <Util/Timer.h>
#include <Util/Utils.h>
#include <cstdint>
#include <iostream>

#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>
#include <vulkan/vulkan.hpp>

const std::string vertexShader = R"vs(
	#version 450

	layout(location = 0) in vec3 sg_Position;
	layout(location = 1) in vec4 sg_Color;

	layout(location = 0) out vec3 fragColor;

	layout(push_constant) uniform PushConstants {
		mat4 sg_matrix_modelToCamera;
	};

	void main() {
		gl_Position = vec4(sg_Position, 1.0);
		fragColor = vec3(1);
	}
)vs";

const std::string fragmentShader = R"fs(
	#version 450

	layout(location = 0) in vec3 fragColor;
	layout(location = 0) out vec4 outColor;

	void main() {
		outColor = vec4(fragColor, 1.0);
	}
)fs";

TEST_CASE("RenderingContext", "[RenderingContextTest]") {
	using namespace Rendering;
	std::cout << std::endl;
	
	auto device = TestUtils::device;
	REQUIRE(device);
	vk::Device vkDevice(device->getApiHandle());
	REQUIRE(vkDevice);
	RenderingContext context(device);

	
	// --------------------------------------------
	// input

	VertexDescription vd;
	vd.appendPosition3D();
	vd.appendColorRGBAByte();
	Mesh::Ref mesh = MeshUtils::createBox(vd, {});
	REQUIRE(mesh);
	
	// compile shaders
	auto shader = Shader::createShader(device, vertexShader, fragmentShader);
	REQUIRE(shader->init());

	context.setShader(shader);
	REQUIRE(context.isShaderEnabled(shader));

	// --------------------------------------------
	// draw

	auto angle = Geometry::Angle::deg(0);
	Geometry::Matrix4x4f mat;
	for(uint_fast32_t round = 0; round < 1000; ++round) {
		context.displayMesh(mesh);
		context.present();
		if(round == 500)
			context.setShader(nullptr);
		mat.rotate_deg(1, {0,1,0});
	}
	vkDevice.waitIdle();
}