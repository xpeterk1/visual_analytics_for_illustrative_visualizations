#include "shader_utilities.h"

std::string load_file(std::filesystem::path file_path) {
    file_path.make_preferred();
    std::ifstream infile{ file_path };

    if (!infile.is_open()) {
        std::cerr << "File " + file_path.generic_string() + "not found.";
    }

    return { std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };
}

GLuint create_shader(std::filesystem::path file_path, GLenum shader_type) {
    const std::string shader_string = load_file(file_path);
    const char* shader_source = shader_string.c_str();

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, nullptr);
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(shader); // Don't leak the shader.

        throw errorLog;
    }

    return shader;
}

GLuint create_program(std::filesystem::path vertex_path, std::filesystem::path fragment_path) {
    try {
        GLuint vertex_shader = create_shader(vertex_path, GL_VERTEX_SHADER);
        GLuint fragment_shader = create_shader(fragment_path, GL_FRAGMENT_SHADER);

        GLuint program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);
            // Don't leak shaders either.
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);

            // Use the infoLog as you see fit.

            // In this simple program, we'll just leave
            throw infoLog;
        }

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        glDetachShader(program, vertex_shader);
        glDetachShader(program, fragment_shader);

        return program;
    }
    catch (std::vector<GLchar> e) {
        for (auto c : e) {
            std::cout << c;
        }
        std::cout << std::endl;
        return -1;
    }
}