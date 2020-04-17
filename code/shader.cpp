enum Shaders
{
    BASIC_SHADER,
    LAST_SHADER
};

enum ShaderType
{
	VERTEX,
	FRAGMENT,
	PROGRAM
};

global u32 shader_cache[LAST_SHADER];

internal b32
check_compile_errors(u32 shader, ShaderType type)
{
	i32 success;
	char infoLog[1024];
	const char *types[3] = { "VERTEX", "FRAGMENT", "PROGRAM" };
	const char *type_string;
	b32 isProgram = (type == PROGRAM);
	if (isProgram)
	{
		type_string = types[2];
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			return false;
		}
	}
	else
	{
		type_string = type == VERTEX ? types[0] : types[1];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			return false;
		}
	}

	return true;
}

internal u32
init_shader(const char *shader_name, u32 shader_cache_id)
{
	char vs_path[128];
	char fs_path[128];
	sprintf_s(vs_path, "../res/shaders/%s.vert", shader_name);
	sprintf_s(fs_path, "../res/shaders/%s.frag", shader_name);

	char *vs_text = read_file(vs_path);
	char *fs_text = read_file(fs_path);

	u32 vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vs_text, NULL);
	glCompileShader(vertex);
	check_compile_errors(vertex, VERTEX);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fs_text, NULL);
	glCompileShader(fragment);
	check_compile_errors(fragment, FRAGMENT);

	u32 program;
	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	check_compile_errors(program, PROGRAM);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	free(vs_text);
	free(fs_text);

	shader_cache[shader_cache_id] = program;
	return program;
}

internal void shader_use(u32 id)
{
	glUseProgram(id);
}

// BASIC TYPES
inline void uniform_set_bool(u32 id, const char *name, b32 value)
{
	glUniform1i(glGetUniformLocation(id, name), value);
}

inline void uniform_set_int(u32 id, const char *name, i32 value)
{
	glUniform1i(glGetUniformLocation(id, name), value);
}

inline void uniform_set_float(u32 id, const char *name, float value)
{
	glUniform1f(glGetUniformLocation(id, name), value);
}

// VECTOR TYPES
inline void uniform_set_vec2(u32 id, const char *name, float x, float y)
{
	glUniform2f(glGetUniformLocation(id, name), x, y);
}

inline void uniform_set_vec2(u32 id, const char *name, hmm_vec2 &value)
{
	glUniform2f(glGetUniformLocation(id, name), value.X, value.Y);
}

inline void uniform_set_vec3(u32 id, const char *name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(id, name), x, y, z);
}

inline void uniform_set_vec3(u32 id, const char *name, hmm_vec3 &value)
{
	glUniform3f(glGetUniformLocation(id, name), value.X, value.Y, value.Z);
}

inline void uniform_set_vec4(u32 id, const char *name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(id, name), x, y, z, w);
}

inline void uniform_set_vec4(u32 id, const char *name, hmm_vec4 &value)
{
	glUniform4f(glGetUniformLocation(id, name), value.X, value.Y, value.Z, value.W);
}

// MATRIX TYPES
inline void uniform_set_mat4(u32 id, const char *name, hmm_mat4 &value)
{
	glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, &value.Elements[0][0]);
}
