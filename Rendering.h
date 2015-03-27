#pragma once
#include "SystemDef.h"
#include "crenderutils.h"

sysDef(Rendering)

bool init() { return true; }
bool term() { return true; }

bool condition(Entity *e)
{
	return e->active && e->render && e->transform;
}

void update(Entity *e)
{
	////// Macros
	auto &r = *e->render;
	auto &t = *e->transform;
	auto &assets = core(Asset);
	auto &camera = core(Camera).getCamera();
	auto mesh    = assets.getMesh   (r.mesh);
	auto shader  = assets.getShader (r.shader);
	auto texture = assets.getTexture(r.texture);
	////// Update Frame
	if (!r.texture.empty())
	{
		r.frame += r.frameRate * core(Time).getDeltaTime();
		if (r.frame >= texture.row * texture.col)
			r.frame = 0/*texture.row * texture.col - 1*/;
	};
	////// 
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.handle);
	GLuint ul_texture = glGetUniformLocation(shader, "Diffuse");
	glUniform1i(ul_texture, 0);
	
	////// Frame info		/*
	GLuint ul_frame = glGetUniformLocation(shader, "frame");
	GLuint ul_row	= glGetUniformLocation(shader, "rows");
	GLuint ul_col	= glGetUniformLocation(shader, "cols");
	glUniform1i(ul_row, texture.row);
	glUniform1i(ul_col, texture.col);
	glUniform1i(ul_frame, r.frame);
	////// Camera
	GLuint ul_proj    = glGetUniformLocation(shader, "Projection");
	GLuint ul_view    = glGetUniformLocation(shader, "View");
	GLuint ul_model	  = glGetUniformLocation(shader, "Model");
	glUniformMatrix4fv(ul_proj,  1, GL_FALSE, camera.projection.m);
	glUniformMatrix4fv(ul_view,  1, GL_FALSE, camera.view.m);
	glUniformMatrix4fv(ul_model, 1, GL_FALSE, t.m_global.m);
	////// 
	glBindVertexArray(mesh.VAO);
	glDrawElements(GL_TRIANGLES, mesh.size, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);	
}


sysEndDef