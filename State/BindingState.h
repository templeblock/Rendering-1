/*
	This file is part of the Rendering library.
	Copyright (C) 2020 Sascha Brandt <sascha@brandt.graphics>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef RENDERING_RENDERINGCONTEXT_BINDINGSTATE_H_
#define RENDERING_RENDERINGCONTEXT_BINDINGSTATE_H_

#include "../Core/Common.h"
#include "../Shader/ShaderUtils.h"

#include <Util/ReferenceCounter.h>

#include <unordered_map>
#include <map>
#include <vector>

namespace Rendering {
class BufferObject;
using BufferObjectRef = Util::Reference<BufferObject>;
class Texture;
using TextureRef = Util::Reference<Texture>;
class ImageView;
using ImageViewRef = Util::Reference<ImageView>;

//------------------

class Binding {
HAS_DIRTY_FLAG
public:
	~Binding();

	bool bindBuffer(const BufferObjectRef& buffer, uint32_t arrayElement=0);
	bool bindTexture(const TextureRef& texture, uint32_t arrayElement=0);
	bool bindInputImage(const ImageViewRef& view, uint32_t arrayElement=0);

	const std::vector<BufferObjectRef>& getBuffers() const { return buffers; }
	const std::vector<TextureRef>& getTextures() const { return textures; }
	const std::vector<ImageViewRef>& getInputImages() const { return views; }
private:
	std::vector<BufferObjectRef> buffers;
	std::vector<TextureRef> textures;
	std::vector<ImageViewRef> views;
};

//------------------

class BindingSet {
HAS_DIRTY_FLAG
public:
	using BindingMap = std::map<uint32_t, Binding>;
	~BindingSet();

	bool bindBuffer(const BufferObjectRef& buffer, uint32_t binding=0, uint32_t arrayElement=0);
	bool bindTexture(const TextureRef& texture, uint32_t binding=0, uint32_t arrayElement=0);
	bool bindInputImage(const ImageViewRef& view, uint32_t binding=0, uint32_t arrayElement=0);

	const BindingMap& getBindings() const { return bindings; }
	const Binding& getBinding(uint32_t binding) const { return bindings.at(binding); }
	bool hasBinding(uint32_t binding) const { return bindings.find(binding) != bindings.end(); }

	void clearDirty(uint32_t binding) { bindings.at(binding).clearDirty(); }
private:
	BindingMap bindings;
};

//------------------

class BindingState {
HAS_DIRTY_FLAG
public:
	BindingState() = default;
	~BindingState();
	BindingState(BindingState&& o);
	BindingState(const BindingState& o);
	BindingState& operator=(BindingState&& o);
	BindingState& operator=(const BindingState& o);

	bool bindBuffer(const BufferObjectRef& buffer, uint32_t set=0, uint32_t binding=0, uint32_t arrayElement=0);
	bool bindTexture(const TextureRef& texture, uint32_t set=0, uint32_t binding=0, uint32_t arrayElement=0);
	bool bindInputImage(const ImageViewRef& view, uint32_t set=0, uint32_t binding=0, uint32_t arrayElement=0);

	BufferObjectRef getBoundBuffer(uint32_t set=0, uint32_t binding=0, uint32_t arrayElement=0);
	TextureRef getBoundTexture(uint32_t set=0, uint32_t binding=0, uint32_t arrayElement=0);
	ImageViewRef getBoundInputImage(uint32_t set=0, uint32_t binding=0, uint32_t arrayElement=0);

	const Binding& getBinding(uint32_t set, uint32_t binding) const { return bindingSets.at(set).getBinding(binding); }
	bool hasBinding(uint32_t set, uint32_t binding) const { return hasBindingSet(set) && bindingSets.at(set).hasBinding(binding); }

	std::unordered_map<uint32_t, BindingSet>& getBindingSets() { return bindingSets; }
	const BindingSet& getBindingSet(uint32_t set) { return bindingSets.at(set); }
	bool hasBindingSet(uint32_t set) const { return bindingSets.find(set) != bindingSets.end(); }

	void reset(){
		bindingSets.clear();
		dirty = true;
	}

	void clearDirty(uint32_t set) { bindingSets.at(set).clearDirty(); }
private:
	std::unordered_map<uint32_t, BindingSet> bindingSets;
};

//------------------

} /* Rendering */

#endif /* end of include guard: RENDERING_RENDERINGCONTEXT_BINDINGSTATE_H_ */