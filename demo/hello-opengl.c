#include "./../src/gf.h"

#include <GL/gl.h>
#include <GL/glext.h>

static const float		g_vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f
};

static const uint32_t	g_indices[] = {
	0, 1, 2,
	1, 2, 3
};

static const char		*g_glsl_vert = 
"#version 460 core\n"
"layout (location = 0) in vec3 a_Pos;\n"
"void main() {\n"
" 	gl_Position = vec4(a_Pos, 1.0f);\n"
"}";
    
static const char		*g_glsl_frag = 
"#version 460 core\n"
"out vec4 f_Col;\n"
"void main() {\n"
"   f_Col = vec4(0.8f, 0.2f, 0.2f, 1.0f);\n"
"}";

PFNGLCREATESHADERPROC				glCreateShader;
PFNGLSHADERSOURCEPROC				glShaderSource;
PFNGLCOMPILESHADERPROC				glCompileShader;
PFNGLDELETESHADERPROC				glDeleteShader;

PFNGLCREATEPROGRAMPROC				glCreateProgram;
PFNGLATTACHSHADERPROC				glAttachShader;
PFNGLLINKPROGRAMPROC				glLinkProgram;
PFNGLUSEPROGRAMPROC					glUseProgram;
PFNGLDELETEPROGRAMPROC				glDeleteProgram;

PFNGLGENVERTEXARRAYSPROC			glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC			glBindVertexArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC	glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC		glVertexAttribPointer;
PFNGLDELETEVERTEXARRAYSPROC			glDeleteVertexArrays;

PFNGLGENBUFFERSPROC					glGenBuffers;
PFNGLBINDBUFFERPROC					glBindBuffer;
PFNGLBUFFERDATAPROC					glBufferData;
PFNGLDELETEBUFFERSPROC				glDeleteBuffers;

int	main(void) {
	t_window	window;
	GLuint		shader;
	GLuint		vertex_array;
	GLuint		vertex_buffer, index_buffer;

	if (
		!gf_createWindow(&window, 960, 540, "gf 1.0 - Hello, OpenGL!") ||
		!gf_makeCurrent(window)
	) {
		return (1);
	}

	/* SECTION:
	 *  Manual OpenGL loading
	 * */
	{
		glCreateShader = gf_getProcAddress("glCreateShader");
		glShaderSource = gf_getProcAddress("glShaderSource");
		glCompileShader = gf_getProcAddress("glCompileShader");
		glDeleteShader = gf_getProcAddress("glDeleteShader");

		glCreateProgram = gf_getProcAddress("glCreateProgram");
		glAttachShader = gf_getProcAddress("glAttachShader");
		glLinkProgram = gf_getProcAddress("glLinkProgram");
		glUseProgram = gf_getProcAddress("glUseProgram");
		glDeleteProgram = gf_getProcAddress("glDeleteProgram");

		glGenVertexArrays = gf_getProcAddress("glGenVertexArrays");
		glBindVertexArray = gf_getProcAddress("glBindVertexArray");
		glEnableVertexAttribArray = gf_getProcAddress("glEnableVertexAttribArray");
		glVertexAttribPointer = gf_getProcAddress("glVertexAttribPointer");
		glDeleteVertexArrays = gf_getProcAddress("glDeleteVertexArrays");

		glGenBuffers = gf_getProcAddress("glGenBuffers");
		glBindBuffer = gf_getProcAddress("glBindBuffer");
		glBufferData = gf_getProcAddress("glBufferData");
		glDeleteBuffers = gf_getProcAddress("glDeleteBuffers");
	}
	/* SECTION:
	 *  Shader creation
	 * */
	{
		GLuint	shader_vertex, shader_fragment;

		shader = glCreateProgram();

		shader_vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(shader_vertex, 1, &g_glsl_vert, 0);
		glCompileShader(shader_vertex);
		glAttachShader(shader, shader_vertex);
		
		shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shader_fragment, 1, &g_glsl_frag, 0);
		glCompileShader(shader_fragment);
		glAttachShader(shader, shader_fragment);

		glLinkProgram(shader);
		glDeleteShader(shader_vertex);
		glDeleteShader(shader_fragment);
	}
	/* SECTION:
	 *  Vertex objects creation
	 * */
	{
		vertex_array = vertex_buffer = index_buffer = 0;
		glGenVertexArrays(1, &vertex_array);
		glBindVertexArray(vertex_array);

		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices), g_vertices, GL_STATIC_DRAW);
		
		glGenBuffers(1, &index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) (0 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	for (bool exit = false; !exit; ) {
		t_event		event;
	
		while (gf_pollEvents(window, &event)) {
			switch (event.type) {

				case (GF_EVENT_QUIT): {
					exit = true;
				} break;

				case (GF_EVENT_RESIZE): {
					glViewport(0, 0, event.s_data.i[0], event.s_data.i[1]);
				} break;

			}
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glUseProgram(shader);
		glBindVertexArray(vertex_array);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		gf_swapBuffers(window);
	}
		
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &index_buffer); index_buffer = 0;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vertex_buffer); vertex_buffer = 0;
	
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vertex_array); vertex_array = 0;
	
	glUseProgram(0);
	glDeleteProgram(shader); shader = 0;
	
	gf_destroyWindow(window);
	
	return (0);
}
