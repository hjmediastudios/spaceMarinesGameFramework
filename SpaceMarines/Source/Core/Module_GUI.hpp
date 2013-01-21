#pragma once

#include "Prerequisites.hpp"
#include "Module_GUI_Math.hpp"
#include "Module_Debug.hpp"
#include "../Components/Camera.hpp"
#include <Rocket/Core.h>
#include <Rocket/Core/SystemInterface.h>

namespace SpaceMarines
{

namespace Module
{

class GUI
{
public:
	GUI(Renderer* renderer);
	virtual ~GUI();
	void render(int mouseX, int mouseY, bool isMouseClicked);
	bool init();

	void loadGUIDoc(const char* path);

protected:
	class RenderInterface: public Rocket::Core::RenderInterface
	{
	public:
		RenderInterface();

		/// Called by Rocket when it wants to render geometry that it does not wish to optimise.
		virtual void RenderGeometry(Rocket::Core::Vertex* vertices,
				int num_vertices, int* indices, int num_indices,
				Rocket::Core::TextureHandle texture,
				const Rocket::Core::Vector2f& translation);

		/// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
		virtual Rocket::Core::CompiledGeometryHandle CompileGeometry(
				Rocket::Core::Vertex* vertices, int num_vertices, int* indices,
				int num_indices, Rocket::Core::TextureHandle texture);

		/// Called by Rocket when it wants to render application-compiled geometry.
		virtual void RenderCompiledGeometry(
				Rocket::Core::CompiledGeometryHandle geometry,
				const Rocket::Core::Vector2f& translation);
		/// Called by Rocket when it wants to release application-compiled geometry.
		virtual void ReleaseCompiledGeometry(
				Rocket::Core::CompiledGeometryHandle geometry);

		/// Called by Rocket when it wants to enable or disable scissoring to clip content.
		virtual void EnableScissorRegion(bool enable);
		/// Called by Rocket when it wants to change the scissor region.
		virtual void SetScissorRegion(int x, int y, int width, int height);

		/// Called by Rocket when a texture is required by the library.
		virtual bool LoadTexture(Rocket::Core::TextureHandle& texture_handle,
				Rocket::Core::Vector2i& texture_dimensions,
				const Rocket::Core::String& source);
		/// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
		virtual bool GenerateTexture(
				Rocket::Core::TextureHandle& texture_handle,
				const Rocket::Core::byte* source,
				const Rocket::Core::Vector2i& source_dimensions);
		/// Called by Rocket when a loaded texture is no longer required.
		virtual void ReleaseTexture(Rocket::Core::TextureHandle texture_handle);
	};

private:
	Renderer* renderer;
	float hey;
	Rocket::Core::Context* context;

	class SystemInterface : public Rocket::Core::SystemInterface
	{
		float GetElapsedTime() { return Time::time; }
	};

	SystemInterface* systemInterface;
	RenderInterface* renderInterface;
	Rocket::Core::ElementDocument* mainGUI;

};

} /* namespace Module */
} /* namespace SpaceMarines */
