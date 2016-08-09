#include <SDL.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include "util.h"

namespace util {
  const char * source(GLenum s) {
    switch (s) {
    case GL_DEBUG_SOURCE_API: return "API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "window system";
    case GL_DEBUG_SOURCE_SHADER_COMPILER: return "shader compiler";
    case GL_DEBUG_SOURCE_THIRD_PARTY: return "third party";
    case GL_DEBUG_SOURCE_APPLICATION: return "application";
    case GL_DEBUG_SOURCE_OTHER: return "other";
    default: return "unknown";
    }
  }

  const char * message_type(GLenum t) {
    switch (t) {
    case GL_DEBUG_TYPE_ERROR: return "error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "deprecated behavior";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "undefined behavior";
    case GL_DEBUG_TYPE_PORTABILITY: return "portability";
    case GL_DEBUG_TYPE_PERFORMANCE: return "performance";
    case GL_DEBUG_TYPE_MARKER: return "marker";
    case GL_DEBUG_TYPE_PUSH_GROUP: return "push group";
    case GL_DEBUG_TYPE_POP_GROUP: return "pop group";
    case GL_DEBUG_TYPE_OTHER: return "other";
    default: return "unknown";
    }
  }

  void objectLabelf(GLenum id, GLuint name, const char *fmt, ...) {
    va_list args;
    char buffer[2048];

    va_start(args, fmt);
    vsprintf_s(buffer, fmt, args);
    va_end(args);

    glObjectLabel(id, name, static_cast<GLsizei>(strnlen_s(buffer, 2048)), buffer);
  }

  void die(const char * title, const char *fmt, ...) {
    va_list args;
    char buffer[2048];

    va_start(args, fmt);
    vsprintf_s(buffer, fmt, args);
    va_end(args);

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, buffer, NULL);
    exit(1);
  }
}