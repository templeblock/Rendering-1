/*
	This file is part of the Rendering library.
	Copyright (C) 2018 Sascha Brandt <sascha@brandt.graphics>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "BindingState.h"
#include "../GLHeader.h"

namespace Rendering {


static inline void bindOrRemoveBuffer(std::unordered_map<uint64_t, BindingState::BufferBinding>& bindings, uint32_t target, uint32_t location) {
	union {
		struct { uint32_t target; uint32_t location; };
		uint64_t key;
	} bufferKey;
	bufferKey.target = target;
	bufferKey.location = location;
	auto it = bindings.find(bufferKey.key);
	if(it != bindings.end()) {
		if(it->second.buffer.isNotNull()) {
			it->second.offset = it->second.buffer->getOffset();
			it->second.size = it->second.buffer->getSize();
			it->second.buffer->bind(target, location);
		} else {
			bindings.erase(it);
			glBindBufferBase(target, location, 0);
		}
	}
}

BindingState::StateDiff_t BindingState::makeDiff(const BindingState& target, bool forced) const {
	StateDiff_t diff;
	
	for(const auto& e : buffers) {
		auto key = e.second._key;
		bool changed = e.second.buffer.isNotNull() && (e.second.offset != e.second.buffer->getOffset() || e.second.size != e.second.buffer->getSize());		
		switch(e.second.target) {
			case GL_SHADER_STORAGE_BUFFER:
				diff.ssbos.set(e.second.location, forced || changed || target.getBufferBinding(key) != e.second);
				break;
			case GL_UNIFORM_BUFFER: 
				diff.ubos.set(e.second.location, forced || changed || target.getBufferBinding(key) != e.second);
				break;
			case GL_ATOMIC_COUNTER_BUFFER: 
				diff.acbos.set(e.second.location, forced || changed || target.getBufferBinding(key) != e.second);
				break;
			case GL_TRANSFORM_FEEDBACK_BUFFER: 
				diff.tfbos.set(e.second.location, forced || changed || target.getBufferBinding(key) != e.second);
				break;
		}
	}
	
	for(const auto& e : target.buffers) {
		auto key = e.second._key;
		bool changed = e.second.buffer.isNotNull() && (e.second.offset != e.second.buffer->getOffset() || e.second.size != e.second.buffer->getSize());
		switch(e.second.target) {
			case GL_SHADER_STORAGE_BUFFER:
				diff.ssbos.set(e.second.location, forced || changed || getBufferBinding(key) != e.second);
				break;
			case GL_UNIFORM_BUFFER: 
				diff.ubos.set(e.second.location, forced || changed || getBufferBinding(key) != e.second);
				break;
			case GL_ATOMIC_COUNTER_BUFFER: 
				diff.acbos.set(e.second.location, forced || changed || getBufferBinding(key) != e.second);
				break;
			case GL_TRANSFORM_FEEDBACK_BUFFER: 
				diff.tfbos.set(e.second.location, forced || changed || getBufferBinding(key) != e.second);
				break;
		}
	}
	
	for(const auto& e : textures) {
		diff.textures.set(e.first, forced || target.getTexture(e.first) != e.second);
	}
	for(const auto& e : target.textures) {
		diff.textures.set(e.first, forced || getTexture(e.first) != e.second);
	}
	
	return diff;
}

void BindingState::apply(const StateDiff_t& diff) {
	GET_GL_ERROR();
	
	// SSBOs
	if(diff.ssbos.any()) {
		for(uint_fast8_t i=0; i<diff.ssbos.size(); ++i) {
			if(diff.ssbos.test(i))
				bindOrRemoveBuffer(buffers, GL_SHADER_STORAGE_BUFFER, i);
		}
		GET_GL_ERROR();
	}
	
	// UBOs
	if(diff.ubos.any()) {
		for(uint_fast8_t i=0; i<diff.ubos.size(); ++i) {
			if(diff.ubos.test(i))
				bindOrRemoveBuffer(buffers, GL_UNIFORM_BUFFER, i);
		}
		GET_GL_ERROR();
	}
	
	// ACBOs
	if(diff.acbos.any()) {
		for(uint_fast8_t i=0; i<diff.acbos.size(); ++i) {
			if(diff.acbos.test(i))
				bindOrRemoveBuffer(buffers, GL_ATOMIC_COUNTER_BUFFER, i);
		}
		GET_GL_ERROR();
	}
	
	// TFBOs
	if(diff.tfbos.any()) {
		for(uint_fast8_t i=0; i<diff.tfbos.size(); ++i) {
			if(diff.tfbos.test(i))
				bindOrRemoveBuffer(buffers, GL_TRANSFORM_FEEDBACK_BUFFER, i);
		}
		GET_GL_ERROR();
	}
	
	// Textures
	if(diff.textures.any()) {
		for(uint_fast8_t i=0; i<getMaxTextureBindings(); ++i) {
			if(diff.textures.test(i)) {
				auto it = textures.find(i);
				if(it != textures.end()) {
					if(it->second.isNotNull()) {
						glBindTextureUnit(i, it->second->getGLId());
					} else {
						textures.erase(it);
						glBindTextureUnit(i, 0);
					}
				}
			}
		}
		GET_GL_ERROR();
	}
	
}

}
